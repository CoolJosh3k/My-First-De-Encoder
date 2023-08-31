#include "include/decode.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <filesystem>
#include <ios>
#include <string>
#include <vector>
#include <bitset>

#include "include/error_strings.h"
#include "include/limits.h"
#include "include/buffers.h"
#include "include/formats.h"
#include "include/file_streams.h"

FileStream fs {};

char iter {1};
char mode {mode_8};
char pad {0};
uint8_t num_mode_segs_per_byte {2};

std::string in_file_string {};
std::string out_file_string {};
std::string out_file_string_temp {};
uintmax_t chars_read {0};	//Total chars read in so far
uint8_t progress {0};
const uint8_t progress_bar_size {101};
const char progress_bar_empty[progress_bar_size] {"                                                                                                    "};
typeof(iter) expected_iterations {1};

bool Decoder::SetInOutFileStrings(char* const in, char* const out) {
	in_file_string.assign(in);
	out_file_string.assign(out);
	out_file_string_temp.assign(out+ext.temp);
	return true;
}

bool RenameOutFile() {
	if (std::filesystem::exists(out_file_string.c_str())) {
		if (std::remove(out_file_string.c_str())) {
			printf("\nError removing previous %s file!\n", ext.done.c_str());
			return false;
		}
	}
	if (std::rename(out_file_string_temp.c_str(), out_file_string.c_str())) {
		printf("\nError renaming temp file!\n");
		return false;
	}
	return true;
}

bool Decoder::OpenFiles() {
	if (!fs.OpenInFile(&in_file_string)) return false;
	if (!fs.OpenOutFile(&out_file_string_temp)) return false;
	return true;
}

bool Decoder::CloseFiles() {
	bool success {true};
	success = fs.CloseInFile();
	if ((success = fs.CloseOutFile())) success = RenameOutFile();
	return success;
}

void GetParams(const char* const header) {
	const char im {0b1111 << ITER_BITS_POS};	//Iterations mask
	const char pm {0b11 << PAD_BITS_POS};		//Paddings mask
	mode = (*header & MODE_BITS_MASK) >> MODE_BITS_POS;
	iter = ((*header & im) >> ITER_BITS_POS) + 1;
	pad = (*header & pm) >> PAD_BITS_POS;
}

bool Decoder::Decode() {
	DecoderBuffer dbuf {};
	uintmax_t in_sz {0};			//Number of characters left to read
	uintmax_t out_sz {0};			//Number of characters to be written
	char*& in {dbuf.buffer.at(0)};
	char*& out {dbuf.buffer.at(1)};
	bool fi {true};					//First iteration
	uintmax_t di {0};				//Current Decode iteration
	
	while (true) {
		
		if (fi) {
			fi = false;
			
			//Get first lot of paramaters
			char h = fs.in_fs.peek();	//Header
			GetParams(&h);
			di = iter;
			expected_iterations = iter;
			
			//Allocate arrays
			uintmax_t b {(MODE8_MAX_SEQ_LEN * num_mode_segs_per_byte)/8};	//Most out bytes for 1 in byte
			uintmax_t m = (fs.GetInFileSize() - HEADER_SIZE) * b;	//Max possible length
			for (int i {1}; i < expected_iterations; ++i) {
				m = (m-HEADER_SIZE) * b;	//Max possible length including more than a single iteration
			}
			if(!dbuf.CreateBuffers(m)) return false;
			
			//Get initial data
			in_sz = fs.GetInFileSize();
			fs.in_fs.read(in, in_sz);
			in_sz -= HEADER_SIZE;
			
		} else {
			
			//Swap buffers
			auto t = in;
			in = out;
			out = t;
			
			GetParams(in);
			
			in_sz = out_sz - HEADER_SIZE;
		}
		
		//Decode
		auto in_t {in + HEADER_SIZE};	//Temporary in reference
		auto out_t {out};				//Temporary out reference
		out_sz = 0;
		uint8_t cb {0};	//Current bit
		do {	//We have not finished the reading the in chars
			uint8_t cs = num_mode_segs_per_byte - 1U;	//Current segment
			do {	//We have not finished the segments
				if (mode == mode_8) {
					uint8_t nb {0b111};	//Next bit
					nb <<= 4*cs;
					nb &= *in_t;
					nb >>= 4*cs;
					nb += 1;
					while (nb) {	//We have not finished the sequence
						
						//Set the out bits
						if (*in_t & 1 << (3 + 4*cs)) {
							*out_t |= 1 << cb;
						} else {
							*out_t &= ~(1 << cb);
						}
					
						if (cb == MODE8_LIM) {	//Reached the end of a out char
							cb = 0;
							++out_sz;
							++out_t;
						} else {
							++cb;
						}
						
						--nb;
					}
				}
				
				if (!cs) break;
				--cs;
				if (pad & !cs & (in_sz == 1)) break;	//Rest of in is just padding bits
			} while (true);
			
			--in_sz;
			if (!in_sz) break;
			++in_t;
		} while (true);
		
		--di;
		
		if (di) continue;	//Go around again if we still have iterations to go through
		
		fs.out_fs.write(out, out_sz);
		
		break;
	}
	
	return true;
}
#include "include/encode.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <filesystem>
#include <ios>
#include <cstring>
#include <limits>
#include <climits>

#include "include/error_strings.h"
#include "include/limits.h"
#include "include/buffers.h"
#include "include/formats.h"
#include "include/file_streams.h"

FileStream fs {};

char iter {1};
char mode {mode_8};
uint8_t num_mode_segs_per_byte {2};

std::string in_file_string {};
std::string out_file_string {};
std::string out_file_string_new {};
std::string out_file_string_done {};

uint8_t encode_iter {0};	//Current encode iteration
uintmax_t chars_read {0};	//Total chars read in so far
uint8_t progress {0};
typeof(progress) progress_last {progress};
const uint8_t progress_bar_size {101};
const char progress_bar_empty[progress_bar_size] {"                                                                                                    "};
typeof(iter) expected_iterations {1};

bool Encoder::SetInOutFileStrings(const char* in) {
	in_file_string.assign(in);
	SetOutFileStrings(in);
	return true;
}

bool Encoder::SetOutFileStrings(const char* out) {
	out_file_string.assign(out);
	out_file_string_new = out_file_string + ext.done_temp;
	out_file_string_done = out_file_string + ext.done;
	return true;
}

bool Encoder::SetIter(char* const argv) {
	char* p = argv;
	
	if (*argv == '-') ++p;
	if (!*p) {
		err_msg.PrintIterNotNum();
		return false;
	}
	
	while (*p) {
		if (!isdigit(*p)) {
			err_msg.PrintIterNotNum();
			return false;
		}
		++p;
	}
	
	if (*argv == '-') {
		err_msg.PrintIterNeg();
		return false;
	}
	
	if (std::strlen(argv) > (std::strlen(std::to_string(std::numeric_limits<int>::max()).c_str()) -1)) {	//Too many character making a too large number?
		err_msg.PrintIterTooLarge();
		return false;
	} else {
		auto argv_iter = atoi(argv);
		if (argv_iter > ITER_LIM) {
			err_msg.PrintIterTooLarge();
			return false;
		}
		iter = static_cast<char>(argv_iter);
	}
	
	if (iter == 0) {
		err_msg.PrintIterZero();
		return false;
	}
	
	expected_iterations = iter;
	
	return true;
}

bool RenameOutFile() {
	if (std::filesystem::exists(out_file_string_done.c_str())) {
		if (std::remove(out_file_string_done.c_str())) {
			printf("\nError removing previous %s file!\n", ext.done.c_str());
			return false;
		}
	}
	if (std::rename(out_file_string_new.c_str(), out_file_string_done.c_str())) {
		printf("\nError renaming temp file!\n");
		return false;
	}
	return true;
}

bool Encoder::OpenFiles() {
	if (!fs.OpenInFile(&in_file_string)) return false;
	if (!fs.OpenOutFile(&out_file_string_new)) return false;
	return true;
}

bool Encoder::CloseFiles() {
	bool success {true};
	success = fs.CloseInFile();
	if ((success = fs.CloseOutFile())) success = RenameOutFile();
	return success;
}

bool WriteChar(const char* const out) {
	fs.out_fs.put(*out);
	if (!fs.out_fs.good()) {
		fs.out_fs.clear();
		err_msg.PrintWriteFail();
		return false;
	}
	return true;
}

void ResetProgress(char progress_bar[]) {
	chars_read = 0;
	progress = 0;
	*progress_bar = *progress_bar_empty;
}

void UpdateProgress() {
	progress_last = progress;
	progress = static_cast<float>(chars_read)/static_cast<float>(fs.GetInFileSize())*100;	//progress
}

void PrintProgress(char progress_bar[]) {
	if (progress <= progress_last) return;
	
	if (cout_buffer.GetHasCoutBuffer()) printf("\rIteration %u of %i; Progress %.3u%% [%s]", encode_iter+1, expected_iterations, progress, progress_bar);
}

bool Encoder::Encode() {
	Encode:
	char hB {0};		//Header byte
	uint8_t pad {0};	//Foot padding
	bool fc {true};		//First char
	char in {0};		//In char
	int8_t fb {0};		//First bit value
	int8_t nb {0};		//Next bit value
	int8_t lb {0};		//Last bit value
	uint8_t n {0};		//Number of sequential bits
	char out {0};		//Output data
	uint8_t s {0};		//Segment
	//Progress bar
	char pb[progress_bar_size]; std::copy(std::begin(progress_bar_empty), std::end(progress_bar_empty), std::begin(pb));
	
	WriteChar(&hB);	//prepare first byte for later
	
	UpdateProgress();
	printf("\n");
	PrintProgress(pb);
	
	fs.in_fs.clear();
	while (true) {
		fs.in_fs.get(in);
		
		fb = in & 1;
		if (fc) lb = nb = fb;
		
		if (lb != fb) {
			lb -= 2;
		} else {
			nb = in & 1;
		}
		
		uint8_t cb {0};	//Current bit
		while (true) {
			while ((s < num_mode_segs_per_byte) && (cb != 8)) {
				if ((nb == lb) && ((n-1) != MODE8_LIM) && (fs.in_fs.good())) {
					lb = in & 1;
					in >>= 1;
					++n;
					++cb;
					nb = in & 1;
				} else {
					if (lb < 0) lb += 2;
					out <<= 8/num_mode_segs_per_byte * s;
					out |= lb << 3;
					--n;
					out |= n;
					n = 0;
					lb = nb;
					++s;
					if (!fs.in_fs.good()) cb = 8;
				}
			}
			if (cb == 8) {	//Exited loop early due to finishing the char
				fc = false;
				break;
			} else {
				WriteChar(&out);
				out = 0;
				s = 0;
			}
		}
		
		if (fs.in_fs.good()) ++chars_read;
		UpdateProgress();
		for (int i {0}; i < progress; ++i) {
			pb[i] = '#';
		}
		PrintProgress(pb);
		
		if (!fs.in_fs.good()) break;
	}
	
	//Do padding
	auto rs = num_mode_segs_per_byte - s;	//Remaining segments
	out <<= 8/num_mode_segs_per_byte * rs;
	pad = rs;
	
	WriteChar(&out);
	
	hB |= encode_iter << ITER_BITS_POS;
	hB |= mode << MODE_BITS_POS;
	hB |= pad << PAD_BITS_POS;
	fs.out_fs.seekp(0);
	fs.out_fs.put(hB);
	
	if (iter-1) {
		++encode_iter;
		--iter;
		CloseFiles();
		fs.OpenInFile(&out_file_string_done);
		fs.OpenOutFile(&out_file_string_new);
		ResetProgress(pb);
		goto Encode;
	}
	
	printf("\n");
	
	return true;
}

#if DEBUG
	void Encoder::Print() {
		std::cout << in_file << std::endl;
		std::cout << out_file << std::endl;
		printf("%i\n", iter);
	}
#endif
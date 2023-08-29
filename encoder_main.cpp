#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <filesystem>

#include "include/error_strings.h"
#include "include/encode.h"
#include "include/buffers.h"

namespace fs = std::filesystem;

const char VERSION[] {"1.0.5"};

struct arg_flag {
	const uint8_t success {in};
	const uint8_t fail {1};
	const uint8_t in {1<<1};
	const uint8_t out {1<<2};
	const uint8_t iter {1<<3};
};

void PrintUsage(char* const path){
	auto fn {fs::path(path).filename().c_str()};
	#if WINDOWS
		printf("\nUsage: %ls in [-o out] [-i number | -t number]\n", fn);
	#else
		printf("\nUsage: %s in [-o out] [-i number | -t number]\n", fn);
	#endif
	printf("\tin: input file\n");
	printf("\tout: output file. File extention will be added automatically.\n");
	printf("\t     Defaults to same as input location.\n");
	printf("\tnumber: number of iterations.\n");
	printf("\t        Defaults to 1.\n");
}

void HandleArgs(int& argc, char** const argv, uint8_t* const valid, const arg_flag* const flag) {
	const struct {
		const std::string out {"-o"};
		const std::string iter {"-i"};
		const std::string iter_alt {"-t"};
	} specifier;
	
	std::string arg {""};
	
	for (int i = 2; i < argc; ++i) {
		
		arg.assign(argv[i]);
		if (arg == specifier.out) {
			if (*valid & flag->out) {
				err_msg.PrintRepeatArg();
			} else {
				if (i+1 == argc) {
					err_msg.PrintTooFewArg();
					*valid |= flag->fail;
					return;
				} else if (Encoder::SetOutFileStrings(argv[i+1])) {
					*valid |= flag->out;
				}
			}
			
			++i;
			if (i == argc) {
				return;
			} else continue;
		}
		
		if (arg == specifier.iter || arg == specifier.iter_alt) {
			if (*valid & flag->iter) {
				err_msg.PrintRepeatArg();
			} else {
				if (i+1 == argc) {
					err_msg.PrintTooFewArg();
					*valid |= flag->fail;
					return;
				} else if (!(Encoder::SetIter(argv[i+1]) ? *valid |= flag->iter : *valid |= flag->fail)) {
					*valid |= flag->fail;
					return;
				}
			}
			
			++i;
			if (i == argc) {
				return;
			} else continue;
		}
	}
}

int main(int argc, char** argv) {
	uint8_t valid {0};
	const arg_flag flag {};
	
	cout_buffer.SetupConsoleOutBuffer();
	
	bool argv_err = false;
	if (argc <= 1) argv_err = true;
	for (int i {1}; i < argc; ++i) {
		if (!argv[i]) {
			argv_err = true;
			break;
		}
	}
	
	if (argv_err) {
		printf("\nVersion: %s\n", VERSION);
		PrintUsage(argv[0]);
		std::cout << std::endl;
		return EXIT_FAILURE;
	}
	
	if (Encoder::SetInOutFileStrings(argv[1])) {
		valid |= flag.in;
		HandleArgs(argc, argv, &valid, &flag);
	} else {
		valid |= flag.fail;
	}
	

	#if DEBUG
		Encoder::Print();
	#endif
	
	valid &= ~flag.out;
	valid &= ~flag.iter;
	if (valid != flag.success) {
		PrintUsage(argv[0]);
		return EXIT_FAILURE;
	}
	
	if (!Encoder::OpenFiles()) return EXIT_FAILURE;
	
	if (!Encoder::Encode()) {
		printf("\nSomething went wrong during encoding!\n");
		return EXIT_FAILURE;
	}
	
	if (!Encoder::CloseFiles()) return EXIT_FAILURE;
	
	printf("\nDone!\n");
	
	return EXIT_SUCCESS;
}
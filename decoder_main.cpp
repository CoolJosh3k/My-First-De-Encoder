#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <filesystem>

#include "include/error_strings.h"
#include "include/decode.h"
#include "include/buffers.h"

namespace fs = std::filesystem;

const char VERSION[] {"1.0.3"};

void PrintUsage(char* const path) {
	std::string fn {NULL};
	try {
		fn = fs::path(path).filename();
	} catch(const std::bad_alloc& e) {
		fn = "PROGRAM_NAME";
	}
	#if WINDOWS
		printf("\nUsage: %ls in out\n", fn.c_str());
	#else
		printf("\nUsage: %s in out\n", fn.c_str());
	#endif
	printf("\tin: input file\n");
	printf("\tout: output file\n");
}

int main (int argc, char* argv[]) {
	
	cout_buffer.SetupConsoleOutBuffer();
	
	bool argv_err = false;
	if (argc <= 2) argv_err = true;
	for (int i {1}; i < argc; ++i) {
		if (!argv[i]) {
			argv_err = true;
			break;
		}
	}
	
	if (argv_err) {
		printf("\nVersion: %s\n", VERSION);
		PrintUsage(argv[0]);
		return EXIT_FAILURE;
	}
	
	Decoder::SetInOutFileStrings(argv[1], argv[2]);
	
	if (!Decoder::OpenFiles()) return EXIT_FAILURE;
	
	if (!Decoder::Decode()) {
		printf("\nSomething went wrong during decoding!\n");
		return EXIT_FAILURE;
	}
	
	if (!Decoder::CloseFiles()) return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}
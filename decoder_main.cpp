#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <filesystem>

#include "include/error_strings.h"
#include "include/decode.h"
#include "include/buffers.h"

namespace fs = std::filesystem;

const char VERSION[] {"1.0.0"};

void PrintUsage(char* const path) {
	auto fn {fs::path(path).filename().c_str()};
	#if WINDOWS
		printf("\nUsage: %ls in out\n", fn);
	#else
		printf("\nUsage: %s in out\n", fn);
	#endif
	printf("\tin: input file\n");
	printf("\tout: output file\n");
}

int main (int argc, char* argv[]) {
	
	cout_buffer.SetupConsoleOutBuffer();
	
	if (argc <= 2) {
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
	if (!Decoder::RenameOutFile()) return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}
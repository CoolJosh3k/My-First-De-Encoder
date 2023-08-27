#include "include/file_streams.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <filesystem>
#include <string>

#include "include/error_strings.h"
#include "include/limits.h"
#include "include/formats.h"

bool FileStream::OpenInFile(std::string* in) {
	in_fs.clear();
	in_fs.open(*in, std::ios::binary | std::ios::in);// | std::ios::noreplace);
	if (!in_fs.good()) {
		printf("Error reading input file!\n");
		return false;
	}
	
	std::error_code e {};
	in_file_size = std::filesystem::file_size(*in, e);
	if (e) printf("\nFailed to get input file size. Cannot report progress.\n");
	
	return true;
}

bool FileStream::OpenOutFile(std::string* out) {
	out_fs.clear();
	out_fs.open(*out, std::ios::binary | std::ios::out | std::ios::trunc);// | std::ios::noreplace);
	if (!out_fs.good()) {
		printf("\nError preparing output file!\n");
		return false;
	}
	return true;
}

bool FileStream::CloseInFile() {
	in_file_size = 0;
	in_fs.clear();
	if (in_fs.is_open()) in_fs.close();
	else return true;
	
	if (!in_fs.good()) {
		in_fs.clear();
		printf("\nError closing input file!\n");
		return false;
	}
	return false;
}

bool FileStream::CloseOutFile() {
	out_fs.clear();
	if (out_fs.is_open()) out_fs.close();
	else return true;
	
	if (!out_fs.good()) {
		out_fs.clear();
		printf("\nError closing output file!\n");
		return false;
	}
	return true;
}
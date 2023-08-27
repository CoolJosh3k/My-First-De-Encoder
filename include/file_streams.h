#pragma once

#include <iostream>
#include <fstream>
#include <stdint.h>

class FileStream {
	public:
		FileStream() {
			typeof(in_fs) in_fs {};
			typeof(in_file_size) in_file_size {};
			(void)in_file_size;	//We will actually be using the variable so supress warning
			typeof(out_fs) out_fs {};
		}
		~FileStream() {
			CloseInFile();
			CloseOutFile();
		}
		bool OpenInFile(std::string* in);
		bool OpenOutFile(std::string* out);
		bool CloseInFile();
		bool CloseOutFile();
		std::ifstream in_fs;
		std::ofstream out_fs;
	
	private:
		uintmax_t in_file_size;
	
	public:
		auto GetInFileSize() {return in_file_size;};
};
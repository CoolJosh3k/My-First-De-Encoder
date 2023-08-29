#pragma once

#include <sys/stat.h>
#include <array>
#include <cstddef>

class CoutBuffer {
	public:
		CoutBuffer();
		void SetupConsoleOutBuffer();
		int GetCoutBufferFail();
		bool GetHasCoutBuffer();
	
	private:
		int cout_buf_fail;
		bool has_cout_buf;
};

static CoutBuffer cout_buffer {};

class DecoderBuffer {
	public:
		DecoderBuffer();
		~DecoderBuffer();
		void CreateBuffers(size_t size);
		char* GetBufferPointer(size_t buffer_index = 0);
		
	private:
		std::array<char*, 2> buffer;
};
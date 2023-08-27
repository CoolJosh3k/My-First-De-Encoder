#pragma once

#include <sys/stat.h>

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
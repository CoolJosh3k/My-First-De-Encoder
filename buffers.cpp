#include "include/buffers.h"

#include <stdio.h>

char buffer[256];

CoutBuffer::CoutBuffer() {
	cout_buf_fail = 0;
	has_cout_buf = 0;
}

void CoutBuffer::SetupConsoleOutBuffer() {
	cout_buf_fail = setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));
	if (cout_buf_fail) printf("\nCould not prepare console output buffer!\nSome information will not be shown.\n");
}

int CoutBuffer::GetCoutBufferFail() {
	return cout_buf_fail;
}

bool CoutBuffer::GetHasCoutBuffer() {
	if (cout_buf_fail) return false;
	return true;
}
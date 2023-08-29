#pragma once
#include <string>
#include "limits.h"

class ErrMsg {
	public:
		void PrintTooFewArg();
		void PrintRepeatArg();
		void PrintIterNotNum();
		void PrintIterNeg();
		void PrintIterZero();
		void PrintIterTooLarge();
		void PrintWriteFail();
	
	private:
		const std::string too_few_arg {"\nError: too few arguments!\n"};
		const std::string repeat_arg {"\nSame argument specified mulitple times. Ignoring duplicates!\n"};
		const std::string iter_not_num {"\nNumber of iterations must be a number.\n"};
		const std::string iter_neg {"\nNumber of iterations cannot be negative.\n"};
		const std::string iter_zero {"\nNumber of iterations cannot be zero.\n"};
		const std::string write_fail {"\nFailed to write to output file!\n"};
};
static ErrMsg err_msg {};
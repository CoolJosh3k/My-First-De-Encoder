#include "include/error_strings.h"

#include <iostream>

void ErrMsg::PrintTooFewArg() {
	std::cout << ErrMsg::too_few_arg << std::endl;
}

void ErrMsg::PrintRepeatArg() {
	std::cout << ErrMsg::repeat_arg << std::endl;
}

void ErrMsg::PrintIterNotNum() {
	std::cout << ErrMsg::iter_not_num << std::endl;
}

void ErrMsg::PrintIterNeg() {
	std::cout << ErrMsg::iter_neg << std::endl;
}

void ErrMsg::PrintIterZero() {
	std::cout << ErrMsg::iter_zero << std::endl;
}

void ErrMsg::PrintIterTooLarge() {
	//std::cout << ErrMsg::iter_too_large << std::endl;
	printf("\nNumber of iterations must be %i at most.\n", ITER_LIM);
}

void ErrMsg::PrintWriteFail() {
	std::cout << ErrMsg::write_fail << std::endl;
}
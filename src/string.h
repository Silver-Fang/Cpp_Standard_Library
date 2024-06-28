#pragma once
#include_next <string.h>
#ifdef ARDUINO_ARCH_AVR
#include <sys/cdefs.h>
// 38
extern char *__strchrnul(const char *__s, int __c)
	__attribute_pure__;
// 41
#endif
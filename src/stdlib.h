#ifndef ARDUINO_ARCH_ESP32
// ESP32非常傻逼地定义了多个stdlib在各种稀奇古怪的条件下互相包含，所以不能#pragma once
#pragma once
#endif
#include_next <stdlib.h>
#ifdef ARDUINO_ARCH_AVR
#include <libc-symbols.h>
// 355
struct abort_msg_s
{
	unsigned int size;
	char msg[0];
};
extern struct abort_msg_s *__abort_msg;
libc_hidden_proto(__abort_msg)
// 363
#endif
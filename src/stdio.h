#pragma once
#ifdef ARDUINO_ARCH_AVR
#define snprintf __glibc_snprintf
#endif
#include_next <stdio.h>
#ifdef ARDUINO_ARCH_AVR
#include <sys/cdefs.h>

/* The maximum number of varargs allowed in a __libc_message format string */
#define LIBC_MESSAGE_MAX_ARGS 4

_Noreturn void __libc_message_impl(const char *__fnt, ...) attribute_hidden
	__attribute__((__format__(__printf__, 1, 2)));

#define __libc_message0(fmt) \
	__libc_message_impl(fmt)
#define __libc_message1(fmt, a1) \
	__libc_message_impl(fmt, a1)
#define __libc_message2(fmt, a1, a2) \
	__libc_message_impl(fmt, a1, a2)
#define __libc_message3(fmt, a1, a2, a3) \
	__libc_message_impl(fmt, a1, a2, a3)
#define __libc_message4(fmt, a1, a2, a3, a4) \
	__libc_message_impl(fmt, a1, a2, a3, a4)

#define __libc_message_concat_x(a, b) a##b
#define __libc_message_concat(a, b) __libc_message_concat_x(a, b)

#define __libc_message_nargs_x(a0, a1, a2, a3, a4, a5, a6, ...) a6
#define __libc_message_nargs(b, ...) \
	__libc_message_nargs_x(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0, )
#define __libc_message_disp(b, ...) \
	__libc_message_concat(b, __libc_message_nargs(__VA_ARGS__))(__VA_ARGS__)
#define __libc_message(...) \
	__libc_message_disp(__libc_message, __VA_ARGS__)
#endif
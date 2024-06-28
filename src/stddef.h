#pragma once
#ifdef ARDUINO_ARCH_AVR
#define __BEOS__
#endif
// 不定义就没有wint_t，但定义了就没有其它定义，所以只能先定义再取消定义，包含两次stddef
#define __need_wint_t
#include_next <stddef.h>
#undef __need_wint_t
#undef __need_size_t
#undef __need_wchar_t
#undef __need_NULL
#include_next <stddef.h>
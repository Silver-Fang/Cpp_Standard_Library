#pragma once
// 此头文件用于标识本库已被包含，并提示编译器应当编译本库中的源文件。请按需包含具体的功能头文件。

//Arduino不支持#include_next，只能用这种魔法实现包含官方标准库
#ifdef ARDUINO_ARCH_AVR
#define _CSL_Official(Header) <Arduino.h/../../arduino/Header>
#define _CSL_OfficialC(Header) <alloca.h/../../include/Header>
#else
#define _CSL_Official(Header) <c++/__GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__/Header>
#define _CSL_OfficialC(Header) <_ansi.h/../../include/Header>
#define _CSL_OfficialB(Header) <bits/basic_file.h/../../Header>
#endif
#define __GLIBC__ 2
#define __GLIBC_MINOR__ 39
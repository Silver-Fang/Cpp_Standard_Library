#pragma once
#ifdef ARDUINO_ARCH_AVR
#ifndef _LANGINFO_H

#include <locale_avr/langinfo.h>

#endif

#else
#include_next <langinfo.h>
#endif
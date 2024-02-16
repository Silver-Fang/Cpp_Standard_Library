#ifdef ARDUINO_ARCH_AVR
#endif
#ifdef ARDUINO_ARCH_SAM
#include "yvals.h"
#include "yvals_core.h"
#include "serstream"
#include "istream"
#include <Arduino.h>
#endif
_STD_BEGIN

__PURE_APPDOMAIN_GLOBAL static serialbuf fin(Serial);

#if defined(_M_CEE_PURE)
__PURE_APPDOMAIN_GLOBAL extern istream cin(&fin);

#else  // ^^^ defined(_M_CEE_PURE) / !defined(_M_CEE_PURE) vvv
__PURE_APPDOMAIN_GLOBAL extern _CRTDATA2_IMPORT istream cin(&fin);
#endif // ^^^ !defined(_M_CEE_PURE) ^^^
// 25
_STD_END
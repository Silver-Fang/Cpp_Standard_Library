#pragma once
#include_next <unistd.h>
#ifdef ARDUINO_ARCH_AVR
#include <features.h>
// 208
/* Standard file descriptors.  */
#define STDIN_FILENO 0	/* Standard input.  */
#define STDOUT_FILENO 1 /* Standard output.  */
#define STDERR_FILENO 2 /* Standard error output.  */
// 223
// 1128
#ifdef __USE_GNU

/* Evaluate EXPRESSION, and repeat as long as it returns -1 with `errno'
   set to EINTR.  */

#define TEMP_FAILURE_RETRY(expression) \
	(__extension__({ long int __result;						      \
       do __result = (long int) (expression);				      \
       while (__result == -1L && errno == EINTR);			      \
       __result; }))
#endif /* __USE_GNU */
// 1146
#endif
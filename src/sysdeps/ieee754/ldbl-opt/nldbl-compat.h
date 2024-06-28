/* Prototypes for compatibility double == long double entry points.
   Copyright (C) 2006-2024 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#ifndef __NLDBL_COMPAT_H
#define __NLDBL_COMPAT_H	1

/* Ensure calls to libm functions from libnldbl_nonshared.a call
   public names, not libm-internal names.  */
#define NO_MATH_REDIRECT

/* Avoid long double prototypes.  */
#define __NO_LONG_DOUBLE_MATH	1
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <printf.h>
#include <wchar.h>
#include <math.h>


/* Declare the __nldbl_NAME function the wrappers call that's in libc.so.  */
#define NLDBL_DECL(name) extern __typeof (name) __nldbl_##name

NLDBL_DECL (vfscanf);
NLDBL_DECL (fprintf);
NLDBL_DECL (vfprintf);
NLDBL_DECL (vsnprintf);
NLDBL_DECL (vsprintf);
NLDBL_DECL (printf_size);

/* These don't use __typeof because they were not declared by the headers,
   since we don't compile with _FORTIFY_SOURCE.  */
extern int __nldbl___vfprintf_chk (FILE *__restrict, int,
				   const char *__restrict, __gnuc_va_list);
extern int __nldbl___vfwprintf_chk (FILE *__restrict, int,
				    const wchar_t *__restrict, __gnuc_va_list);
extern int __nldbl___vsprintf_chk (char *__restrict, int, size_t,
				   const char *__restrict, __gnuc_va_list)
  __THROW;
extern int __nldbl___vsnprintf_chk (char *__restrict, size_t, int, size_t,
				    const char *__restrict, __gnuc_va_list)
  __THROW;
extern int __nldbl___vswprintf_chk (wchar_t *__restrict, size_t, int, size_t,
				    const wchar_t *__restrict, __gnuc_va_list)
  __THROW;
extern int __nldbl___vasprintf_chk (char **, int, const char *, __gnuc_va_list)
  __THROW;
extern int __nldbl___vdprintf_chk (int, int, const char *, __gnuc_va_list);
extern void __nldbl___vsyslog_chk (int, int, const char *, va_list);

/* The original declarations of these were hidden by the including
   file.  */
extern double __nldbl_daddl (double, double) __THROW;
extern double __nldbl_ddivl (double, double) __THROW;
extern double __nldbl_dmull (double, double) __THROW;
extern double __nldbl_dsubl (double, double) __THROW;

#endif /* __NLDBL_COMPAT_H */
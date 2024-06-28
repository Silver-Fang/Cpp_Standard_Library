#ifdef ARDUINO_ARCH_AVR
/* Support for dynamic linking code in static libc.
   Copyright (C) 1996-2024 Free Software Foundation, Inc.
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

/* This file defines some things that for the dynamic linker are defined in
   rtld.c and dl-sysdep.c in ways appropriate to bootstrap dynamic linking.  */

#include <string.h>
/* Mark symbols hidden in static PIE for early self relocation to work.
   Note: string.h may have ifuncs which cannot be hidden on i686.  */
#if BUILD_PIE_DEFAULT
# pragma GCC visibility push(hidden)
#endif
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdint.h>
#include <ldsodefs.h>
#include <array_length.h>

extern char *__progname;
char **_dl_argv = &__progname;	/* This is checked for some error messages.  */

/* Name of the architecture.  */
const char *_dl_platform;
size_t _dl_platformlen;

int _dl_debug_mask;
int _dl_lazy;
int _dl_dynamic_weak;

/* If nonzero print warnings about problematic situations.  */
int _dl_verbose;

/* Names of shared object for which the RUNPATHs and RPATHs should be
   ignored.  */
const char *_dl_inhibit_rpath;

/* The map for the object we will profile.  */
struct link_map *_dl_profile_map;

/* This is the address of the last stack address ever used.  */
void *__libc_stack_end;

/* Path where the binary is found.  */
const char *_dl_origin_path;

/* Nonzero if runtime lookup should not update the .got/.plt.  */
int _dl_bind_not;

/* Incremented whenever something may have been added to dl_loaded. */
unsigned long long _dl_load_adds = 1;

#ifndef HAVE_INLINED_SYSCALLS
/* Nonzero during startup.  */
int _dl_starting_up = 1;
#endif

/* Random data provided by the kernel.  */
void *_dl_random;

size_t _dl_pagesize = EXEC_PAGESIZE;

int _dl_inhibit_cache;

/* All known directories in sorted order.  */
struct r_search_path_elem *_dl_all_dirs;

/* All directories after startup.  */
struct r_search_path_elem *_dl_init_all_dirs;

/* The object to be initialized first.  */
struct link_map *_dl_initfirst;

/* Descriptor to write debug messages to.  */
int _dl_debug_fd = STDERR_FILENO;

enum dso_sort_algorithm _dl_dso_sort_algo;

#if PTHREAD_IN_LIBC
list_t _dl_stack_used;
list_t _dl_stack_user;
list_t _dl_stack_cache;
size_t _dl_stack_cache_actsize;
uintptr_t _dl_in_flight_stack;
int _dl_stack_cache_lock;
#else
/* If loading a shared object requires that we make the stack executable
   when it was not, we do it by calling this function.
   It returns an errno code or zero on success.  */
int (*_dl_make_stack_executable_hook) (void **) = _dl_make_stack_executable;
void (*_dl_init_static_tls) (struct link_map *) = &_dl_nothread_init_static_tls;
#endif
struct dl_scope_free_list *_dl_scope_free_list;

#ifdef NEED_DL_SYSINFO
/* Needed for improved syscall handling on at least x86/Linux.  NB: Don't
   initialize it here to avoid RELATIVE relocation in static PIE.  */
uintptr_t _dl_sysinfo;
#endif

#ifdef DL_SYSINFO_IMPLEMENTATION
DL_SYSINFO_IMPLEMENTATION
#endif
#endif
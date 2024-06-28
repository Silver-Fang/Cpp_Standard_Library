#ifndef _SYS_MMAN_H
#include <misc/sys/mman.h>

#ifndef _ISOMAC
/* Now define the internal interfaces.  */
extern int __munmap (void *__addr, size_t __len);
libc_hidden_proto (__munmap)
extern int __mprotect (void *__addr, size_t __len, int __prot);
libc_hidden_proto (__mprotect)

extern int __madvise (void *__addr, size_t __len, int __advice);
libc_hidden_proto (__madvise)

/* This one is Linux specific.  */
extern void *__mremap (void *__addr, size_t __old_len,
		       size_t __new_len, int __flags, ...);
libc_hidden_proto (__mremap)

# if IS_IN (rtld)
#  include <dl-mman.h>
# endif
#endif

#endif
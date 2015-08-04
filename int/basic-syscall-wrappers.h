#define WRAPPER1(typeret, name, type1) \
static inline typeret name(type1 p1) \
{ \
    return (typeret) syscall1(__NR_ ## name, (syscall_arg_t) p1); \
}
#define WRAPPER2(typeret, name, type1, type2) \
static inline typeret name(type1 p1, type2 p2) \
{ \
    return (typeret) syscall2(__NR_ ## name, (syscall_arg_t) p1, (syscall_arg_t) p2); \
}
#define WRAPPER3(typeret, name, type1, type2, type3) \
static inline typeret name(type1 p1, type2 p2, type3 p3) \
{ \
    return (typeret) syscall3(__NR_ ## name, (syscall_arg_t) p1, (syscall_arg_t) p2, (syscall_arg_t) p3); \
}


WRAPPER3(ssize_t, read, int, void*, size_t)
WRAPPER3(ssize_t, write, int, const void*, size_t)
WRAPPER1(int, close, int)
WRAPPER3(int, mprotect, void*, size_t, int)
WRAPPER3(int, sigaction, int, struct sigaction *, struct sigaction *)
WRAPPER2(int, sigaltstack, stack_t *, stack_t *)
#undef WRAPPER1
#undef WRAPPER2
#undef WRAPPER3


static inline int open(const char *pathname, int flags, ...)
{
    mode_t mode = 0;
    if (((flags & O_CREAT) == O_CREAT) || ((flags & O_TMPFILE) == O_TMPFILE)) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, int); // Not mode_t, due to integer promotion
        va_end(ap);
    }

    return (int) syscall3(__NR_open, (syscall_arg_t) pathname, (syscall_arg_t) flags, (syscall_arg_t) mode);
}


/* "Cheat" with shorter assembly when possible.
 * Also, syscalls with six arguments are not actually implemented yet :) */
#ifdef __NR_mmap2
#define __NR_mmap_registers __NR_mmap2 /* The "modern" version. Takes offset in pages. */
#else
#define __NR_mmap_registers __NR_mmap
#endif
static inline void* mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    if ((fd == -1) || ((flags & MAP_ANONYMOUS) == MAP_ANONYMOUS))
        return (void*) syscall6_negone5_zero6(__NR_mmap_registers, (syscall_arg_t) addr, (syscall_arg_t) length, (syscall_arg_t) prot, (syscall_arg_t) flags);

    // TODO: verify the offset adjustment is correct
#ifdef __NR_mmap2
    offset /= PAGE_SIZE;
#endif
    return (void*) syscall6(__NR_mmap_registers, (syscall_arg_t) addr, (syscall_arg_t) length, (syscall_arg_t) prot, (syscall_arg_t) flags, (syscall_arg_t) fd, (syscall_arg_t) offset);
}

#define MAP_FAILED ((void*) -1)

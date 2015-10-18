#define WRAPPER0(typeret, name) \
static inline typeret name() \
{ \
    return (typeret) syscall0(__NR_ ## name); \
}
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
#define WRAPPER4(typeret, name, type1, type2, type3, type4) \
static inline typeret name(type1 p1, type2 p2, type3 p3, type4 p4) \
{ \
    return (typeret) syscall4(__NR_ ## name, (syscall_arg_t) p1, (syscall_arg_t) p2, (syscall_arg_t) p3, (syscall_arg_t) p4); \
}
#define WRAPPER5(typeret, name, type1, type2, type3, type4, type5) \
static inline typeret name(type1 p1, type2 p2, type3 p3, type4 p4, type5 p5) \
{ \
    return (typeret) syscall5(__NR_ ## name, (syscall_arg_t) p1, (syscall_arg_t) p2, (syscall_arg_t) p3, (syscall_arg_t) p4, (syscall_arg_t) p5); \
}


WRAPPER3(ssize_t, read, int, void*, size_t)
WRAPPER3(ssize_t, write, int, const void*, size_t)
// open: see below
WRAPPER1(int, close, int)
WRAPPER2(int, stat, const char*, struct stat*)
WRAPPER2(int, fstat, int, struct stat*)
WRAPPER2(int, lstat, const char*, struct stat*)
WRAPPER3(int, poll, struct pollfd*, nfds_t, int)
WRAPPER3(off_t, lseek, int, off_t, int)
WRAPPER3(int, mprotect, void*, size_t, int)
WRAPPER2(int, munmap, void*, size_t)
WRAPPER1(int, brk, void*)
WRAPPER2(int, access, const char*, int)
// pipe
WRAPPER3(int, msync, void*, size_t, int)
WRAPPER1(int, dup, int)
WRAPPER2(int, dup2, int, int)
WRAPPER1(unsigned int, alarm, unsigned int)
WRAPPER0(pid_t, getpid)
WRAPPER4(ssize_t, sendfile, int, int, off_t *, size_t)
WRAPPER0(pid_t, fork)
WRAPPER3(int, execve, const char *, char *const *const, char *const *const)
WRAPPER4(pid_t, wait4, pid_t, int*, int, struct rusage *)
WRAPPER2(int, kill, pid_t, int)
WRAPPER1(int, fsync, int)
WRAPPER1(int, fdatasync, int)
WRAPPER2(int, truncate, char*, off_t)
WRAPPER2(int, ftruncate, int, off_t)
WRAPPER1(int, chdir, const char*)
WRAPPER2(int, rename, const char *, const char *)
WRAPPER2(int, mkdir, const char *, mode_t)
WRAPPER1(int, rmdir, const char *)
WRAPPER2(int, creat, const char*, mode_t)
WRAPPER2(int, link, const char*, const char*)
WRAPPER1(int, unlink, const char*)
WRAPPER2(int, symlink, const char*, const char*)
WRAPPER3(ssize_t, readlink, const char*, char*, size_t)
WRAPPER2(int, chmod, const char*, mode_t)
WRAPPER2(int, fchmod, int, mode_t)
WRAPPER3(int, chown, const char*, uid_t, gid_t)
WRAPPER3(int, fchown, int, uid_t, gid_t)
WRAPPER3(int, lchown, const char*, uid_t, gid_t)
WRAPPER1(mode_t, umask, mode_t)
WRAPPER2(int, getrlimit, int, struct rlimit*)
WRAPPER2(int, getrusage, int, struct rlimit*)
WRAPPER1(int, sysinfo, struct sysinfo*)
WRAPPER0(uid_t, getuid)
WRAPPER0(gid_t, getgid)
WRAPPER1(int, setuid, uid_t)
WRAPPER1(int, setgid, gid_t)
WRAPPER0(uid_t, geteuid)
WRAPPER0(gid_t, getegid)
WRAPPER2(int, setpgid, pid_t, pid_t)
WRAPPER0(int, getppid)
WRAPPER2(int, setreuid, uid_t, uid_t)
WRAPPER2(int, setregid, gid_t, gid_t)
WRAPPER2(int, getgroups, int, gid_t*)
WRAPPER2(int, setgroups, size_t, const gid_t*)
WRAPPER3(int, setresuid, uid_t, uid_t, uid_t)
WRAPPER3(int, getresuid, uid_t*, uid_t*, uid_t*)
WRAPPER3(int, setresgid, gid_t, gid_t, gid_t)
WRAPPER3(int, getresgid, gid_t*, gid_t*, gid_t*)
WRAPPER1(int, setfsuid, uid_t)
WRAPPER1(int, setfsgid, uid_t)
WRAPPER1(int, personality, unsigned long)
WRAPPER5(int, prctl, int, unsigned long, unsigned long, unsigned long, unsigned long)
WRAPPER2(int, setrlimit, int, const struct rlimit*)
WRAPPER5(int, remap_file_pages, void*, size_t, int, size_t, int)
WRAPPER2(int, clock_getres, clockid_t, struct timespec*)
WRAPPER4(int, clock_nanosleep, clockid_t, int, const struct timespec*, struct timespec *);
WRAPPER4(ssize_t, tee, int, int, size_t, unsigned int)
WRAPPER3(int, dup3, int, int, int)
WRAPPER2(int, sigaltstack, stack_t *, stack_t *)
#undef WRAPPER0
#undef WRAPPER1
#undef WRAPPER2
#undef WRAPPER3
#undef WRAPPER4
#undef WRAPPER5


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

static inline int pipe(int pipefd[2])
{
    return (int) syscall1(__NR_pipe, (syscall_arg_t) pipefd);
}
static inline int pipe2(int pipefd[2], int flags)
{
    return (int) syscall2(__NR_pipe2, (syscall_arg_t) pipefd, (syscall_arg_t) flags);
}


static inline int rt_sigaction(int signum, struct sigaction *act, struct sigaction *oldact); // They use each other
static inline int sigaction(int signum, struct sigaction *act, struct sigaction *oldact)
{
#ifdef __NR_sigaction
    return (int) syscall3(__NR_sigaction, (syscall_arg_t) signum, (syscall_arg_t) act, (syscall_arg_t) oldact);
#else
    return rt_sigaction(signum, act, oldact);
#endif
}

static inline int rt_sigaction(int signum, struct sigaction *act, struct sigaction *oldact)
{
#ifdef __i386__
    // There seems to be weird stuff going on here
    // i386 user signal.h mentions it "must cater to libcs that poke about in kernel headers", and seems to simply have the wrong structure defined
    // Let's use the legacy one, seems to work fine
    return sigaction(signum, act, oldact);
#else
    return (int) syscall3(__NR_rt_sigaction, (syscall_arg_t) signum, (syscall_arg_t) act, (syscall_arg_t) oldact);
#endif
}

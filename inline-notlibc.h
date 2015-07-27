/* Copyright 2013-2015 Jacopo Corbetta <jacopo.corbetta@gmail.com> */

/* Fully static/inline libc-like library for Linux + GCC.
 *
 * No linker, no GOT, no global variables, no constructors, no nothing.
 * Use it when you don't want to depend on _anything_ at run time.
 * Examples include entry point interception and static binary patching.
 * (Or even very lazy shellcode writing :D)
 *
 * Obviously, it does not cover everything that the real libc does, and it's not fully
 * compatible. It's meant to help in creating small "packages" that can be simply loaded
 * anywhere in memory and jumped to, and do so in a familiar way. C is already 95% there:
 * this thing just tries to add the missing 5%.
 *
 * Note that there is no errno, just return values. Use VS() to simulate that.
 * For instance, 'if (xxx() == -1) err(109, "Failed xxx()")' should become 'VS(xxx())'.
 *
 * To use it, include this file instead of libc and compile with -nostdlib
 * and -nostdinc.
 * It should not be necessary to use -ffreestanding (it would also disable GCC builtins).
 * You will need your target linux-libc-dev (the uapi/xxx headers).
 */

#pragma once

#define NULL ((void*)0)

/* Stuff we get straight from gcc */
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stddef.h>

/* Stuff we get from Linux headers */
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/time.h>
typedef __u32 __kernel_dev_t;
typedef __kernel_fd_set         fd_set;
typedef __kernel_dev_t          dev_t;
typedef __kernel_ino_t          ino_t;
typedef __kernel_mode_t         mode_t;
typedef unsigned short          umode_t;
typedef __u32                   nlink_t;
typedef __kernel_pid_t          pid_t;
typedef __kernel_daddr_t        daddr_t;
typedef __kernel_key_t          key_t;
typedef __kernel_suseconds_t    suseconds_t;
typedef __kernel_timer_t        timer_t;
typedef __kernel_clockid_t      clockid_t;
typedef __kernel_mqd_t          mqd_t;
typedef __kernel_uid32_t        uid_t;
typedef __kernel_gid32_t        gid_t;
typedef __kernel_uid16_t        uid16_t;
typedef __kernel_gid16_t        gid16_t;

/* Some defines are still missing.
 * Shameless copy from include/linux/kernel.h */
typedef __kernel_off_t          off_t;
typedef __kernel_ssize_t        ssize_t;
typedef __kernel_time_t         time_t;
typedef __kernel_clock_t        clock_t;
#define USHRT_MAX       ((uint16_t)(~0U))
#define SHRT_MAX        ((int16_t)(USHRT_MAX>>1))
#define SHRT_MIN        ((int16_t)(-SHRT_MAX - 1))
#define INT_MAX         ((int)(~0U>>1))
#define INT_MIN         (-INT_MAX - 1)
#define UINT_MAX        (~0U)
#define LONG_MAX        ((long)(~0UL>>1))
#define LONG_MIN        (-LONG_MAX - 1)
#define ULONG_MAX       (~0UL)
#define LLONG_MAX       ((long long)(~0ULL>>1))
#define LLONG_MIN       (-LLONG_MAX - 1)
#define ULLONG_MAX      (~0ULL)

/* Still missing. This is very annoying. */
#define SSIZE_MAX        ((ssize_t)(SIZE_MAX>>1))
#define SSIZE_MIN        ((ssize_t)(-SIZE_MAX - 1))


/* Linux headers useful for syscalls & co. */
#include <linux/fcntl.h>
#include <linux/mman.h>
#include <linux/prctl.h>
#include <linux/resource.h>
#include <linux/signal.h>
#include <linux/unistd.h> /* __NR_syscall_name */
#include <asm/sigcontext.h>
#include <asm/ucontext.h>


/* Preprocessor utils */
#define MACRO_VAL_TO_STR(x) __STRING(x)
#define S__LINE__ MACRO_VAL_TO_STR(__LINE__)

/* Shorten some GCC attributes/pragmas */
#define FORCE_INLINE inline __attribute__((__always_inline__))
#define OK_UNUSED __attribute__((__unused__))
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define START_PACKED_STRUCTURES _Pragma("pack(push, 1)")
#define END_PACKED_STRUCTURES _Pragma("pack(pop)")
#define static_assert _Static_assert


/* 0. Basic stuff that does not involve syscalls (memcpy, strlen, ...) */
#include "int/string.h"


/* 1. Raw Linux syscall interface. Includes an always-inlined exit_group(). */
static inline bool retcode_ok(long r) { return (r >= 0) || (r < -125); }
#define TEMP_FAILURE_RETRY(x) ({ long r; do { r = (x); } while (r == EINTR); r; })
#include "int/syscall.h"


/* 2. libc-like syscall wrappers.
 *
 * Note that (as in libc's case) wrappers may still return EINTR.
 * Remember to use TEMP_FAILURE_RETRY to "auto-fix" that.
 */
#include "int/basic-syscall-wrappers.h"


/* 3. Very basic IO to stderr. NOT CHECKED FOR SUCCESS. */
#include "int/basic-io.h"

/* Now that we can print stuff, we can define the basic assert-like macro. */
#define V(x)  if (unlikely(!(x))) do { \
    puts("FAILED: " __FILE__ ":" S__LINE__ " it's not " #x "\n"); \
    exit_group(109); \
} while (0)
#define VS(x) ({ \
        long r = (x); \
        if (unlikely(!retcode_ok(r))) { \
            puts("FAILED: " __FILE__ ":" S__LINE__ " " #x " failed\n"); \
            exit_group(109); \
        } \
        r; \
    })
#ifdef NDEBUG
# define assert(x) ((void) 0)
#else
#define assert(x) V(x)
#endif


/* 4. Fancy IO (printf, etc.) */
#include "int/fancy-io.h"

/* Now that we have printf, we can define the fancy version of VS */
#undef VS
#define VS(x) ({ \
        long r = (x); \
        if (unlikely(!retcode_ok(r))) { \
            printf("FAILED: " __FILE__ ":%d %s, %s failed " \
                   "(returned %ld, errno would be %ld)\n", \
                   __LINE__, __func__, #x, r, -r); \
            exit_group(109); \
        } \
        r; \
    })


/* 5. Utility wrappers that mask away annoying behavior (EINTR, partial read/writes/...). */
#include "int/utility-syscall-wrappers.h"

/* Re #define functions as their builtins. Should force gcc to use them. */
#include "int/builtin-functions.h"

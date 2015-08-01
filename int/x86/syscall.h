#pragma once

#define PAGE_SIZE 4096u

/* Uses the int80 interface by default.
 * Tries to keep the code very short, very close to what one would write
 * if doing this in assembly.
 *
 * Note that other methods exist, including:
 *    - syscall on AMD CPUs
 *    - sysenter on Intel CPUs (inconvenient return system)
 *    - the VDSO, wrapping the above
 *    - glibc's wrapper in gs:0x10
 */

typedef uint32_t syscall_arg_t;

#define syscall0 int80_syscall0
#define syscall1 int80_syscall1
#define syscall2 int80_syscall2
#define syscall3 int80_syscall3
#define syscall4 int80_syscall4
#define syscall5 int80_syscall5
#define syscall6 int80_syscall6

/* Allows shorter calls for basic prctl and mmap use cases */
#define syscall5_zero5 int80_syscall5_zero5
#define syscall6_negone5_zero6 int80_syscall6_negone5_zero6


#include "syscall_int80.h"
#include "syscall_alt.h"
#include "syscall_exit.h"

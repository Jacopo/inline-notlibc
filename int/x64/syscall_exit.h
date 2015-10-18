#pragma once

/* These versions do not worry about clobbering. */

static FORCE_INLINE __attribute__((__noreturn__))
void exit_group(int exit_code)
{
    uint64_t p1 = (uint64_t) exit_code;
    asm volatile(
        "syscall"
        : : "a"(__NR_exit_group), "D"(p1) : "rcx", "r11", "memory");
    __builtin_unreachable();
}

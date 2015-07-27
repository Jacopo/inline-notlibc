#pragma once

/* These versions do not worry about clobbering. */

#define exit_group int80_exit_group

static FORCE_INLINE __attribute__((__noreturn__))
void int80_exit_group(int exit_code)
{
#if defined(__i386__) && defined(__PIC__)
    uint32_t for_ebx = (uint32_t) exit_code;
    asm volatile(
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "int $0x80"                  "\n\t"\
        : : "a"(__NR_exit_group), [for_ebx]"ir"(for_ebx) : "memory");
#else
    asm volatile("int $0x80" : : "a"(__NR_exit_group), "b"(exit_code) : "memory");
#endif
    __builtin_unreachable();
}

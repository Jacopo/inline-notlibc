#pragma once

/* FIXME: There seems to be something wrong with the clobbering, marking the untested versions noinline */
#define DIRECT_SYSCALL_ATTRS  static __attribute__((__warn_unused_result__, __unused__, __noinline__))

static FORCE_INLINE long int80_syscall0(uint32_t n)
{
    uint32_t eax = n;
    __asm__ __volatile__("int $0x80" : "=a"(eax) : "0"(eax) : "memory");
    return eax;
}

DIRECT_SYSCALL_ATTRS long int80_syscall1(uint32_t n, uint32_t p1)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx) : "memory");
#else
    __asm__ __volatile__("int $0x80" : "=a"(eax) : "0"(eax), "b"(p1) : "memory");
#endif
    return eax;
}

DIRECT_SYSCALL_ATTRS long int80_syscall2(uint32_t n, uint32_t p1, uint32_t p2)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx), "c"(p2) : "memory");
#else
    __asm__ __volatile__("int $0x80" : "=a"(eax) : "0"(eax), "b"(p1), "c"(p2) : "memory");
#endif
    return eax;
}

DIRECT_SYSCALL_ATTRS long int80_syscall3(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx), "c"(p2), "d"(p3) : "memory");
#else
    __asm__ __volatile__("int $0x80" : "=a"(eax) : "0"(eax), "b"(p1), "c"(p2), "d"(p3) : "memory");
#endif
    return eax;
}

DIRECT_SYSCALL_ATTRS long int80_syscall5_zero5(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    /* Also, doesn't find enough registers if EDI is used directly */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "push %%edi"                 "\n\t"\
        "xor %%edi, %%edi" /* edi=0 */ "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%edi"                  "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx), "c"(p2), "d"(p3), "S"(p4) : "memory");
#else
    __asm__ __volatile__(
        "push %%ebp"                 "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%ebp"                  "\n\t"\
    : "=a"(eax) : "0"(eax), "b"(p1), "c"(p2), "d"(p3), "S"(p4), "D"(0) : "memory");
#endif
    return eax;
}

DIRECT_SYSCALL_ATTRS long int80_syscall6_negone5_zero6(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    /* Also, doesn't find enough registers if EDI is used directly */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "push %%ebp"                 "\n\t"\
        "push %%edi"                 "\n\t"\
        "xor %%ebp, %%ebp" /* ebp=0 */ "\n\t"\
        "mov $-1, %%edi" /* edi=-1 */ "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%edi"                  "\n\t"\
        "pop %%ebp"                  "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx), "c"(p2), "d"(p3), "S"(p4) : "memory");
#else
    __asm__ __volatile__(
        "push %%ebp"                 "\n\t"\
        "xor %%ebp, %%ebp" /* ebp=0 */ "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%ebp"                  "\n\t"\
    : "=a"(eax) : "0"(eax), "b"(p1), "c"(p2), "d"(p3), "S"(p4), "D"(-1) : "memory");
#endif
    return eax;
}




/* Annoying six-argument form */

/* **********  THIS MAY BE WRONG  ********* */

DIRECT_SYSCALL_ATTRS long int80_syscall6(uint32_t n, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5, uint32_t p6)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    /* Also, doesn't find enough registers if EDI is used directly */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "push %%ebp"                 "\n\t"\
        "push %%edi"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "mov %[p6], %%edi"           "\n\t"\
        "mov %[p5], %%ebp"           "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%edi"                  "\n\t"\
        "pop %%ebp"                  "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx), "c"(p2), "d"(p3), "S"(p4), [p5]"im"(p5), [p6]"im"(p6) : "memory");
#else
    __asm__ __volatile__(
        "push %%ebp"                 "\n\t"\
        "mov %[p6], %%ebp"      "\n\t"\
        "int $0x80"                  "\n\t"\
        "pop %%ebp"                  "\n\t"\
    : "=a"(eax) : "0"(eax), "b"(p1), "c"(p2), "d"(p3), "S"(p4), "im"(p5), [p6]"D"(p6) : "memory");
#endif
    return eax;
}


#define int80_syscall5(n, p1, p2, p3, p4, p5) int80_syscall6(n, p1, p2, p3, p4, p5, 0)

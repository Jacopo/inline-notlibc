/* Alternative syscall methods: */

DIRECT_SYSCALL_ATTRS long gs10_syscall1(uint32_t n, uint32_t p1)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "call *%%gs:0x10"            "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx) : "memory");
#else
    __asm__ __volatile__("call *%%gs:0x10" : "=a"(eax) : "0"(eax), "b"(p1) : "cc","memory");
#endif
    return eax;
}


#ifdef ENABLE_MIMIC_VSYSCALL

/* Calling my bare_mimic_vsyscall should basically be equivalent to calling gs:0x10 when it uses sysenter */
extern __attribute__((__warn_unused_result__)) int bare_mimic_vsyscall();

DIRECT_SYSCALL_ATTRS long mimic_vsyscall1(uint32_t n, uint32_t p1)
{
    uint32_t eax = n;
#if defined(__i386__) && defined(__PIC__)
    /* Preserve EBX: used for PIC */
    uint32_t for_ebx = p1;
    __asm__ __volatile__(
        "push %%ebx"                 "\n\t"\
        "mov %[for_ebx],%%ebx"       "\n\t"\
        "call bare_mimic_vsyscall"   "\n\t"\
        "pop %%ebx"                  "\n\t"\
        : "=a"(eax) : "0"(eax), [for_ebx]"ir"(for_ebx) : "memory");
#else
    __asm__ __volatile__("call bare_mimic_vsyscall" : "=a"(eax) : "0"(eax), "b"(p1) : "cc","memory");
#endif
    return eax;
}

#endif /* ENABLE_MIMIC_VSYSCALL */

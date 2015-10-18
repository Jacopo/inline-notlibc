#pragma once
typedef uint64_t syscall_arg_t;
#define PAGE_SIZE 4096u

#include "syscall_exit.h"


/* FIXME: Same caution as for 32-bit syscalls */
#define DIRECT_SYSCALL_ATTRS  static __attribute__((__warn_unused_result__, __unused__, __noinline__))

DIRECT_SYSCALL_ATTRS uint64_t syscall0(uint64_t n)
{
    uint64_t ret;
    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
    return ret;
}

DIRECT_SYSCALL_ATTRS uint64_t syscall1(uint64_t n, uint64_t p1)
{
    uint64_t ret;
    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n), "D"(p1) : "rcx", "r11", "memory");
    return ret;
}

DIRECT_SYSCALL_ATTRS uint64_t syscall2(uint64_t n, uint64_t p1, uint64_t p2)
{
    uint64_t ret;
    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n), "D"(p1), "S"(p2)
        : "rcx", "r11", "memory");
    return ret;
}

DIRECT_SYSCALL_ATTRS uint64_t syscall3(uint64_t n, uint64_t p1, uint64_t p2, uint64_t p3)
{
    uint64_t ret;
    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n), "D"(p1), "S"(p2), "d"(p3)
        : "rcx", "r11", "memory");
    return ret;
}

DIRECT_SYSCALL_ATTRS uint64_t syscall4(uint64_t n, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4)
{
    uint64_t ret;

    register uint64_t r10 asm("r10") = p4;

    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n), "D"(p1), "S"(p2), "d"(p3),
          "r"(r10)
        : "rcx", "r11", "memory");
    return ret;
}

DIRECT_SYSCALL_ATTRS uint64_t syscall5(uint64_t n, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5)
{
    uint64_t ret;

    register uint64_t r10 asm("r10") = p4;
    register uint64_t r8 asm("r8") = p5;

    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n), "D"(p1), "S"(p2), "d"(p3),
          "r"(r10), "r"(r8)
        : "rcx", "r11", "memory");
    return ret;
}

DIRECT_SYSCALL_ATTRS uint64_t syscall6(uint64_t n, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t p5, uint64_t p6)
{
    uint64_t ret;

    register uint64_t r10 asm("r10") = p4;
    register uint64_t r8 asm("r8") = p5;
    register uint64_t r9 asm("r9") = p6;

    asm volatile(
        "syscall"
        : "=a"(ret) : "a"(n), "D"(p1), "S"(p2), "d"(p3),
          "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory");
    return ret;
}


#define syscall5_zero5(n, p1, p2, p3, p4) syscall5(n, p1, p2, p3, p4, 0)
#define syscall6_negone5_zero6(n, p1, p2, p3, p4) syscall6(n, p1, p2, p3, p4, -1, 0)

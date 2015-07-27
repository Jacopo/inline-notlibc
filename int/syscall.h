#pragma once

/* Should define syscallN() with the correct argument type */

#if defined(__i386__)
#   include "x86/syscall.h"
#elif defined(__x86_64__) && defined(__ILP32__)
#   error "TODO: x32 (Note: be mindful of the __X32_SYSCALL_BIT)"
#elif defined(__x86_64__)
#   include "x64/syscall.h"
#else
#   error "TODO: that's a fancy architecture you have there :)"
#endif

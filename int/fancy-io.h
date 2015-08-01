#include <linux/fcntl.h>

static inline __attribute__((__nonnull__)) void printfile(const char *pathname)
{
    int fd = VS(open(pathname, O_RDONLY));
    int r;
    while (true) {
        unsigned char c;
        r = read(fd, &c, 1);
        if (r != 1)
            break;
        putchar(c);
    }
    VS(r == 0);
    VS(close(fd));
}


#define stderr 0xEAEAEAEA
#define fprintf(target, args...) ({ static_assert(target == stderr, "fprintf must use stderr"); printf(args); })

#define errx(code, args...) ({ printf(args); exit_group(code); })

static inline __attribute__((__format__(__printf__, 1, 2), __nonnull__(1)))
int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    V(fmt != NULL);
    size_t fmt_len = strlen(fmt);
    V(fmt_len < INT_MAX); /* Well below UINTMAX */

    unsigned int i, i_normal = 0;
    for (i = 0; i < fmt_len; ++i) {
        if (fmt[i] == '%') {
            /* Write the "normal" stuff before the % */
            assert(i_normal <= i);
            printlen(&fmt[i_normal], i - i_normal);

            /* Let's look at what comes after % */
            ++i;

            /* Flags */
            enum { NO_FLAGS=0, ALTFORM=1, ZEROPAD=2 }; int flags = 0;
            if (fmt[i] == '#') { flags |= ALTFORM; ++i; }
            if (fmt[i] == '0') { flags |= ZEROPAD; ++i; }

            /* Field width */
            unsigned int field_width = 0;
            if ((fmt[i] >= '1') && (fmt[i] <= '9'))
                field_width = fmt[i++] - '0';

            /* Length modifier */
            enum { NO_LM=0, LM_CHAR, LM_SHORT, LM_LONG, LM_LONGLONG }; int lenmod = 0;
            if (fmt[i] == 'h') {
                lenmod = LM_SHORT; ++i;
                if (fmt[i] == 'h') { lenmod = LM_CHAR; ++i; }
            } else if (fmt[i] == 'l') {
                lenmod = LM_LONG; ++i;
                if (fmt[i] == 'l') { lenmod = LM_LONGLONG; ++i; }
            }

            /* Conversion specifier, actual print */
            char spec; /* (allows changing it for %p) */
            switch (spec = fmt[i]) {
            case 'c':
            case '%':
                {
                    unsigned char c = (spec == '%') ? '%' : ((unsigned char) va_arg(ap, int));
                    V((flags == 0) && (field_width == 0) && (lenmod == 0));
                    putchar(c);
                }
                break;

            case 's':
                {
                    const char *s = va_arg(ap, char*);
                    V((flags == 0) && (field_width == 0) && (lenmod == 0));
                    print(s);
                }
                break;

            case 'p':
                V((flags == 0) && (field_width == 0) && (lenmod == 0));
                /* I prefer the 0x%08x form */
                static_assert(sizeof(void*) == sizeof(unsigned int), "TODO: 64-bit");
                putchar('0'); putchar('x');
                flags = ZEROPAD; field_width = 8; spec = 'x';
                /* Fall through ... */

            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
                {
                    /* 1. Get n = abs(the_number) */
                    unsigned long long n; /* Should be enough for anything */
                    int print_signed = !!((spec == 'd') || (spec == 'i'));
                    int negative = 0;
                    if (lenmod == LM_LONGLONG) {
                        signed long long tmp = va_arg(ap, long long);
                        if (print_signed && (tmp < 0)) { negative = 1; n = -tmp; } else { n = tmp; }
                    } else if (lenmod == LM_LONG) {
                        signed long tmp = va_arg(ap, long);
                        if (print_signed && (tmp < 0)) { negative = 1; n = -tmp; } else { n = tmp; }
                        n &= ULONG_MAX;
                    } else if (lenmod == LM_SHORT) {
                        signed short tmp = va_arg(ap, int); /* promoted when passing */
                        if (print_signed && (tmp < 0)) { negative = 1; n = -tmp; } else { n = tmp; }
                        n &= USHRT_MAX;
                    } else if (lenmod == LM_CHAR) {
                        signed char tmp = va_arg(ap, int); /* promoted when passing */
                        if (print_signed && (tmp < 0)) { negative = 1; n = -tmp; } else { n = tmp; }
                        n &= 0xFFu;
                    } else {
                        int tmp = va_arg(ap, int);
                        if (print_signed && (tmp < 0)) { negative = 1; n = -tmp; } else { n = tmp; }
                        n &= UINT_MAX;
                    }

                    /* 2. If requested, prefix with sign or 0x */
                    if (negative && print_signed) putchar('-');
                    if (flags & ALTFORM) {
                        V((spec == 'x') || (spec == 'X'));
                        if (n != 0) { putchar('0'); putchar(spec); }
                    }

                    /* 3. Prepare a string 0000...number */
                    char buf[] = "0000000000000000000000000000000000000000000000000000000";
                    int buf_i = sizeof(buf) - 1;
                    while (n != 0) {
                        --buf_i;
                        if ((spec == 'x') || (spec == 'X')) {
                            unsigned char c = (n % 16u);
                            if (c <= 9)
                                c += '0';
                            else c += (((spec == 'X') ? 'A' : 'a') - 10);
                            buf[buf_i] = c;
                            n /= 16u;
                        } else {
                            V(lenmod != LM_LONGLONG);
                            unsigned int nint = (unsigned int) n;
                            buf[buf_i] = '0' + (nint % 10u);
                            nint /= 10u;
                            n = nint;
                        }
                    }

                    /* 4. Print as many zeros as wanted before the number */
                    /*    Note: the real printf has much more complex rules */
                    int print_i = buf_i;  /* Default: print only the number */

                    if (buf[print_i] == '\0')
                        --print_i;        /* Print at least a '0' */
                    assert(strlen(&buf[print_i]) > 0);

                    if (field_width) {
                        V(flags & ZEROPAD);
                        assert(field_width <= 9);
                        print_i = sizeof(buf) - field_width - 1;
                        V(print_i <= buf_i); /* Would be shortening! */
                        assert(strlen(&buf[print_i]) == field_width);
                    }

                    print(&buf[print_i]);
                }
                break;

            default:
                print("I am not libc, I can't print your fancy stuff!\n");
                exit_group(-9);
                break;
            }

            /* The next normal part begins */
            i_normal = i+1;
        }
    }

    /* Write the last normal part */
    printlen(&fmt[i_normal], i - i_normal);

    va_end(ap);

    return 666; /* Completely fake return value. Doesn't actually keep the count. */
}

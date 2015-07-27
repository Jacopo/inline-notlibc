// NOTE: to keep the thing short, stderr writes are NOT CHECKED FOR SUCCESS in this version

#ifndef PRINT_FD
#define PRINT_FD 2
#endif

static inline int putchar(int ci)
{
    char c = (char) ci;
    write(PRINT_FD, &c, 1); // UNCHECKED
    return c;
}


static inline __attribute__((__nonnull__)) void printlen(const char *s, size_t len)
{
    write(PRINT_FD, s, len);
}

#define print(s) printlen(s, strlen(s))

static inline __attribute__((__nonnull__)) int puts(const char *s)
{
    print(s);
    putchar('\n');
    return 0;
}

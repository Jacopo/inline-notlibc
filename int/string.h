#pragma once

/* Could probably be made "smarter" with arch-specific code (rep scas & co.).
 * Restrict might also help.
 *
 * Note: these are just built-in helpers, GCC should do most simple cases itself.
 */

static inline __attribute__((__nonnull__, __warn_unused_result__, __pure__))
size_t strlen(const char *s)
{
    const char *p = s;
    while (*p != '\0')
        ++p;
    return p - s;
}

static inline __attribute__((__nonnull__))
void* memcpy(void *dest, const void *src, size_t len)
{
    char *d = (char *) dest;
    const char *s = (const char *) src;
    while (len--)
        *(d++) = *(s++);
    return dest;
}
static inline __attribute__((__nonnull__))
void* memmove(void *dest, const void *src, size_t len)
{
    char *d = (char *) dest;
    const char *s = (const char *) src;
    if (dest <= src)
        return memcpy(dest, src, len);
    d += (len - 1);
    s += (len - 1);
    while (len--)
        *(d--) = *(s--);
    return dest;
}

static inline __attribute__((__nonnull__))
void* memset(void *dest, int c, size_t len)
{
    char *d = (char *) dest;
    while (len--)
        *(d++) = (char) c;
    return dest;
}
static inline __attribute__((__nonnull__))
void bzero(void *dest, size_t len)
{
    memset(dest, 0, len);
}

static inline __attribute__((__nonnull__))
int memcmp(const void *s1, const void *s2, size_t len)
{
    const char *a = s1, *b = s2;
    while (len--) {
        int diff = *(a++) - *(b++);
        if (diff != 0)
            return diff;
    }
    return 0;
}

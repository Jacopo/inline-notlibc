#pragma once

__attribute__ ((__warn_unused_result__))  __attribute__ ((__nonnull__))
static inline ssize_t do_read_partial(int fd, uint8_t* buf, size_t len)
{
    V(len <= SSIZE_MAX);
    ssize_t r;
    uint8_t *rbuf = buf;
    do {
        VS(r = TEMP_FAILURE_RETRY(read(fd, rbuf, len)));
        if (r == 0)
            break;
        len -= r;
        rbuf += r;
    } while (len != 0);
    return rbuf - buf;
}

__attribute__ ((__warn_unused_result__))  __attribute__ ((__nonnull__))
static inline ssize_t do_write_partial(int fd, const uint8_t* buf, size_t len)
{
    V(len <= SSIZE_MAX);
    ssize_t w;
    const uint8_t *wbuf = buf;
    do {
        VS(w = TEMP_FAILURE_RETRY(write(fd, wbuf, len)));
        if (w == 0)
            break;
        len -= w;
        wbuf += w;
    } while (len != 0);
    return wbuf - buf;
}


__attribute__((__nonnull__)) 
static inline void do_read(int fd, uint8_t *buf, size_t len)
{
    V(do_read_partial(fd, buf, len) == (ssize_t) len);
}

__attribute__((__nonnull__)) 
static inline void do_write(int fd, const uint8_t *buf, size_t len)
{
    V(do_write_partial(fd, buf, len) == (ssize_t) len);
}

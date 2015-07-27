// Both libc's and linux's header conflict with a ton of other stuff!
// #include <signal.h>

static inline int sigaction(int sig, const struct sigaction *old, struct sigaction*)

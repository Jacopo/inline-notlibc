#include "inline-notlibc.h"

void _start()
{
    pid_t pid = getpid(); VS(pid);
    uid_t uid = getuid(); VS(uid);

    mode_t old_umask = umask(0022); VS(old_umask);

    VS(write(2, "Prova\n", 6));
    VS(write(2, &pid, sizeof(pid)));
    VS(write(2, "\n", 1));
    VS(write(2, &uid, sizeof(uid)));
    VS(write(2, "\n", 1));
    VS(write(2, &old_umask, sizeof(old_umask)));
    VS(write(2, "\n", 1));

    struct timespec request = { 1, 0 }, remain;
    int ret = clock_nanosleep(CLOCK_REALTIME, 0, &request, &remain);
    if (ret != -EINTR) VS(ret);

    VS(prctl(PR_SET_NAME, "prova_start", 0, 0, 0));
    char pnamebuf[16];
    VS(prctl(PR_GET_NAME, pnamebuf, 0, 0, 0));
    V(strncmp(pnamebuf, "prova_start", 16) == 0);

    VS(prctl(PR_GET_NO_NEW_PRIVS, 0, 0, 0, 0)); // Should check zeros
    VS(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)); // Should check zeros

    exit_group(5);
}

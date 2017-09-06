#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <unistd.h>

static void pr_times(clock_t wall_clock_tick, struct tms start_tms, struct tms end_tms);
static void exec_cmd(const char* cmd);
static void pr_exit(int status);

int main(int argc, char* argv[])
{
    int i;
    for (i = 1; i < argc; ++i)
        exec_cmd(argv[i]);
    return 0;
}

static void exec_cmd(const char* cmd)
{
    struct tms start_tms;
    struct tms end_tms;
    clock_t start, end;
    int status;
    printf("exec:%s\n", cmd);
    if ((start = times(&start_tms)) < 0) {
        perror("times error");
        exit(-1);
    }
    if ((status = system(cmd)) < 0) {
        printf("system error\n");
        exit(-1);
    }
    if ((end = times(&end_tms)) < 0) {
        perror("tiems error");
        exit(-1);
    }
    pr_times(end - start, start_tms, end_tms);
    pr_exit(status);
}

static void pr_times(clock_t wall_clock_tick, struct tms start_tms, struct tms end_tms)
{
    static long clktck = 0;
    if (clktck == 0)
        if ((clktck = sysconf(_SC_CLK_TCK)) < 0) {
            printf("sysconf error\n");
            exit(-1);
        }
    printf("real: %7.2f\n", wall_clock_tick / (double)clktck);
    printf("user cpu:%7.2f\n", (end_tms.tms_utime - start_tms.tms_utime) / (double)clktck);
    printf("system cpu:%7.2f\n", (end_tms.tms_stime - start_tms.tms_stime) / (double)clktck);
    printf("child user cpu:%7.2f\n", (end_tms.tms_cutime - start_tms.tms_cutime) / (double)clktck);
    printf("child system cpu:%7.2f\n", (end_tms.tms_cstime - start_tms.tms_cstime) / (double)clktck);
}

void pr_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status:%d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf(
            "abnormal termination, signal number:%d%s\n", WTERMSIG(status), WCOREDUMP(status) ? " (core dumped)" : "");
    else if (WIFSTOPPED(status))
        printf("child stoped, signal number:%d\n", WSTOPSIG(status));
    else if (WIFCONTINUED(status))
        printf("child continued, signal number:%d\n", SIGCONT);
}

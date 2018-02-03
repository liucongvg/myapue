#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIG SIGUSR1

static void diec(int error, const char* msg)
{

    fprintf(stderr, "%s: %s\n", msg, strerror(error));
    exit(1);
}

static void die(const char* msg) { diec(errno, msg); }

#if 0
static void
print_alive_fd(const char *tag)
{
	struct stat sb;
	int i;

	for (i = 0; i < 32; i++) {
		if (fstat(i, &sb) == -1)
			continue;
		printf("%s: fd %d is alive.\n", tag, i);
	}
}
#endif

static int do_sendmsg(int sock, const char* sockpath, int fd)
{
    struct msghdr msg;
    struct cmsghdr* cmsghdr;
    struct iovec iov[1];
    ssize_t nbytes;
    int i, *p;
    char buf[CMSG_SPACE(sizeof(int))], c;

    c = '*';
    iov[0].iov_base = &c;
    iov[0].iov_len = sizeof(c);
    memset(buf, 0x0b, sizeof(buf));
    cmsghdr = (struct cmsghdr*)buf;
    cmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
    cmsghdr->cmsg_level = SOL_SOCKET;
    cmsghdr->cmsg_type = SCM_RIGHTS;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = sizeof(iov) / sizeof(iov[0]);
    msg.msg_control = cmsghdr;
    msg.msg_controllen = CMSG_LEN(sizeof(int));
    msg.msg_flags = 0;
    p = (int*)CMSG_DATA(cmsghdr);
    *p = fd;
    printf("sendmsg: %d\n", fd);

    nbytes = sendmsg(sock, &msg, 0);
    if (nbytes == -1)
        return (1);

    return (0);
}

static int server_main(pid_t ppid, const char* sockpath)
{
    struct sockaddr_storage storage;
    struct sockaddr_un* addr;
    int error, fd, s, sock;
    const char* filepath = "fd_passing.txt";

    sock = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (sock == -1)
        die("socket(2)");
    addr = (struct sockaddr_un*)&storage;
    addr->sun_family = AF_LOCAL;
    strncpy(addr->sun_path, sockpath, sizeof(addr->sun_path));
    if (bind(sock, (struct sockaddr*)addr, SUN_LEN(addr)) == -1)
        die("bind(2)");
    if (listen(sock, 0) == -1)
        goto fail;
    if (kill(ppid, SIG) == -1)
        goto fail;
    if ((s = accept(sock, NULL, 0)) == -1)
        goto fail;
    if (unlink(filepath) == -1)
        goto fail;
    if ((fd = open(filepath, O_WRONLY | O_CREAT, 0644)) == -1)
        goto fail;

    if (do_sendmsg(s, sockpath, fd) != 0)
        goto fail;

    if (close(fd) == -1)
        goto fail;
    if (close(s) == -1)
        goto fail;
    if (close(sock) == -1)
        goto fail;
    if (unlink(sockpath) == -1)
        goto fail;

    return (0);

fail:
    error = errno;
    unlink(sockpath);
    diec(error, "");

    /* NOTREACHED */
    return (1);
}

static int do_recvmsg(int sock)
{
    struct msghdr msg;
    struct cmsghdr* cmsghdr;
    struct iovec iov[1];
    ssize_t nbytes;
    int i, *p;
    char buf[CMSG_SPACE(sizeof(int))], c;

    iov[0].iov_base = &c;
    iov[0].iov_len = sizeof(c);
    memset(buf, 0x0d, sizeof(buf));
    cmsghdr = (struct cmsghdr*)buf;
    cmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
    cmsghdr->cmsg_level = SOL_SOCKET;
    cmsghdr->cmsg_type = SCM_RIGHTS;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = iov;
    msg.msg_iovlen = sizeof(iov) / sizeof(iov[0]);
    msg.msg_control = cmsghdr;
    msg.msg_controllen = CMSG_LEN(sizeof(int));
    msg.msg_flags = 0;

    nbytes = recvmsg(sock, &msg, 0);
    if (nbytes == -1)
        return (1);

    p = (int*)CMSG_DATA(cmsghdr);
    printf("recvmsg: %d\n", *p);
// fp = fdopen(*p, "w");
// fprintf(fp, "OK\n");
#define MAXLINE 4096
    char buffer[MAXLINE];
    int count = read(*p, buffer, MAXLINE);
    if (count < 0) {
        perror("read");
        return -1;
    } else {
        printf("%d bytes in file\n", count);
    }
    buffer[count] = 0;
    printf("file content:%s\n", buffer);
    return (0);
}

static int client_main(pid_t pid, const char* sockpath)
{
    struct sockaddr_storage sockaddr;
    struct sockaddr_un* addr;
    sigset_t set;
    int sig, sock, status;
    const char* signame;

    if (sigemptyset(&set) == -1)
        die("sigemptyset(3)");
    if (sigaddset(&set, SIG) == -1)
        die("sigaddset(3)");
    if (sigwait(&set, &sig) != 0)
        die("sigwait(2)");
    if (sig != SIG)
        return (2);
    sock = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (sock == -1)
        die("socket(2)");
    addr = (struct sockaddr_un*)&sockaddr;
    addr->sun_family = AF_LOCAL;
    strncpy(addr->sun_path, sockpath, sizeof(addr->sun_path));
    if (connect(sock, (struct sockaddr*)addr, SUN_LEN(addr)) == -1)
        die("connect(2)");

    if (do_recvmsg(sock) == -1)
        return (3);

    if (close(sock) == -1)
        die("close(2)");
    if (wait4(pid, &status, 0, NULL) == -1)
        die("wait4(2)");
    if (!WIFEXITED(status))
        return (4);
    if (WEXITSTATUS(status) != 0)
        return (32 + WEXITSTATUS(status));

    return (0);
}

int main(int argc, const char* argv[])
{
    sigset_t set;
    pid_t pid, ppid;
    char sockpath[MAXPATHLEN];

    if (sigfillset(&set) == -1)
        die("sigfillset(3)");
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1)
        die("sigprocmask(2)");

    ppid = getpid();
    snprintf(sockpath, sizeof(sockpath), "%d.sock", ppid);

    pid = fork();
    switch (pid) {
    case -1:
        die("fork(2)");
    case 0:
        return (server_main(ppid, sockpath));
    default:
        break;
    }

    return (client_main(pid, sockpath));
}

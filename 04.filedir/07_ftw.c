#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef int Myfunc(const char* pathname, const struct stat* statptr, int type);
static Myfunc myfunc;
static int myftw(char* pathname, Myfunc* myfunc);
static int dopath(Myfunc* myfunc);
static char* path_alloc(size_t* sizep);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("usage: ./07_ftw path_name\n");
        return -1;
    }
    int ret = myftw(argv[1], myfunc);
    ntot = nreg + ndir + nblk + nfifo + nslink + nsock;
    if (ntot == 0)
        ntot = 1;
    printf("regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
    printf("directories = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
    printf("block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
    printf("char special = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
    printf("FIFOs special = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
    printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
    printf("sockets = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
    return ret;
}

static char* fullpath;
static size_t pathlen;
static int myftw(char* pathname, Myfunc* myfunc)
{
    fullpath = path_alloc(&pathlen);
    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            printf("error realloc\n");
            exit(EXIT_FAILURE);
        }
    }
    strcpy(fullpath, pathname);
    return dopath(myfunc);
}

#define FTW_FILE 1
#define FTW_DIRECTORY 2
#define FTW_DIRECTORY_READ_ERROR 3
#define FTW_STAT_ERROR 4

static int dopath(Myfunc* myfunc)
{
    struct stat statbuf;
    struct dirent* dirp;
    DIR* dp;
    int ret, n;
    if (lstat(fullpath, &statbuf) < 0)
        return myfunc(fullpath, &statbuf, FTW_STAT_ERROR);
    if (S_ISDIR(statbuf.st_mode) == 0) // not dir
        return myfunc(fullpath, &statbuf, FTW_FILE);
    // it's a directory
    if ((ret = myfunc(fullpath, &statbuf, FTW_DIRECTORY)) != 0)
        return ret;
    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
            printf("error realloc\n");
            exit(EXIT_FAILURE);
        }
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;
    if ((dp = opendir(fullpath)) == NULL)
        return myfunc(fullpath, &statbuf, FTW_DIRECTORY_READ_ERROR);
    while ((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0)
            continue;
        strcpy(&fullpath[n], dirp->d_name);
        if ((ret = dopath(myfunc)) != 0)
            break;
    }
    fullpath[n - 1] = 0;
    if (closedir(dp) < 0) {
        perror("closedir error");
        return -1;
    }
    return 0;
}

static int myfunc(const char* pathname, const struct stat* statptr, int type)
{
    switch (type) {
    case FTW_FILE:
        switch (statptr->st_mode & S_IFMT) {
        case S_IFREG:
            ++nreg;
            break;
        case S_IFBLK:
            ++nblk;
            break;
        case S_IFCHR:
            ++nchr;
            break;
        case S_IFIFO:
            ++nfifo;
            break;
        case S_IFLNK:
            ++nslink;
            break;
        case S_IFSOCK:
            ++nsock;
            break;
        case S_IFDIR:
            printf("not should be S_IFDIR\n");
            break;
        }
    case FTW_DIRECTORY:
        ++ndir;
        break;
    case FTW_STAT_ERROR:
        printf("can't stat file\n");
        break;
    case FTW_DIRECTORY_READ_ERROR:
        printf("can't read directory:%s\n", pathname);
        break;
    default:
        printf("error type %d for pathname %s\n", type, pathname);
        break;
    }
    return 0;
}

#ifdef PATH_MAX
static long pathmax = PATH_MAX;
#else
static long pathmax = 0;
#endif

static long posix_version = 0;
static long xsi_version = 0;
#define PATH_MAX_GUESS 1024

static char* path_alloc(size_t* sizep)
{
    char* ptr;
    size_t size;
    if (posix_version == 0)
        posix_version = sysconf(_SC_VERSION);
    if (xsi_version == 0)
        xsi_version = sysconf(_SC_XOPEN_VERSION);
    if (pathmax == 0) {
        errno = 0;
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;
            else {
                perror("pathconf error");
                exit(EXIT_FAILURE);
            }
        } else
            ++pathmax;
    }
    if ((posix_version < 200112L) && (xsi_version < 4))
        size = pathmax + 1;
    else
        size = pathmax;
    if ((ptr = malloc(size)) == NULL) {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }
    if (sizep != NULL)
        *sizep = size;
    return ptr;
}

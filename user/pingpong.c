#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent[2], child[2];
    // idx = 0, read
    // idx = 1, write
    pipe(parent);
    pipe(child);

    char buf[5];

    int pid = fork();
    if (pid < 0) { 
        fprintf(2, "error: fork error\n");
    } else if(pid == 0) {
        if(read(child[0], buf, 5)) {
            fprintf(1, "<%d>: received %s\n", getpid(), buf);
            write(parent[1], "pong", 5);
        }
    } else {
        write(child[1], "ping", 5);
        if(read(parent[0], buf ,5)) {
            fprintf(1, "<%d>: received %s\n", getpid(), buf);
        }
    }

    close(parent[0]);
    close(parent[1]);
    close(child[0]);
    close(child[1]);

    exit(0);
}
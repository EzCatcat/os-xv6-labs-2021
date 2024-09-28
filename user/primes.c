#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define SIZE_INT sizeof(int)

void filter(int pipe_fd[2])
{
    close(pipe_fd[1]);
    int prime;
    if(read(pipe_fd[0], &prime, SIZE_INT) != SIZE_INT) {
        fprintf(2, "error: read error\n");
        exit(1);
    }
    fprintf(1, "prime %d\n",prime);

    // pipe_fd is empty?
    int num;
    if(read(pipe_fd[0], &num, SIZE_INT) == 0) {
        exit(0);
    }

    int new_pipe_fd[2];
    pipe(new_pipe_fd);
    int pid = fork();
    if(pid < 0) {
        fprintf(2, "error: fork error\n");
        exit(1);
    } else if(pid == 0) {
        filter(new_pipe_fd);
    } else {
        close(new_pipe_fd[0]);
        if(num % prime != 0) {
            write(new_pipe_fd[1], &num, SIZE_INT);
        }
        while(read(pipe_fd[0], &num, SIZE_INT) > 0) {
            if(num % prime != 0) {
                write(new_pipe_fd[1], &num, SIZE_INT);
            }
        }
        close(new_pipe_fd[1]);
        close(pipe_fd[0]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int pipe_fd[2];
    pipe(pipe_fd);

    int pid = fork();
    if(pid < 0) {
        fprintf(2, "error: fork error\n");
        exit(1);
    } else if(pid == 0) {
        filter(pipe_fd);
    } else {
        close(pipe_fd[0]);
        for(int i=2; i<=35; i++) {
            if(write(pipe_fd[1], &i, SIZE_INT) != SIZE_INT) {
                fprintf(2, "error: write error\n");
                exit(1);
            }
        } 
        close(pipe_fd[1]);
        wait(0);
    } 
    exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int rec_seive(int* fd) {
    close(fd[1]);
    int my_prime;
    while(my_prime == 0) {
        read(fd[0], &my_prime, 4);
    }
    printf("prime %d\n", my_prime);
    int num;
    int another_fd[] = {0, 0};
    while (read(fd[0], &num, 4) != 0) {
        if (num % my_prime != 0) {
            if ((another_fd[0] == 0) && (another_fd[1] == 0)) {
                pipe(another_fd);
                int child = fork();
                if (child == 0) {
                    rec_seive(another_fd);
                    exit(0);
                }
                else {
                    close(another_fd[0]);
                }
            }
            write(another_fd[1], &num, 4);
        }
    }
    if (another_fd[0] == 0 && another_fd[1] == 0) {
        close(fd[0]);
        exit(0);
    }
    close(another_fd[1]);
    while (wait(0) != -1);
    close(fd[0]);
    exit(0);
}

int main() {
    int fd[2];
    pipe(fd);
    int child = fork();
    if (child != 0) {
        close(fd[0]);
        for (int i = 2; i < 36; ++i) {
            write(fd[1], &i, 4);
        }
        close(fd[1]);
        while (wait(0) != -1);
        exit(0);
    }

    else {
        rec_seive(fd);
        exit(0);
    }
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char* argv[]) {
    char buf[100][100];
    while (1) {
        int i = 0;
        int k = 0;
        char c;
        int stop = 1;
        while (1) {
            stop = read(0, &c, 1);
            if (c == '\n') {
                buf[k][i] = '\0';
                break;
            }
            if (stop == 0) {
                break;
            }
            if (c == ' ') {
                buf[k][i] = '\0';
                k++;
                continue;
            }
            buf[k][i] = c;
            ++i;
        }
        if (stop == 0) {
            break;
        }
        char* new_argv[argc + k + 1];
        for (int j = 0; j < argc - 1; ++j) {
            new_argv[j] = argv[j + 1];
        }
        for (int j = 0; j < k + 1; ++j) {
            new_argv[argc - 1 + j] = buf[j];
        }
        int child = fork();
        if (child == 0) {
            exec(argv[1], new_argv);
            exit(0);
        }
        wait(0);
    }
    exit(0);
}
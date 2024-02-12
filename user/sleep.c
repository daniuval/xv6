#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        char* error_message = "one argument expected\n";
        write(1, error_message, strlen(error_message));
        exit(0);
    }

    sleep(atoi(argv[1]));
    exit(0);
}
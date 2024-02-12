#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int parent_to_child[2];
    int child_to_parent[2];
    pipe(child_to_parent);
    pipe(parent_to_child);
    int child = fork();
    char message = 'a';
    if (child == 0) {
        char received;
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        read(parent_to_child[0], &received, 0);
        printf("%d: received ping\n", getpid());
        write(child_to_parent[1], &received, 1);
        close(child_to_parent[1]);
        close(parent_to_child[0]);
        exit(0);
    }
    else {
        char received;
        close(child_to_parent[1]);
        close(parent_to_child[0]);
        write(parent_to_child[1], &message, 1);
        read(child_to_parent[0], &received, 0);
        printf("%d: received pong\n", getpid());
        close(parent_to_child[1]);
        close(child_to_parent[0]);
        exit(0);
    }
    
}
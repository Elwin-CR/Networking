#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    char cwd[1024];

    pid = fork();

    if (pid = -1) {
        // If fork() fails
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        printf("Child process PID: %d\n", getpid());
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd failed");
            exit(1);
        }
        printf("Child working directory: %s\n", cwd);
        exit(1); 
    } else {
       
        printf("Parent process PID: %d\n", getpid());
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Child process finished with status: %d\n", WEXITSTATUS(status));
        }

        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd failed");
            return 1;
        }
        printf("Parent working directory: %s\n", cwd);
    }

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h> 

int main() {
    char cwd[1024];

    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        exit(1); 
    }


    printf("Current directory: %s\n", cwd);

    DIR *dir;
    struct dirent *entry; 


    dir = opendir(cwd);
    if (dir == NULL) {
        perror("Unable to open directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);

    return 0;
}


//
// Created by YeonwooSung on 2018. 5. 25..
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUFF_SIZE 2048

int main() {
    char *cwd = (char *) malloc(sizeof(char) * BUFF_SIZE);

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd() error");
    }
    free(cwd);
    return 0;
}

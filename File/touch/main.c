//
// Created by YeonwooSung on 2018. 5. 28..
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define USAGE_MESSAGE "Usage: touch [-A [-][[hh]mm]SS] [-acfhm] [-r file] [-t [[CC]YY]MMDDhhmm[.SS]] file ..."
#define FAILED_MESSAGE "failed to create file"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, USAGE_MESSAGE);
    }

    char *fileName = (char *) malloc(sizeof(char) * 50);

    //TODO check arguments

    int n = open(fileName, O_WRONLY|O_CREAT|O_TRUNC);

    if (n != 0) {
        return n;
    }

    fprintf(stderr, FAILED_MESSAGE);

    return -1;
}

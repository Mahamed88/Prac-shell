//Cd-builtin by krishma.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"

int builtin_cd(char **args, int argc) {
    char *target_dir;

    // Case 1: cd with no arguments(go to HOME).
    if (argc == 1) {
        target_dir = getenv("HOME");
        if (target_dir == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    // Case 2: cd with one argument.
    else if (argc == 2) {
        target_dir = args[1];
    }
    // Case 3: too many arguments(error).
    else {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    // Attempt to change directory.
    if (chdir(target_dir) != 0) {
        perror("cd");
        return 1;
    }
    // return 1(handled as built-in).
    return 1; 
}

#include <string.h>
#include "builtins.h"

int handle_builtin(char **args, int argc) {
    if (argc == 0) {
        return 1;
    }

    if (strcmp(args[0], "cd") == 0) {
        return builtin_cd(args, argc);
    }

    if (strcmp(args[0], "exit") == 0) {
        return builtin_exit(args, argc);
    }

    if (strcmp(args[0], "path") == 0) {
        return builtin_path(args, argc); 
    }

    if (strcmp(args[0], "myhistory") == 0) {
        return builtin_myhistory(args, argc);
    }

    if (strcmp(args[0], "alias") == 0) {
        return builtin_alias(args, argc);
    }

    return 0;
}

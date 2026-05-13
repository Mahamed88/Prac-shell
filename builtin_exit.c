/*
builtin_exit by Belen Lezama
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builtins.h"
#include "shell.h"

int builtin_exit(char **args, int argc)
{
    /*display message is there are too many commands*/
    if(argc > 1)
    {
        fprintf(stderr, "exit: too many commands\n");
        return 1;
    }
        /*set shell should exit variable to 1*/
        shell_should_exit = 1;
        return 1;
}

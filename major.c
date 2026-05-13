#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "builtins.h"
#define MAX_LINE 512

/* 
 * process_line:
 * function:
 *  - split by semicolons
 *  - parse commands/arguments
 *  - detect built-ins
 *  - fork/exec external commands
 */

// Function implemented in shell.c 
void process_line(char *line); 


int main(int argc, char *argv[]) {
    FILE *input = NULL;
    char line[MAX_LINE + 2];  
    // +2 gives a little room in case input is slightly over the limit

    /*
     *   argc values are:
     *   1 -> interactive mode
     *   2 -> batch mode
     */
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [batchFile]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    init_path_builtin();
    
    /* Batch mode */
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "Error: cannot open batch file.\n");
            exit(EXIT_FAILURE);
        }

        while (fgets(line, sizeof(line), input) != NULL) {
            /*
             * defensive check for very long lines
             * if the line does not contain '\n', it may be too long
             */
            if (strchr(line, '\n') == NULL && !feof(input)) {
                fprintf(stderr, "Error: command line too long.\n");

                // discard rest of the line
                int ch;
                while ((ch = fgetc(input)) != '\n' && ch != EOF)
                    ;
                continue;
            }

            // Echo each batch line before executing it
            printf("%s", line);

            process_line(line);

            // Handle exist
            if (shell_should_exit)
            {
                break;
            } 
        }

        fclose(input);
    }
    /* Interactive mode */
    else {
        while (1) {
            printf("newshell> ");
            fflush(stdout);

            if (fgets(line, sizeof(line), stdin) == NULL) {
                // Handles Ctrl-D / EOF gracefully
                printf("\n");
                break;
            }

            if (strchr(line, '\n') == NULL && !feof(stdin)) {
                fprintf(stderr, "Error: command line too long.\n");

                // discard rest of the line
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF)
                    ;
                continue;
            }

            process_line(line);
           // Handle exist
            if (shell_should_exit) {
                break;
            } 
        }
    }

    cleanup_path_builtin();

    return 0;
}

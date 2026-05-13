#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

/* we need this to re-run commands */
void process_line(char *line);

#define HISTORY_SIZE 20
#define MAX_LINE 512

/* circular buffer */
static char history[HISTORY_SIZE][MAX_LINE + 1];
static int history_count = 0;

/*
 * add_history_entry:
 * store command into circular history buffer
 */
void add_history_entry(const char *command) {
    if (command == NULL || *command == '\0') {
        return;
    }

    strncpy(history[history_count % HISTORY_SIZE], command, MAX_LINE);
    history[history_count % HISTORY_SIZE][MAX_LINE] = '\0';

    history_count++;
}

/*
 * show_history:
 */
static void show_history(void) {
    int total = (history_count < HISTORY_SIZE) ? history_count : HISTORY_SIZE;
    int start = (history_count > HISTORY_SIZE) ? (history_count - HISTORY_SIZE) : 0;

    for (int i = 0; i < total; i++) {
        printf("%d %s\n", i + 1, history[(start + i) % HISTORY_SIZE]);
    }
}

/*
 * clear_history:
 */
static void clear_history(void) {
    for (int i = 0; i < HISTORY_SIZE; i++) {
        history[i][0] = '\0';
    }
    history_count = 0;
}

/*
 * execute_history:
 */
static int execute_history(int index) {
    int total = (history_count < HISTORY_SIZE) ? history_count : HISTORY_SIZE;
    int start = (history_count > HISTORY_SIZE) ? (history_count - HISTORY_SIZE) : 0;

    if (index < 1 || index > total) {
        return -1;
    }

    char command[MAX_LINE + 1];
    strncpy(command, history[(start + index - 1) % HISTORY_SIZE], MAX_LINE);
    command[MAX_LINE] = '\0';

    process_line(command);
    return 0;
}

/*
 * builtin_myhistory:
 */
int builtin_myhistory(char **args, int argc) {
    if (argc == 1) {
        show_history();
    }
    else if (argc == 2 && strcmp(args[1], "-c") == 0) {
        clear_history();
    }
    else if (argc == 3 && strcmp(args[1], "-e") == 0) {
        int index = atoi(args[2]);
        if (execute_history(index) != 0) {
            fprintf(stderr, "myhistory: invalid index\n");
        }
    }
    else {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  myhistory\n");
        fprintf(stderr, "  myhistory -c\n");
        fprintf(stderr, "  myhistory -e <number>\n");
    }

    return 1;
}

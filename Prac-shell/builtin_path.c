#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builtins.h"

#define MAX_PATHS 100

static char *path_list[MAX_PATHS];
static int path_count = 0;

/*
 * Initialize PATH from environment
 */
void init_path_builtin(void) {
    char *env = getenv("PATH");

    if (!env) return;

    char *copy = strdup(env);
    char *token = strtok(copy, ":");

    while (token && path_count < MAX_PATHS) {
        path_list[path_count++] = strdup(token);
        token = strtok(NULL, ":");
    }

    free(copy);
}

/*
 * Print PATH
 */
static void print_path() {
    for (int i = 0; i < path_count; i++) {
        printf("%s", path_list[i]);
        if (i < path_count - 1) {
            printf(":");
        }
    }
    printf("\n");
}

/*
 * Add path
 */
static void add_path(char *new_path) {
    if (path_count < MAX_PATHS) {
        path_list[path_count++] = strdup(new_path);
    }
}

/*
 * Remove path
 */
static void remove_path(char *target) {
    for (int i = 0; i < path_count; i++) {
        if (strcmp(path_list[i], target) == 0) {
            free(path_list[i]);

            for (int j = i; j < path_count - 1; j++) {
                path_list[j] = path_list[j + 1];
            }

            path_count--;
            return;
        }
    }
}


static void update_env_path() {
    char buffer[1024] = "";

    for (int i = 0; i < path_count; i++) {
        strcat(buffer, path_list[i]);

        if (i < path_count - 1) {
            strcat(buffer, ":");
        }
    }

    setenv("PATH", buffer, 1);
}

/*
 * Main builtin function
 */
int builtin_path(char **args, int argc) {

    if (argc == 1) {
        print_path();
        return 1;
    }

    if (argc == 3 && strcmp(args[1], "+") == 0) {
        add_path(args[2]);
        update_env_path();
        return 1;
    }

    if (argc == 3 && strcmp(args[1], "-") == 0) {
        remove_path(args[2]);
        update_env_path();
        return 1;
    }

    fprintf(stderr, "Invalid path command\n");
    return 1;
}


void cleanup_path_builtin(void) {
    for (int i = 0; i < path_count; i++) {
        free(path_list[i]);
    }
}

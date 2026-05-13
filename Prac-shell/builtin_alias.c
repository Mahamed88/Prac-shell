#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"

#define MAX_LINE 512

typedef struct Alias {
    char name[64];
    char command[MAX_LINE + 1];
    struct Alias *next;
} Alias;

static Alias *alias_head = NULL;

const char *lookup_alias(const char *name) {
    Alias *curr = alias_head;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            return curr->command;
        }
        curr = curr->next;
    }

    return NULL;
}

static void add_alias_entry(const char *name, const char *command) {
    Alias *curr = alias_head;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            strncpy(curr->command, command, MAX_LINE);
            curr->command[MAX_LINE] = '\0';
            return;
        }
        curr = curr->next;
    }

    curr = (Alias *)malloc(sizeof(Alias));
    if (curr == NULL) {
        perror("malloc");
        return;
    }

    strncpy(curr->name, name, sizeof(curr->name) - 1);
    curr->name[sizeof(curr->name) - 1] = '\0';

    strncpy(curr->command, command, MAX_LINE);
    curr->command[MAX_LINE] = '\0';

    curr->next = alias_head;
    alias_head = curr;
}

static void remove_alias_entry(const char *name) {
    Alias *curr = alias_head;
    Alias *prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            if (prev == NULL) {
                alias_head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

static void clear_alias_entries(void) {
    Alias *curr = alias_head;
    Alias *next;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    alias_head = NULL;
}

static void print_alias_entries(void) {
    Alias *curr = alias_head;

    while (curr != NULL) {
        printf("%s='%s'\n", curr->name, curr->command);
        curr = curr->next;
    }
}

int builtin_alias(char **args, int argc) {
    char buffer[MAX_LINE + 1] = "";
    char *eq;
    char *name;
    char *value;
    int i;

    if (argc == 1) {
        print_alias_entries();
        return 1;
    }

    if (argc == 2 && strcmp(args[1], "-c") == 0) {
        clear_alias_entries();
        return 1;
    }

    if (argc == 3 && strcmp(args[1], "-r") == 0) {
        remove_alias_entry(args[2]);
        return 1;
    }

    for (i = 1; i < argc; i++) {
        if (i > 1) {
            strncat(buffer, " ", sizeof(buffer) - strlen(buffer) - 1);
        }
        strncat(buffer, args[i], sizeof(buffer) - strlen(buffer) - 1);
    }

    eq = strchr(buffer, '=');
    if (eq == NULL) {
        fprintf(stderr, "alias: invalid format\n");
        return 1;
    }

    *eq = '\0';
    name = buffer;
    value = eq + 1;

    if (*value == '\'') {
        value++;
        if (strlen(value) > 0 && value[strlen(value) - 1] == '\'') {
            value[strlen(value) - 1] = '\0';
        }
    }

    if (strlen(name) == 0 || strlen(value) == 0) {
        fprintf(stderr, "alias: invalid format\n");
        return 1;
    }

    add_alias_entry(name, value);
    return 1;
}

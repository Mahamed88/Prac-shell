#ifndef BUILTINS_H
#define BUILTINS_H

int handle_builtin(char **args, int argc);

int builtin_cd(char **args, int argc);
int builtin_exit(char **args, int argc);
int builtin_path(char **args, int argc);
int builtin_myhistory(char **args, int argc);
int builtin_alias(char **args, int argc);

void init_path_builtin(void);
void cleanup_path_builtin(void);

void add_history_entry(const char *command);

const char *lookup_alias(const char *name);

#endif

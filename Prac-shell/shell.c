#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "shell.h"
#include "builtins.h"

int shell_should_exit = 0;

static void shell_signal_handler(void) {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

static char *trim_whitespace(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == '\0') {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    *(end + 1) = '\0';
    return str;
}

static int parse_arguments(char *cmd, char *args[]) {
    int argc = 0;
    char *saveptr = NULL;
    char *token = strtok_r(cmd, " \t", &saveptr);

    while (token != NULL && argc < MAX_ARGS - 1) {
        args[argc++] = token;
        token = strtok_r(NULL, " \t", &saveptr);
    }

    args[argc] = NULL;
    return argc;
}

static void reset_child_signals(void) {
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
}

static void run_external_command_with_redirect(char **args,
                                               char *input_file,
                                               char *output_file) {
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        reset_child_signals();

        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("open input");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open output");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    }

    waitpid(pid, &status, 0);
}

static void handle_redirection(char *cmd) {
    char copy[MAX_LINE];
    char *args[MAX_ARGS];
    char *input_file = NULL;
    char *output_file = NULL;
    char *saveptr = NULL;
    char *token;
    int argc = 0;

    strncpy(copy, cmd, MAX_LINE - 1);
    copy[MAX_LINE - 1] = '\0';

    token = strtok_r(copy, " \t", &saveptr);

    while (token != NULL && argc < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token == NULL) {
                fprintf(stderr, "redirection: missing input file\n");
                return;
            }
            input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token == NULL) {
                fprintf(stderr, "redirection: missing output file\n");
                return;
            }
            output_file = token;
        } else {
            args[argc++] = token;
        }

        token = strtok_r(NULL, " \t", &saveptr);
    }

    args[argc] = NULL;

    if (argc == 0) {
        return;
    }

    if (handle_builtin(args, argc)) {
        return;
    }

    run_external_command_with_redirect(args, input_file, output_file);
}

static void run_pipeline(char *cmd) {
    char *commands[3];
    char *args[3][MAX_ARGS];
    char *saveptr = NULL;
    char *token;
    int num_commands = 0;
    int pipes[2][2];
    pid_t pids[3];
    int i, j;

    token = strtok_r(cmd, "|", &saveptr);

    while (token != NULL && num_commands < 3) {
        commands[num_commands++] = trim_whitespace(token);
        token = strtok_r(NULL, "|", &saveptr);
    }

    if (token != NULL) {
        fprintf(stderr, "pipeline: maximum of 3 commands allowed\n");
        return;
    }

    for (i = 0; i < num_commands; i++) {
        parse_arguments(commands[i], args[i]);

        if (args[i][0] == NULL) {
            fprintf(stderr, "pipeline: empty command\n");
            return;
        }
    }

    for (i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe");
            return;
        }
    }

    for (i = 0; i < num_commands; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork");
            return;
        }

        if (pids[i] == 0) {
            reset_child_signals();

            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(args[i][0], args[i]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

static void execute_command(char *cmd) {
    char temp[MAX_LINE];
    char alias_check[MAX_LINE];
    char rebuilt[MAX_LINE];
    char *args[MAX_ARGS];
    char *first;
    const char *alias_value;
    int argc;

    cmd = trim_whitespace(cmd);

    if (*cmd == '\0') {
        return;
    }

    strncpy(temp, cmd, MAX_LINE - 1);
    temp[MAX_LINE - 1] = '\0';

    strncpy(alias_check, temp, MAX_LINE - 1);
    alias_check[MAX_LINE - 1] = '\0';

    first = strtok(alias_check, " \t");

    if (first != NULL) {
        alias_value = lookup_alias(first);

        if (alias_value != NULL) {
            snprintf(rebuilt, sizeof(rebuilt), "%s%s",
                     alias_value,
                     temp + strlen(first));

            strncpy(temp, rebuilt, MAX_LINE - 1);
            temp[MAX_LINE - 1] = '\0';
        }
    }

    if (strchr(temp, '|') != NULL) {
        run_pipeline(temp);
        return;
    }

    if (strchr(temp, '<') != NULL || strchr(temp, '>') != NULL) {
        handle_redirection(temp);
        return;
    }

    argc = parse_arguments(temp, args);

    if (argc == 0) {
        return;
    }

    if (!handle_builtin(args, argc)) {
        run_external_command_with_redirect(args, NULL, NULL);
    }
}

void process_line(char *line) {
    char copy[MAX_LINE];
    char history_copy[MAX_LINE];
    char *cmd;
    char *saveptr = NULL;

    shell_signal_handler();

    line[strcspn(line, "\n")] = '\0';

    strncpy(copy, line, MAX_LINE - 1);
    copy[MAX_LINE - 1] = '\0';

    strncpy(history_copy, line, MAX_LINE - 1);
    history_copy[MAX_LINE - 1] = '\0';

    if (*trim_whitespace(history_copy) != '\0') {
        add_history_entry(trim_whitespace(history_copy));
    }

    cmd = strtok_r(copy, ";", &saveptr);

    while (cmd != NULL) {

        if(shell_should_exit)
        {
            return;
        }

        execute_command(cmd);
        cmd = strtok_r(NULL, ";", &saveptr);
    }
}

#ifndef SHELL_H
#define SHELL_H

#define MAX_LINE 512
#define MAX_ARGS 128

extern int shell_should_exit;

void process_line(char *line);

#endif

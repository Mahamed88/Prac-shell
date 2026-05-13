CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = newshell

OBJS = major.o shell.o builtins.o \
       builtin_cd.o builtin_exit.o \
       builtin_path.o builtin_myhistory.o builtin_alias.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

major.o: major.c shell.h builtins.h
	$(CC) $(CFLAGS) -c major.c

shell.o: shell.c shell.h builtins.h
	$(CC) $(CFLAGS) -c shell.c

builtins.o: builtins.c builtins.h
	$(CC) $(CFLAGS) -c builtins.c

builtin_cd.o: builtin_cd.c builtins.h
	$(CC) $(CFLAGS) -c builtin_cd.c

builtin_exit.o: builtin_exit.c builtins.h
	$(CC) $(CFLAGS) -c builtin_exit.c

builtin_path.o: builtin_path.c builtins.h
	$(CC) $(CFLAGS) -c builtin_path.c

builtin_myhistory.o: builtin_myhistory.c builtins.h shell.h
	$(CC) $(CFLAGS) -c builtin_myhistory.c

builtin_alias.o: builtin_alias.c builtins.h shell.h
	$(CC) $(CFLAGS) -c builtin_alias.c

clean:
	rm -f *.o $(TARGET)

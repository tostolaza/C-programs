#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

void prompt() {
    printf("mini-bash> ");
}

void read_input(char* line) {
    fgets(line, MAX_LINE, stdin);
    line[strcspn(line, "\n")] = '\0'; 
}

void parse_input(char* line, char** args) {
    int i = 0;
    args[i] = strtok(line, " ");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " ");
    }
}

int execute_command(char** args) {
    if (args[0] == NULL) {
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Uso: cd <directorio>\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd");
            }
        }
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("mini-bash");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
    } else {
        int status;
        waitpid(pid, &status, 0);
    }

    return 1;
}

int main() {
    char line[MAX_LINE];
    char* args[MAX_ARGS];
    int status = 1;

    while (status) {
        prompt();
        read_input(line);
        parse_input(line, args);
        status = execute_command(args);
    }

    return 0;
}


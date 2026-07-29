#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include "builtins.h"

#define INPUT_MAX_LENGTH 1024 // 1KB

int main()
{
    char *buffer = NULL;
    pid_t pid;
    char *cursor = "$> ";
    size_t bufsize = INPUT_MAX_LENGTH;
    char **tokens;
    char *token;
    size_t counter = 0;

    tokens = (char **)malloc(bufsize * sizeof(char *));

    if (tokens == NULL)
    {
        perror("Unable to allocate tokens");
        exit(EXIT_FAILURE);
    }

    do
    {
        printf("%s", cursor);
        read_command(&buffer, tokens);

        token = strtok(buffer, " ");

        while (token != NULL)
        {
            tokens[counter] = token;
            counter++;

            if (counter >= bufsize)
            {
                bufsize += INPUT_MAX_LENGTH;
                tokens = realloc(tokens, bufsize * sizeof(char *));
            }

            token = strtok(NULL, " ");
        }
        tokens[counter] = NULL;

        if (tokens[0] == NULL)
            continue;

        int i = 0;

        for (i; i < counter; i++)
        {
            if (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0 || strcmp(tokens[i], ">>") == 0)
                break;
        }
        if (counter > i)
        {
            char *commands[i + 1];
            for (int j = 0; j <= i; j++)
            {
                commands[j] = tokens[j];
            }
            commands[i] = NULL;

            if (tokens[i + 1] != NULL)
            {
                redirections(commands, tokens[i], tokens[i + 1]);
                counter = 0;
                continue;
            }
        }

        if (strcmp(tokens[0], "echo") == 0)
        {
            echo(counter, tokens);
            counter = 0;
            continue;
        }
        else if (strcmp(tokens[0], "cd") == 0)
        {
            cd(counter, tokens);
            counter = 0;
            continue;
        }
        else if (strcmp(tokens[0], "exit") == 0)
        {
            free(buffer);
            free(tokens);
            exit(EXIT_SUCCESS);
        }

        pid = fork();

        if (pid < 0) // error
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // child
        {
            if (execvp(tokens[0], tokens) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        else // parent
        {
            int status;
            waitpid(pid, &status, 0);
        }

        counter = 0;

    } while (1);
    return 0;
}

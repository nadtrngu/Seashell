#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "methods.h"

#define INPUT_MAX_LENGTH 1024 // 1KB

char *get_home_path()
{
    return getenv("HOME");
}


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

        if (strcmp(token, "exit") == 0)
        {
            free(buffer);
            free(tokens);
            exit(EXIT_SUCCESS);
        }
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

        if (strcmp(tokens[0], "echo") == 0)
        {
            if (counter > 1)
            {
                for(size_t i = 1; i < counter; i++)
                {
                    printf("%s ", tokens[i]);
                }
                
                printf("\n");
                fflush(stdout);
            }
            counter = 0;
            continue;
        }
        else if (strcmp(tokens[0], "cd") == 0)
        {
            char *home_path = NULL;
            if (counter == 1)
            {
                home_path = get_home_path();
            }
            else
            {
                home_path = tokens[1];
            }
            if (chdir(home_path) == -1)
            {
                perror("chdir");
            }
            counter = 0;
            continue;
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define INPUT_MAX_LENGTH 1024 // 1KB

int main()
{
    char *buffer = NULL;
    pid_t pid;
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
        printf("$> ");
        if (getline(&buffer, &bufsize, stdin) == -1)
        {
            if (!feof(stdin))
            {
                perror("getline");
                exit(EXIT_FAILURE);
            }
            // This happens when CTRL + D is pressed, so we need to cleanup.
            free(buffer);
            free(tokens);
        }

        buffer[strcspn(buffer, "\r\n")] = 0; // The function counts the number of characters until it hits a '\r' or a '\n'

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

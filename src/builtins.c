#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtins.h"
#define INPUT_MAX_LENGTH 1024 // 1KB

char *get_home_path()
{
    return getenv("HOME");
}

void read_command(char **buffer, char **tokens)
{
    size_t bufsize = INPUT_MAX_LENGTH;

    if (getline(buffer, &bufsize, stdin) == -1)
    {
        if (!feof(stdin))
        {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        // This happens when CTRL + D is pressed, so we need to cleanup.
        free(*buffer);
        free(tokens);
        exit(EXIT_SUCCESS);
    }

    // buffer is a char **
    // *buffer is a char *
    // (*buffer)[n] is a char
    (*buffer)[strcspn(*buffer, "\r\n")] = 0;
}

void echo(size_t counter, char **tokens)
{
    if (counter > 1)
    {
        for (size_t i = 1; i < counter; i++)
        {
            printf("%s ", tokens[i]);
        }

        printf("\n");
        fflush(stdout);
    }
}

void cd(size_t counter, char **tokens)
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
}

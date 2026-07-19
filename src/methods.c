#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "methods.h"
#define INPUT_MAX_LENGTH 1024 // 1KB

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
        free(buffer);
        free(tokens);
        exit(EXIT_SUCCESS);
    }

    // buffer is a char **
    // *buffer is a char *
    // (*buffer)[n] is a char
    (*buffer)[strcspn(*buffer, "\r\n")] = 0;
}

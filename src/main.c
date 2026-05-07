#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MAX_LENGTH 1024 // 1KB

int main()
{
    char* buffer;
    char *tokens;
    size_t bufsize = INPUT_MAX_LENGTH;

    tokens = (char*) malloc(bufsize * sizeof(char));
    buffer = (char*) malloc(bufsize * sizeof(char));

    if (tokens == NULL)
    {
        perror("Unable to allocate tokens");
        exit(EXIT_FAILURE);
    }

    do
    {
        printf("$> ");
        if (!(getline(&buffer, &bufsize, stdin)))
        {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        buffer[strcspn(buffer, "\r\n")] = 0; // The function counts the number of characters until it hits a '\r' or a '\n'
        tokens = strtok(buffer, " ");
        while (tokens != NULL)
        {
            printf("Your input is: %s\n", tokens);
            tokens = strtok(NULL, " ");
        }

    } while (1);
    free(tokens);
    free(buffer);
    return 0;
}

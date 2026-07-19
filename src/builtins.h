#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdlib.h>

void read_command(char **buffer, char **tokens);
void echo(size_t counter, char **tokens);
void cd(size_t counter, char **tokens);

#endif

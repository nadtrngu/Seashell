#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#ifndef METHODS_H
#define METHODS_H

void read_command(char **buffer, char **tokens);

#endif

#define UTILITIES__H
#ifdef UTILITIES__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024

char** parse_input(char* input);
void free_tokens(char** tokens);

#endif
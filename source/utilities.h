#define UTILITIES__H
#ifdef UTILITIES__H

#include "main.h"

#define MAX_INPUT 1024

char** parse_input(char* input);
void free_tokens(char** tokens);
int builtin_functions_shell(char** args, char** argv, char* initial_directory);
char* get_path(char* env[]);
char** split_into_paths(char* path, int* count);

#endif
#define MY_CUSTOM_SHELL_H
#ifdef MY_CUSTOM_SHELL_H

#include "main.h"

int builtin_functions_shell(char** args, char** env, char* initial_directory);
void loop_shell(char* envp[]);

#endif
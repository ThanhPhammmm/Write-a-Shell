#define COMMAND_H
#ifdef COMMAND_H

#include "main.h"

#define PATH_MAX 1024

int command_cd(char** args, char* initial_directory);
int command_pwd();
int command_env(char** env);
int command_echo(char** args, char** env);
int command_which(char** args, char** env);
char* find_command_in_path(char* args, char* env[]);
char** command_setenv(char* args[], char* env[]);
char** command_unsetenv(char* args[], char* env[]);
int count_env_vars(char* env[]);
int executor(char* args[], char* env[]);
int child_process(char* args[], char* env[]);


#endif
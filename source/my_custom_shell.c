#include "main.h"
#include "my_custom_shell.h"
#include "command.h"
#include "utilities.h"

// cd, pwd, echo, env, setenv, unsetenv, which, exit
int builtin_functions_shell(char** args, char** env, char* initial_directory){
    for(size_t i = 0;args[i];i++){
      printf("Arg[%ld]: %s", i, args[i]);
      printf("\n");
    } 
    if (strcmp(args[0], "cd") == 0) {
        return command_cd(args, initial_directory);
    } 
    else if (strcmp(args[0], "pwd") == 0) {
        return command_pwd();
    } 
    else if (strcmp(args[0], "echo") == 0) {
        return command_echo(args, env);
    } 
    else if (strcmp(args[0], "env") == 0) {
        return command_env(env);
    } 
    else if (strcmp(args[0], "which") == 0) {
        return command_which(args, env);
    } 
    else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
    } 
    else {
        // Not a builtin command
    }
    return 0;
}

void loop_shell(char* envp[]){
  char* input = NULL;
  size_t input_size = 0;

  char** args;

  char* initial_directory = getcwd(NULL, 0);

  while(1){
    printf("[ThanhPham_Shell]:~$ ");
    if(getline(&input, &input_size, stdin) == -1){
      perror("getline");
      break;
    }

    //printf("Input: %s", input);
 
    args = parse_input(input);

    if(args[0]){
      builtin_functions_shell(args, envp, initial_directory);
    }
  }

  free_tokens(args);
  free(input);
} 
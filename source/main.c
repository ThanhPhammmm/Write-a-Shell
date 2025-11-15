#include "utilities.h"

void loop_shell(char* argv[]){
  char* input = NULL;
  size_t input_size = 0;

  char** args;

  while(1){
    printf("[ThanhPham_Shell]> ");
    if(getline(&input, &input_size, stdin) == -1){
      perror("getline");
      break;
    }

    printf("Input: %s", input);

    args = parse_input(input);

    for(size_t i = 0;args[i];i++){
      printf("%s\n", args[i]);
    }
  }

  free_tokens(args);
  free(input);
} 

int main(int argc, char* argv[]){

  loop_shell(argv);
  return 0;
}
#include "main.h"
#include "my_custom_shell.h"

int main(int argc, char* argv[], char *envp[]){
  loop_shell(envp);
  return 0;
}
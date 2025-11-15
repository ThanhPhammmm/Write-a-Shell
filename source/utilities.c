#include "utilities.h"

char** parse_input(char* input){
    char** tokens = malloc(MAX_INPUT * sizeof(char*));
    size_t position = 0;
    size_t i = 0;

    if(!tokens){
        perror("Malloc");
        exit(EXIT_FAILURE);
    }

    while(input[i]){
        while(input[i] == ' ' || input[i] == '\n' || input[i] == '\t' || input[i] == '\r' || input[i] == '\a'){
            i++;
        }

        if(input[i] == '\0') break;

        size_t start = i;

        while (input[i] && input[i] != ' ' && input[i] != '\n' && input[i] != '\t' && input[i] != '\r'){
            i++;
        }

        size_t token_length = i - start;
        tokens[position] = malloc((token_length + 1));

        if(!tokens[position]){
            perror("Malloc");
            exit(EXIT_FAILURE);
        }

        strncpy(tokens[position], &input[start], token_length);

        tokens[position][token_length] = '\0';
        position++;
    }
    tokens[position] = NULL;
    return tokens;
}

void free_tokens(char** tokens){
    if(!tokens) return;

    for(size_t i = 0; tokens[i];i++){
        free(tokens[i]); 
    }
    free(tokens); 
}
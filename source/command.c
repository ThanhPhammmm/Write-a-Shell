#include "command.h"

int command_cd(char **args, char *init_dir) {
    static char prev_dir[PATH_MAX] = {0};
    char current_dir[PATH_MAX];

    if(getcwd(current_dir, sizeof(current_dir)) == NULL){
        perror("getcwd");
        return 0;
    }

    if(args[1] == NULL){
        char *home = getenv("HOME");
        if (home == NULL){
            fprintf(stderr, "cd: HOME not set\n");
            return 0;
        }
        if(chdir(home) != 0) perror("cd");
        else{
            strcpy(prev_dir, current_dir);
        }
        return 0;
    }

    if(strcmp(args[1], "-") == 0){
        if(prev_dir[0] == 0){
            fprintf(stderr, "cd: no previous directory\n");
            return 0;
        }
        printf("%s\n", prev_dir);
        if(chdir(prev_dir) != 0) perror("cd");
        else {
            strcpy(prev_dir, current_dir);
        }
        return 0;
    }

    if(args[1][0] == '~'){
        char path[PATH_MAX];
        char *home = getenv("HOME");
        if(!home) {
            fprintf(stderr, "cd: HOME not set\n");
            return 0;
        }

        // HOME + remaining characters after symbol ~
        snprintf(path, sizeof(path), "%s%s", home, args[1] + 1);

        if(chdir(path) != 0) perror("cd");
        else strcpy(prev_dir, current_dir);
        return 0;
    }

    if(chdir(args[1]) != 0){
        perror("cd");
    }
    else{
        strcpy(prev_dir, current_dir);
    }

    return 0;
}

int command_pwd(){
    char* cwd = NULL;

    cwd = getcwd(NULL, 0);

    if(cwd){
        printf("%s\n", cwd);
        free(cwd);
    }
    else{
        free(cwd);
        perror("getpwd");
    }
    return 0;
}

int command_echo(char** args, char** env){
    int newline = 1; // Mặc định echo xuống dòng
    int index = 1;

    // Kiểm tra option -n
    if(args[1] && strcmp(args[1], "-n") == 0){
        newline = 0;
        index = 2;
    }

    if(args[index] == NULL){
        if(newline) printf("\n");
        return 0;
    }

    for(size_t i = index;args[i];i++){
        //Support ${VAR}
        if(args[i][0] == '$'){
            char *name = NULL;
            if(args[i][1] == '{'){
                size_t len = strlen(args[i]);
                if(len >= 3 && args[i][len - 1] == '}'){
                    char temp[256];
                    if (len - 3 >= sizeof(temp)) {
                        fprintf(stderr, "echo: variable name too long\n");
                        return 1;
                    }
                    strncpy(temp, args[i] + 2, len - 3);
                    temp[sizeof(temp) - 1] = '\0';
                    name = temp;
                } 
                else{
                    fprintf(stderr, "echo: bad substitution\n");
                    return 1;
                }
            }
            else{
                name = args[i] + 1; // skip $
            }

            char *value = getenv(name);
            if (value) printf("%s", value);
            else printf("%s", args[i]); 
        }
        else printf("%s", args[i]);

        if(args[i + 1]) printf(" ");
    }
    if(newline){
        printf("\n");
    }
    return 0;
}

int command_env(char** env){
    size_t index = 0;
    while(env[index]){
        printf("%s\n", env[index]);
        index++;
    }
    return 0;
}

int command_which(char** args, char** env){
    if(args[1] == NULL){
        printf("which: expected argument\n");
        return 0;
    }
    char* built_in_commands[] = {"cd", "pwd", "echo", "env", "setenv", "unsetenv", "which", "exit", NULL};
    for(size_t i = 0;built_in_commands[i];i++){
        if(strcmp(built_in_commands[i], args[1]) == 0){
            printf("%s: shell built-in command\n", args[1]);
            return 0;
        }
    }

    char* full_path = find_command_in_path(args[1], env);
    if(full_path){
        printf("%s\n", full_path);
        free(full_path);
    }
    else{
        printf("which: %s command not found\n", args[1]);
    }
    return 0;
}

char* find_command_in_path(char* command, char* env[]){
    char* env_path = NULL;

    for(size_t i = 0;env[i];i++){
        if(strncmp(env[i], "PATH=", 5) == 0){
            env_path = env[i] + 5;
            break;
        }
    }
    if(env_path == NULL){
        return NULL;
    }

    char* temp_path = strdup(env_path);
    if(temp_path == NULL){
        perror("strdup");
        return NULL;
    }

    char* token = NULL;
    token = strtok(temp_path, ":");

    char full_path[1024];

    while(token != NULL){
        size_t len = strlen(token);
        if(token[len - 1] != '/'){
            snprintf(full_path, sizeof(full_path), "%s%s%s", token, "/", command);
        }
        else{
            snprintf(full_path, sizeof(full_path), "%s%s", token, command);
        }

        if(access(full_path, X_OK) == 0){
            free(temp_path);
            return strdup(full_path); //return address because it presents until the end of the command processing
        }

        token = strtok(NULL, ":");
    }
    free(temp_path);

    return 0;
}

int count_env_vars(char* env[]){
    int result = 0;
    while(env[result]){
        result += 1;
    }
    return result;
}

char** command_setenv(char* args[], char* env[]){
    if (args[1] == NULL) {
        printf("Usage:  setenv VAR=value\nor\tsetenv <variable> <value>\n");
        return env;
    }

    int env_count = count_env_vars(env);
    char** new_env = malloc((env_count + 2)* sizeof(char*)); //Add new variable and NULL
    if(!new_env){
        perror("malloc");
        return env;
    }

    for(int i = 0;i < env_count;i++){
        new_env[i] = strdup(env[i]); //copy string
        if(!new_env[i]){
            perror("strdup");
            for (int j = 0; j < i; j++) {
                free(new_env[j]);
            }
            free(new_env);
            return env;
        }
    }

    char* new_string = NULL;
    if(args[2] == NULL){
        new_string = strdup(args[1]);
            printf("%s\n", new_string);

        if(!new_string){
            perror("strdup");
            free(new_string);
            return env;
        }
    }
    else{
        char* new_string = malloc(strlen(args[1]) + strlen(args[2]));
        if(!new_string){
            perror("malloc");
            free(new_string);
            return env;
        }
        sprintf(new_string, "%s=%s", args[1], args[2]);
    }
    printf("%s\n", new_string);
    new_env[env_count] = new_string;
    new_env[env_count + 1] = NULL;

    return new_env;
}

char** command_unsetenv(char* args[], char* env[]){
    if(!args[1]){
        printf("Usage: unsetenv <variable>\n");
    }

    int env_count = count_env_vars(env);

    int index = -1;
    for(int i = 0;i < env_count;i++){
        if(strncmp(env[i], args[1], strlen(args[1])) == 0 && env[i][strlen(args[1])] == '='){
            index = i;
            break;
        }
    }

    if(index == -1){
        printf("Variable %s not found in environment\n", args[1]);
        return env;
    }

    free(env[index]);

    while(env[index] != NULL){
        env[index] =env[index + 1];
        index++;
    }

    env[env_count - 1] = NULL;

    return env;
}
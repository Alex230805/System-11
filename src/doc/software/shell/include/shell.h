#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_LENGHT 256
#define FALSE 0
#define TRUE 1


void shell_interface();
void parse_command();
char*get_string_pos(int pos);

#ifdef SHELL_IMPLEMENTATION

char command_list[][9] = {
    "ls",
    "mkdir",
    "touch",
    "cd",
    "cat"
};
char buffer[BUFFER_LENGHT];
char current_path[BUFFER_LENGHT];

void shell_interface(){
    int i = 0;
    bool break_input = FALSE;
    printf("%s : >>>: ", current_path);
    scanf("%s", buffer);
}

void parse_command(){
    int pos = 0;
    char * command = get_string_pos(0);
    while(strcmp(command, command_list[pos]) != 0){
        pos++;
    }

    switch(pos){
        case 0:
                command_ls();
                break;
        case 1:
                printf("World\n");
                break;

        default:
                printf("Wrong command\n");
                break;
    }
    return;
}


char* get_string_pos(int pos){
    char buffer_cpy[BUFFER_LENGHT];
    strcpy(buffer_cpy,buffer);
    char * std;
    std = strtok(buffer_cpy, " ");
    int i = 0;
    while(std != NULL && i < pos){
        if(i < pos){
            strtok(NULL," ");
        }
    }
    return std;
}

void command_ls(char*current_path){
    zenith_node node = zenith_node_navigation(current_path);
    
}

#endif

#endif

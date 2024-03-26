#include <stdio.h>
#include <stdlib.h>

#define SHELL_IMPLEMENTATION

#include "./include/shell.h"



int main(){

    printf("Start shell imp..\n");
    strcpy(current_path, "/");
    while(TRUE){
        shell_interface();
        parse_command();
        printf("\n");
    }
}



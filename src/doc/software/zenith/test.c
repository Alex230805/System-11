#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#define ZENITH_IMPLEMENTATION
#define Z_DEBUG

#include "./include/zenith_system.h"



int main(){
    FILE * testf;
    uint8_t *test = NULL;
    char * path = "./lorem_fold.txt";

    zenith_init_fs("ermagno test");
    zenith_get_info();
    zenith_mkdir("/", "test");
    zenith_mkdir("/usr", "test");
    zenith_mkdir("/home", "am");
    zenith_mkdir("/home/am", "documents");
    zenith_mkdir("/home/am/documents", "texts");

    if((testf = fopen(path,"r")) == NULL){
        printf("Error while opening file in %s: %s\n", path, strerror(errno));
        return 1;
    }
    test = (uint8_t*)malloc(sizeof(uint8_t) * (80*22));
    int counter = 0;
    char character = "A";
    while(fscanf(testf,"%c", &character) != EOF){
        test[counter] = character;
        counter++;
    }
    fclose(testf);
    zenith_write_f(test, "/", "lorem.txt");
    zenith_write_f(test, "/home/am", "lorem.txt");
    zenith_write_f(test, "/home/am/documents/texts", "lorem.txt");
    zenith_write_f(test, "/usr", "lorem_1.txt"); 
    zenith_write_f(test, "/usr", "lorem_2.txt"); 
    zenith_write_f(test, "/usr", "lorem_3.txt");
    zenith_get_used_space();
    zenith_write_f(test, "/usr", "lorem_3.txt");
    printf("content of lorem_3.txt in /usr\n");
    char* data = (char*)zenith_get_f("/usr", "lorem_3.txt");
    free(virtual_fs);
    return 0;
}

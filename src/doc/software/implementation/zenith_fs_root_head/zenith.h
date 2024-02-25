#ifndef ZENITH_H
#define ZENITH_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>



#define TRUE 1
#define FALSE 0

typedef struct znode{
    char * name;
    struct znode * next;  // next address
    char * value;               // actual array of data
    int has_next;               // flag
}zenith_node_file;


typedef struct zfolder{
    char * name;
    struct zfolder* folder_0;                // node -> may be just a file or other folder
    struct zfolder * folder_1;
    struct zfolder * folder_2;
    struct zfolder * folder_3;
    struct zfolder * folder_4;
    struct zfolder * folder_5;
    struct zfolder * folder_6;
    struct zfolder * folder_7;
}zenith_folder; 


typedef struct {
    char * name;
    uint8_t enable_flag;
    zenith_folder *folder_0;
    zenith_folder *folder_1;
    zenith_folder *folder_2;
    zenith_folder *folder_3;
    zenith_folder *folder_4;
    zenith_folder *folder_5;
    zenith_folder *folder_6;
    zenith_folder *folder_7;
    uint8_t lb_free_m;
    uint8_t hb_free_m;
    uint8_t xlb_free_m;
    uint8_t xhb_free_m;

    int * allocated_page;           // array to keep track of allocated pointer
    void * page_pointer;            // array to keep track of available pointer
}zenith_root_map;

zenith_root_map* create_partition(int size);
int zenith_mkdir(zenith_root_map * root, char * name, zenith_folder * current_pos,char * path);
zenith_folder* zenith_cd(zenith_root_map* root,zenith_folder* current_pos, char* path);
char* get_name_from_path(char*path, int iteration_number, char *separator);
zenith_folder* compare_dir_name(zenith_folder * dir_address, char * name);
int get_max_occurrences(char* name, char *separator);

#ifdef ZENITH_IMPLEMENTATION

zenith_root_map* create_partition(int size){
    zenith_root_map * root = (zenith_root_map*)malloc(sizeof(zenith_root_map));
    int * allocated_page = (int*)malloc(sizeof(int)*size-sizeof(zenith_root_map));
    void * page_pointer = (void*)malloc(sizeof(void)*size-sizeof(zenith_root_map));
    memcpy(allocated_page, 0,size-sizeof(zenith_root_map));
    memcpy(page_pointer, NULL,size-sizeof(zenith_root_map));
    root->folder_0 = NULL;
    root->folder_1 = NULL;
    root->folder_2 = NULL;
    root->folder_3 = NULL;
    root->folder_4 = NULL;
    root->folder_5 = NULL;
    root->folder_6 = NULL;
    root->folder_7 = NULL;

    root->name = (char*)malloc(sizeof(char)*9);
    strcpy(root->name, "/");
    root->enable_flag = TRUE;
    root->allocated_page = allocated_page;
    root->page_pointer = page_pointer;

    if(
        zenith_mkdir(root, "usr",(zenith_folder*)root ,"/") &&\
        zenith_mkdir(root, "share",(zenith_folder*)root, "/") &&\
        zenith_mkdir(root, "bin",(zenith_folder*)root, "/") &&\
        zenith_mkdir(root, "home",(zenith_folder*)root, "/") &&\
        zenith_mkdir(root, "etc",(zenith_folder*)root, "/") &&\
        zenith_mkdir(root, "lib",(zenith_folder*)root, "/")){

        return root;
    }
    return NULL;
}


int zenith_mkdir(zenith_root_map * root, char * name, zenith_folder * current_pos,char * path){

    int occurrences = 0;
    zenith_folder * folder = (zenith_folder*)malloc(sizeof(zenith_folder));
    zenith_folder* dir_address = zenith_cd(root,current_pos, path);
    folder->name = (char*)malloc(sizeof(char)*9);
    
    strcpy(folder->name,name);
    if(dir_address == NULL){
        return FALSE;
    }

    if(strcmp(name, dir_address->folder_0->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_1->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_2->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_3->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_4->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_5->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_6->name) == FALSE){
        occurrences+=1;
    }
    if(strcmp(name, dir_address->folder_7->name) == FALSE){
        occurrences+=1;
    }
    if(occurrences == 0){
        if(dir_address->folder_0 == NULL){
            dir_address->folder_0 = folder;
        }
        if(dir_address->folder_1 == NULL){
            dir_address->folder_1 = folder;
        }
        if(dir_address->folder_2 == NULL){
            dir_address->folder_2 = folder;
        }
        if(dir_address->folder_3 == NULL){
            dir_address->folder_3 = folder;
        }
        if(dir_address->folder_4 == NULL){
            dir_address->folder_4 = folder;
        }
        if(dir_address->folder_5 == NULL){
            dir_address->folder_5 = folder;
        }
        if(dir_address->folder_6 == NULL){
            dir_address->folder_6 = folder;
        }
        if(dir_address->folder_7 == NULL){
            dir_address->folder_7 = folder;
        }
    }else{
        return FALSE;
    }
    return TRUE;
}

zenith_folder* zenith_cd(zenith_root_map* root,zenith_folder* current_pos, char* path){

    if(strlen(path) == 1){
        return root;
    }else{
        char * name = get_name_from_path(path, 1, "/");
        int it = 1;
        int max_occurrences = get_max_occurrences(path, "/");
        while(compare_dir_name(current_pos,name) != NULL && it != max_occurrences){
            name = get_name_from_path(path, it, "/");
            current_pos = compare_dir_name(current_pos,name);
            it+=1;
        }
        return current_pos; 
    }
}

char* get_name_from_path(char*path, int iteration_number, char *separator){
    char name[16];
    int pos = 0;

    if(path[0] != "/"){
        strcat(name, "./");
    }

    for(int i=0;i<strlen(path) && iteration_number != 0;i++){
        if(path[i] == separator){
            pos = i+1;
            iteration_number -= 1;
        }
        if(i==0 && path[i] == "." && path[i+1] == "/"){
            pos = i+1;
            iteration_number -= 1;
        }
    }

    int i=0;
    while(path[pos+i] != separator){
        name[i] = path[pos+i];
        i++;
    }
    strcat(name,"/");
    return name;
}

zenith_folder* compare_dir_name(zenith_folder * dir_address, char * name){
    zenith_folder * obtained_address = NULL;
    if(strcmp(name, dir_address->folder_0->name) == TRUE){
        obtained_address = dir_address->folder_0;
    }
    if(strcmp(name, dir_address->folder_1->name) == TRUE){
        obtained_address = dir_address->folder_1;
    }
    if(strcmp(name, dir_address->folder_2->name) == TRUE){
        obtained_address = dir_address->folder_2;
    }
    if(strcmp(name, dir_address->folder_3->name) == TRUE){
        obtained_address = dir_address->folder_3;
    }
    if(strcmp(name, dir_address->folder_4->name) == TRUE){
        obtained_address = dir_address->folder_4;
    }
    if(strcmp(name, dir_address->folder_5->name) == TRUE){
        obtained_address = dir_address->folder_5;
    }
    if(strcmp(name, dir_address->folder_6->name) == TRUE){
        obtained_address = dir_address->folder_6;
    }
    if(strcmp(name, dir_address->folder_7->name) == TRUE){
        obtained_address = dir_address->folder_7;
    }
    return obtained_address;
}

int get_max_occurrences(char* name, char *separator){
    int max_occurrences = 0;

    if(name[0] != "/"){
        max_occurrences+=1;
    }
    for(int i=0;i<strlen(name);i++){
        if(name[i] == "/"){
            max_occurrences++;
        }
    }
    return max_occurrences;
}

#endif

#endif
#ifndef ZENITH_H
#define ZENITH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>



#define SIZE 1048576
#define MAX_CAPACITY (SIZE / 256)

#define __ZENITH_NODE__  0x20
#define __ZENITH_DATA_NODE__ 0x40

#define __DATA_SIZE__ (sizeof(zenith_data))
#define __NODE_SIZE__ (sizeof(zenith_node))
#define __ROOT_SIZE__ (sizeof(root))

#define __DATA_MASK__ 0x20
#define __NODE_MASK__ 0x40
#define __ROOT_MARK__ 0x80

#define __NODE_NESTING_COMPLETE__ (0x00 | __NODE_MASK__)
#define __NODE_NESTING_FAILED__ (0x01 | __NODE_MASK__)
#define __NODE_FOLDER_FREE_NAME__ (0x02 | __NODE_MASK__)

#define __DATA_NESTING_COMPLETE__ (0x00 | __DATA_MASK__)
#define __DATA_NESTING_FAILED__ (0x01 | __DATA_MASK__)
#define __DATA_READING_COMPLETE__ (0x02 | __DATA_MASK__)
#define __DATA_READING_FAILED__ (0x03 | __DATA_MASK__)


#define __ZENITH_VERSION__ "zenith_1.0"

#define FALSE 0
#define TRUE 1

#define NAME_LENGHT 16
#define ELEMENT_LENGHT (((256 - NAME_LENGHT) / sizeof(uint32_t))-sizeof(uint32_t)*2)
#define DATA_LENGHT (ELEMENT_LENGHT - (sizeof(uint32_t)*2) - (sizeof(uint8_t)*2))


typedef struct{
    char name[NAME_LENGHT];
    uint32_t * adr;
    uint32_t type_t;
    uint8_t data[DATA_LENGHT];
    uint8_t contiguous_flag;
    uint8_t page_lenght;
    uint32_t * next;
}zenith_data;

typedef struct{
    char name[NAME_LENGHT];
    uint32_t * adr;
    uint32_t type_t;
    uint32_t * element[ELEMENT_LENGHT];
}zenith_node;

typedef struct{
    char fs_name[NAME_LENGHT];
    char fs_version[NAME_LENGHT];
    char name[NAME_LENGHT];
    uint32_t total_size;
    uint32_t used_space;
    uint32_t * page_pointer[MAX_CAPACITY];
    int page_allocated[MAX_CAPACITY];
    uint32_t * folder[ELEMENT_LENGHT];
}root;

uint32_t * virtual_fs;


uint32_t*  zenith_malloc(uint8_t type, char*name);
void zenith_init_fs(char*name);
void zenith_free(uint32_t *pointer);
void zenith_mkdir(char* path, char*name);
zenith_node zenith_node_navigation(char*path);
bool zenith_check_name(zenith_node *node,char*name);
int zenith_write_f(uint8_t*file, char*path, char*name);
uint8_t* zenith_get_f(char*path, char*name);
void zenith_get_info();
void zenith_get_used_space();
int zenith_insert_node(uint32_t *adr, char*path);
bool zenith_check_folder_cont(char*path,char*name);



#ifdef ZENITH_IMPLEMENTATION


void zenith_get_info(){
    root fs_tab;
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    printf("Fs Name: %s\n", fs_tab.fs_name);
    printf("Fs Version: %s\n", fs_tab.fs_version);
    printf("Root name: %s\n", fs_tab.name);
    printf("Total partition space: %d\n", fs_tab.total_size);
    printf("Used space: %d\n", fs_tab.used_space);
}

void zenith_get_used_space(){
    root fs_tab;
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    printf("Used space: %d\n", fs_tab.used_space);
}

uint8_t* zenith_get_f(char*path, char*name){
    zenith_node node = zenith_node_navigation(path);  
    zenith_data data;
    uint8_t*buffer = NULL;
    int pos = 0;
    if(zenith_check_folder_cont(path,name) != TRUE){
        return buffer;
    }
    memcpy(&data,node.element[pos], __DATA_SIZE__);
    while(strcmp(data.name,name) != 0){
        if(pos<ELEMENT_LENGHT){
            pos+=1;
            memcpy(&data,node.element[pos], __DATA_SIZE__);

        }
    }

    if(!data.contiguous_flag && data.type_t == __ZENITH_DATA_NODE__){
        pos = 0;
        buffer = (uint8_t*)malloc(sizeof(uint8_t)* DATA_LENGHT);
        while(data.data[pos] != "\0" && pos < DATA_LENGHT){
            buffer[pos] = data.data[pos];
        }
    }else if(data.type_t == __ZENITH_DATA_NODE__){
        pos = 0;
        bool end = FALSE;
        buffer = (uint8_t*)malloc(sizeof(uint8_t)*(data.page_lenght*256));
        while(end == FALSE){
            for(int i = 0; i < DATA_LENGHT && data.data[i] != "\0";i++){
                buffer[pos] = data.data[i];
                pos+=1;
            }
            if(data.contiguous_flag != FALSE){
                memcpy(&data, data.next,__DATA_SIZE__);
            }else{
                end = TRUE;
            }
        }
    }
    return buffer;
}

int zenith_write_f(uint8_t*file, char*path,char*name){
    int size = strlen((char*)file);
    uint32_t *data = zenith_malloc(__ZENITH_DATA_NODE__, name);    
    uint32_t *c_data;

    zenith_data data_header;
    zenith_data move_data;

    size_t counter = 0;
    root fs_tab;

    int mem_block_num_mod = size%DATA_LENGHT;
    int mem_block_num = (int)size/DATA_LENGHT;

    memcpy(&data_header, data, __DATA_SIZE__);
    memcpy(&fs_tab,virtual_fs, __ROOT_SIZE__);
    
    // check name

    if(zenith_check_folder_cont(path, name) == TRUE){
        printf("Write error: a file named the same way already exist!\n");
        zenith_free(data);
        return __DATA_NESTING_FAILED__;       
    }
    // creating data structure 
    if(size > DATA_LENGHT){
        int mem_block_counter = 0;
        size_t file_p = 0;
        bool end = FALSE;
        data_header.contiguous_flag = 0xFF;    

        if(mem_block_num_mod > 0){
            mem_block_num+=1;
        }
        
        for(size_t i=0;i<DATA_LENGHT;i++){
            data_header.data[i] = file[file_p];
            file_p+=1;
        }
        data_header.page_lenght = mem_block_num;
        memcpy(data_header.adr,&data_header ,__DATA_SIZE__);

        while(mem_block_counter < mem_block_num){

            c_data = zenith_malloc(__ZENITH_DATA_NODE__, name);
            memcpy(&move_data, c_data, __DATA_SIZE__);


            for(size_t i = 0;i<DATA_LENGHT && !end;i++){
                move_data.data[i] = file[file_p];
                if(file_p==size){
                    end = TRUE;
                }
                file_p+=1;
            }

            if(mem_block_counter+1 == mem_block_num){
                move_data.contiguous_flag = 0x00;
            }else{
                move_data.contiguous_flag = 0xFF;
            }

            memcpy(c_data,&move_data, __DATA_SIZE__);
            move_data = data_header;
            while(move_data.next !=NULL){
                memcpy(&move_data,move_data.next, __DATA_SIZE__);
            }
            move_data.next = c_data;
            memcpy(move_data.adr,&move_data,__DATA_SIZE__);
            mem_block_counter+=1;
        }
            
    }else{
        for(size_t i=0;i<DATA_LENGHT;i++){
                data_header.data[i] = file[i];
        }
        data_header.page_lenght = 1;
    }
    memcpy(data_header.adr,&data_header,__DATA_SIZE__);

    // write information after the data array has been composed
    
    if(zenith_insert_node(data,path) == __NODE_NESTING_COMPLETE__){
        return __DATA_NESTING_COMPLETE__;
    }
    return __DATA_NESTING_FAILED__;
}

uint32_t *zenith_malloc(uint8_t type, char*name){
    zenith_node node;
    zenith_data data;
    root fs_tab;    
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    uint32_t * pointer = NULL;
    size_t i = 0;
    switch(type){
        case __ZENITH_DATA_NODE__:
            strcpy(data.name, name);
            data.contiguous_flag = 0x00;
            data.next = NULL;

            while(fs_tab.page_allocated[i] != 0x00){
                i++;
            }
            pointer = fs_tab.page_pointer[i];
            fs_tab.page_allocated[i] = 0xFF;
            fs_tab.used_space += 256;
            data.adr = pointer;
            data.page_lenght = 0;
            data.type_t = __ZENITH_DATA_NODE__;
            memcpy(&virtual_fs[i*256],&data ,__DATA_SIZE__);
            memcpy(virtual_fs,&fs_tab,__ROOT_SIZE__);
            break;
        case __ZENITH_NODE__:
            strcpy(node.name,name);
            for(size_t i = 0; i<ELEMENT_LENGHT; i++){
                node.element[i] = 0x00;
            }

            while(fs_tab.page_allocated[i] != 0x00){
                i++;
            }
            pointer = fs_tab.page_pointer[i];
            fs_tab.page_allocated[i] = 0xFF;
            fs_tab.used_space += 256;
            node.adr = pointer;
            node.type_t = __ZENITH_NODE__;
            memcpy(&virtual_fs[i*256],&node, __NODE_SIZE__);
            memcpy(virtual_fs,&fs_tab ,__ROOT_SIZE__);
            break;

    }
    return pointer;

}

void zenith_free(uint32_t *pointer){
    root fs_tab;
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    for(size_t i = 0; i< MAX_CAPACITY;i++){
        if(fs_tab.page_pointer[i] == pointer){
            fs_tab.page_allocated[i] = 0x00;
            fs_tab.used_space -= 256;
        }
    }
    memcpy(virtual_fs, &fs_tab, __ROOT_SIZE__);
}

void zenith_mkdir(char* path, char*name){
    root fs_tab;
    uint32_t * folder_pointer = zenith_malloc(__ZENITH_NODE__, name);
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    if(zenith_check_folder_cont(path,name) == TRUE){
        printf("Write folder error: a file named the same way already exist!\n");
        zenith_free(folder_pointer);
        return;
    }
    if(zenith_insert_node(folder_pointer,path) == __NODE_NESTING_COMPLETE__){
        return;
    }
    printf("Nesting error: unable to nest a new folder!\n");
    
}

int zenith_insert_node(uint32_t *adr, char*path){
    zenith_node node;
    root fs_tab;
    node = zenith_node_navigation(path);
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    bool end_insert = FALSE;

   if(strlen(path) == 1){ 
        for(size_t i=0;i<ELEMENT_LENGHT && !end_insert;i++){
            if(fs_tab.folder[i] == NULL){
                fs_tab.folder[i] = adr;
                end_insert = TRUE;
            }
        }

        memcpy(virtual_fs,&fs_tab,__ROOT_SIZE__);
        return __NODE_NESTING_COMPLETE__;
 
    }else{
        for(size_t i = 0 ;i < ELEMENT_LENGHT && !end_insert;i++){
            if(node.element[i] == NULL){
                node.element[i] = adr;
                end_insert = TRUE;
            }
        }
        memcpy(node.adr,&node,__NODE_SIZE__);
    }
   return __NODE_NESTING_COMPLETE__;
}

zenith_node zenith_node_navigation(char*path){
    zenith_node nav_node;
    zenith_node f_node;
    zenith_node node;


    char *sep = "/";
    char path_c[256];
    strcpy(path_c,path);
    char * token = strtok(path_c,sep);
    bool root_navigation = TRUE, break_flag = FALSE;
    int path_size = strlen(path);
    root fs_tab;
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);

    if(path_size > 1){
        while(token != NULL){
            break_flag = FALSE;
            if(root_navigation){
                for(size_t i = 0; i < ELEMENT_LENGHT && !break_flag;i++){
                    if(fs_tab.folder[i] != NULL){
                        memcpy(&node, fs_tab.folder[i], __NODE_SIZE__);
                        if(strcmp(node.name, token) == 0 && node.type_t == __ZENITH_NODE__){
                            nav_node = node;
                            root_navigation = FALSE;
                            break_flag = TRUE;
                        }
                    }
                }
            }else{
                for(size_t i = 0; i < ELEMENT_LENGHT && !break_flag;i++){
                    if(nav_node.element[i]!= NULL){
                        memcpy(&f_node, nav_node.element[i], __NODE_SIZE__);
                        if(strcmp(f_node.name, token) == 0  && f_node.type_t == __ZENITH_NODE__){
                            nav_node = f_node;
                            break_flag = TRUE;
                        }
                    }

                }

            }
            token = strtok(NULL, sep);
        }
    }
    return nav_node;
}

bool zenith_check_folder_cont(char*path,char*name){
    root fs_tab;
    zenith_node move_data;
    int counter = 0;
    memcpy(&fs_tab, virtual_fs, __ROOT_SIZE__);
    if(strlen(path)>1){
        zenith_node node = zenith_node_navigation(path);
        if(zenith_check_name(&node,name) == TRUE){
            return TRUE;
        }
    }else{
        for(size_t i = 0; i< ELEMENT_LENGHT;i++){
            if(fs_tab.folder[i]!= NULL){
                memcpy(&move_data, &fs_tab.folder[i], __DATA_SIZE__);
                if(strcmp(move_data.name, name) == 0){
                    counter+=1;
                }
            }
        }
        if(counter!=0){
            return TRUE;
        }
    }
    return FALSE;
}

bool zenith_check_name(zenith_node *node,char*name){
    int counter = 0;
    zenith_node c_node;
    bool end = FALSE;
    for(size_t i = 0;i < ELEMENT_LENGHT && end != TRUE;i++){
        if(node->element[i] != NULL){
            memcpy(&c_node, node->element[i],__NODE_SIZE__);
            if(strcmp(c_node.name, name) == 0){
                counter+=1;
                end = TRUE;
            }
        }
    }
    if(counter>0){
        return TRUE;
    }
    return FALSE;
}

void zenith_init_fs(char*name){
    virtual_fs = (uint32_t*)malloc(sizeof(uint32_t) * SIZE);

    root fs_tab;
    strcpy(fs_tab.name, "/");
    fs_tab.total_size = SIZE;
    strcpy(fs_tab.fs_name, name);
    strcpy(fs_tab.fs_version, __ZENITH_VERSION__);
    int root_size = 0;

    for(size_t i = 0;i < MAX_CAPACITY; i++){
        fs_tab.page_pointer[i] = &virtual_fs[i*256];
    }
    for(size_t i = 0;i < MAX_CAPACITY; i++){
        fs_tab.page_allocated[i] = 0x00;
    }

    for(size_t i = 0;i < ELEMENT_LENGHT;i++){
        fs_tab.folder[i] = NULL;
    }
    
    for(size_t i = 0;i < __ROOT_SIZE__/256; i++){
        fs_tab.page_allocated[i] = 0xFF;
    }
    fs_tab.used_space = __ROOT_SIZE__;

    memcpy(virtual_fs,&fs_tab, sizeof(root));
    uint32_t * node_pointer;
    node_pointer = zenith_malloc(__ZENITH_NODE__, "usr");
    memcpy(&fs_tab, virtual_fs, sizeof(root));
    fs_tab.folder[0] = node_pointer;
    memcpy(virtual_fs,&fs_tab ,sizeof(root));
    node_pointer = zenith_malloc(__ZENITH_NODE__, "share");
    memcpy(&fs_tab, virtual_fs, sizeof(root));
    fs_tab.folder[1] = node_pointer;
    memcpy(virtual_fs,&fs_tab ,sizeof(root));
    node_pointer = zenith_malloc(__ZENITH_NODE__, "etc");
    memcpy(&fs_tab, virtual_fs, sizeof(root));
    fs_tab.folder[2] = node_pointer;
    memcpy(virtual_fs,&fs_tab ,sizeof(root));
    node_pointer = zenith_malloc(__ZENITH_NODE__, "bin");
    memcpy(&fs_tab, virtual_fs, sizeof(root));
    fs_tab.folder[3] = node_pointer;
    memcpy(virtual_fs,&fs_tab ,sizeof(root));
    node_pointer = zenith_malloc(__ZENITH_NODE__, "home");
    memcpy(&fs_tab, virtual_fs, sizeof(root));
    fs_tab.folder[4] = node_pointer; 
    memcpy(virtual_fs,&fs_tab ,sizeof(root));
}



#endif
#endif

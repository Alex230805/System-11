#ifndef ZENITH_H
#define ZENITH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#define MAX_CAPACITY 4096

#define __Z_DEFAULT_USER_PERM__ 0b10000000
#define __Z_USER_READ_PERM__ 0b100000001
#define __Z_USER_WRITE_PERM__ 0b10000010
#define __Z_USER_EXECUTE_PERM__ 0b1000100

#define __Z_SUPER_PERM__ 0b10000000
#define __Z_SUPER_WRITE_PERM__ 0b10001000
#define __Z_SUPER_READ_PERM__ 0b10010000
#define __Z_SUPER_EXECUTE_PERM__ 0b11000000

#define __SUPER_USER__ 0xF0
#define __USER__ 0x0F

#define __ZENITH_NODE__  0x20
#define __ZENITH_DATA_NODE__ 0x40
#define __ZENITH_FOLDER___ 0x80


#define __ZENITH_INIT_COMPLETE__ 0xAF
#define __ZENITH_LOW_LEVEL_SWAP__ 0xBE
#define __ZENITH_NESTING_FAILED__ 0xea
#define __ZENITH_NESTING_FINISHED__ 0xde

#define FALSE 0
#define TRUE 1



/**

#define ZENITH_IMPLEMENTATION

    - init_node fn
    - init_data_node fn
    - init_folder fn
    - zenith_malloc fn
    - zenith_flush_fstab fn

#define ZENITH_MKFS_IMPLEMENTATION

    - zenith_init_fs fn
    - zenith_init_root_dir fn

Zenith init with low level swap:

Include the argument "__ZENITH_LOW_LEVEL_SWAP__" in zenith_init_fs function
to erase all data in the phisical drive

zenith_init_fs(dim,__ZENITH_LOW_LEVEL_SWAP__) -> overwrite all data with 0 and generate partition table
zenith_init_fs(dim) -> generate partition table


#define ZENITH_NAVIGATOR_IMPLEMENTATION

    - zenith_insert_folder
    - zenith_init_root_dir

**/


typedef struct z_folder{
    struct z_folder* next;   // next concatenated folder -> all part of the same node 
    void * node;            // the pointer data of the folder inside content
}z_folder;

typedef struct data_node{
    int user_perm;
    char name[16];
    uint8_t data_array[235];
    struct data_node* next_node;
    struct data_node* contiguous_data;
    int contiguous_flag;          // have next flag
}data_node;

typedef struct zenith_node{
    int user_perm;      // user perm
    char name[16];          // node name
    z_folder * folder;      // folder pointer list
    data_node * data;    // data poitner list
    struct zenith_node * prev;     // previous node
}zenith_node;

typedef struct{
    char version_name[8];       // version name 
    int total_size;             // total size ( in Bytes)
    int free_pages;             // free pages (page = 256b)
    int used_space;
    zenith_node * first_node;   // first node pointer
    void *page_pointer[MAX_CAPACITY];   // pointer to page
    int page_allocated[MAX_CAPACITY];   // page flag 
}root;


uint8_t* drive_space;           // drive space to simulate
int global_root_size;


#ifdef ZENITH_IMPLEMENTATION

    void* zenith_malloc(root * fs_tab,uint8_t type, int user_type);
    void init_node(zenith_node * node, char * name, int type);
    void init_data_node(data_node * data, char * name, int type);
    void init_folder(z_folder * folder);
    void zenith_flush_fstab(root*fstab, int size);

#endif

#ifdef ZENITH_MKFS_IMPLEMENTATION

    int zenith_init_fs(int size, ...);

#endif

#ifdef ZENITH_NAVIGATOR_IMPLEMENTATION

    void zenith_insert_folder(root*fs_tab,char* path, z_folder * folder);
    int zenith_init_root_dir(root * fs_tab);

#endif


#ifdef ZENITH_IMPLEMENTATION


void* zenith_malloc(root * fs_tab,uint8_t type, int user_type){
    int i=0;
    
    z_folder folder;
    data_node data;
    zenith_node node;

    void* pointer = NULL;
    int dim = 0;

    switch(type){
        case __ZENITH_NODE__:
            init_node(&node, "folder/",user_type);

            while(fs_tab->page_allocated[i] != 0x00){
                i++;
            }
            dim = sizeof(zenith_node);
            pointer = (void*)fs_tab->page_pointer[i];
            memcpy(pointer, &node, dim);
            fs_tab->free_pages -=1;
            fs_tab->used_space +=1;
            break;

        case __ZENITH_DATA_NODE__:
            init_data_node(&data, "file", user_type);
            while(fs_tab->page_allocated[i] != 0x00){
                i++;
            }
            dim = (int)sizeof(data_node);
            pointer = (void*)fs_tab->page_pointer[i];
            memcpy(pointer, &data, dim);
            fs_tab->free_pages -=1;
            fs_tab->used_space +=1;
            break;

        case __ZENITH_FOLDER___:
            init_folder(&folder);
            while(fs_tab->page_allocated[i] != 0x00){
                i++;
            }
            dim = sizeof(folder);
            pointer = (void*)fs_tab->page_pointer[i];
            memcpy(pointer, &folder, dim);
            fs_tab->free_pages -=1;
            fs_tab->used_space +=1;
            break;
    }
    fs_tab->page_allocated[i] = 0xff;
    return pointer;
}

void init_node(zenith_node * node, char * name, int type){
    switch(type){
        case __SUPER_USER__:
            node->user_perm = __Z_SUPER_PERM__+__Z_SUPER_READ_PERM__+__Z_SUPER_WRITE_PERM__;
            break;
        case __USER__:
            node->user_perm = __Z_DEFAULT_USER_PERM__+__Z_USER_READ_PERM__+__Z_USER_WRITE_PERM__;
            break;
        default:
            break;
    }
    strcpy(node->name,name);
    node->folder = NULL;
    node->data = NULL;
    node->prev = NULL;
    return;
}

void init_data_node(data_node * data, char * name, int type){
    switch(type){
        case __SUPER_USER__:
            data->user_perm = __Z_SUPER_PERM__+__Z_SUPER_READ_PERM__+__Z_SUPER_WRITE_PERM__;
            break;
        case __USER__:
            data->user_perm = __Z_DEFAULT_USER_PERM__+__Z_USER_READ_PERM__+__Z_USER_WRITE_PERM__;
            break;
        default:
            break;
    }
    strcpy(data->name,name);
    data->next_node = NULL;
    data->contiguous_data = NULL;
    data->contiguous_flag = 0X00;
    return;
}

void init_folder(z_folder * folder){
    folder->next = NULL;
    folder->node = NULL;
    return;
}


void zenith_flush_fstab(root * fs_tab, int size){
    memcpy(drive_space, fs_tab, size);
    return;
}

#ifdef ZENITH_NAVIGATOR_IMPLEMENTATION

void zenith_insert_folder(root*fs_tab,char* path, z_folder * folder){

    int lenght_string = strlen(path);
    z_folder * moving_folder = fs_tab->first_node->folder;
    int control_flag = FALSE;

    if(lenght_string > 1){

        char * token = strtok(path, "/");
        int lenght_token = strlen(token);
        for(int i=0;i<lenght_token;i++){
            control_flag = FALSE;
            while(moving_folder->next != NULL && control_flag != TRUE){
                zenith_node * cache_node = (zenith_node*)moving_folder->node;
                if(strcmp(cache_node->name, &token[i]) == 0){
                    moving_folder = cache_node->folder;
                    control_flag = TRUE;
                }else{
                    moving_folder = moving_folder->next;
                }
            }
        }

        while(moving_folder->next != NULL){
            moving_folder = moving_folder->next;
        }
        moving_folder->next = folder;

        
    }else{
        printf("i'm here");
        while(moving_folder->next != NULL){
            moving_folder = moving_folder->next;
        }
        moving_folder->next = folder;
    }
}

int zenith_init_root_dir(root * fs_tab){
    zenith_node * node_0 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_0 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER___, __SUPER_USER__);
    strcpy(node_0->name, "usr");
    folder_0->node = node_0;

    zenith_node * node_1 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_1 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER___, __SUPER_USER__);
    strcpy(node_1->name, "share");
    folder_1->node = node_1;

    zenith_node * node_2 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_2 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER___, __SUPER_USER__);
    strcpy(node_2->name, "lib");
    folder_2->node = node_2;


    zenith_node * node_3 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_3 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER___, __SUPER_USER__);
    strcpy(node_3->name, "etc");
    folder_3->node = node_3;

    zenith_node * node_4 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_4 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER___, __SUPER_USER__);
    strcpy(node_4->name, "home");
    folder_4->node = node_4;

    zenith_node * node_5 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_5 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER___, __SUPER_USER__);
    strcpy(node_5->name, "bin");
    folder_5->node = node_5;

    zenith_insert_folder(fs_tab,"/", folder_0);
    zenith_insert_folder(fs_tab,"/", folder_1);
    zenith_insert_folder(fs_tab,"/", folder_2);
    zenith_insert_folder(fs_tab,"/", folder_3);
    zenith_insert_folder(fs_tab,"/", folder_4);
    return 0;
}


#endif

#ifdef ZENITH_MKFS_IMPLEMENTATION

int zenith_init_fs(int size, ...){
    drive_space = (uint8_t*)malloc(sizeof(uint8_t)*size);     // define a space
    va_list ptr;

    va_start(ptr,1);

    if(va_arg(ptr,int) == __ZENITH_LOW_LEVEL_SWAP__){
        printf("\nDeep swap!\n");
        memset(drive_space,0, size);
    }

    va_end(ptr);

    root fs_table;                                  // create fs table

    strcpy(fs_table.version_name,"zth_1.0");      // copy name
    fs_table.total_size = size;                     // write size
    fs_table.free_pages = size/256;                   // write free pages
    fs_table.used_space = 0;
    fs_table.first_node = NULL;                     // link first node to NULL

    // copy address for pages
    int j = 0;
    for(int i=0;i<MAX_CAPACITY;i++){                       
        fs_table.page_pointer[i] = &drive_space[j];
        j+=256;
    }

    // set flags to 0
    for(int i=0;i<MAX_CAPACITY;i++){
        fs_table.page_allocated[i] = FALSE;
    }

    int root_size = (int)sizeof(root)/256;
    float extra_page_root_size = (sizeof(root)/256 - root_size)*10;
    if(extra_page_root_size > 0){
        root_size+=1;
    }

    for(int i=0;i<root_size;i++){
        fs_table.page_allocated[i] = 0xff;
    }
    global_root_size = root_size*256;

    zenith_node * node = (zenith_node*)zenith_malloc(&fs_table,__ZENITH_NODE__,__SUPER_USER__);
    z_folder * folder = (z_folder*)zenith_malloc(&fs_table,__ZENITH_FOLDER___,__SUPER_USER__);

    node->folder = folder;
    fs_table.first_node = node;
    strcpy(node->name,"/");

    fs_table.free_pages = fs_table.free_pages - root_size ;
    fs_table.used_space = root_size;

    zenith_flush_fstab(&fs_table, global_root_size);
    

    return __ZENITH_INIT_COMPLETE__;
}



#endif

#endif
#endif
#ifndef ZENITH_H
#define ZENITH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

#define MAX_CAPACITY 256

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

typedef struct z_folder{
    struct z_folder* next;   // next concatenated folder -> all part of the same node 
    void * node;            // the pointer data of the folder inside content
}z_folder;

typedef struct data_node{
    uint8_t user_perm;
    char name[8];
    uint8_t data_array[240];
    struct data_node* next_node;
    struct data_node* contiguous_data;
    uint8_t contiguous_flag;          // have next flag
}data_node;

typedef struct zenith_node{
    uint8_t user_perm;      // user perm
    char name[8];          // node name
    z_folder * folder;      // folder pointer list
    data_node * data;    // data poitner list
    struct zenith_node * prev;     // previous node
}zenith_node;

typedef struct{
    char version_name[8];       // version name 
    uint8_t total_size;             // total size ( in Bytes)
    uint8_t free_pages;             // free pages (page = 256b)
    uint8_t used_space;
    zenith_node * first_node;   // first node pointer
    void *page_pointer[MAX_CAPACITY];   // pointer to page
    uint8_t page_allocated[MAX_CAPACITY];   // page flag 
}root;


uint8_t * drive_space;           // drive space to simulate

int zenith_init_fs(int size);
void* zenith_malloc(root * fs_tab,uint8_t type, int user_type);
void init_node(zenith_node * node, char * name, int type);
void init_data_node(data_node * data, char * name, int type);
void init_folder(z_folder * folder);

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
            break;

        case __ZENITH_DATA_NODE__:
            init_data_node(&data, "new_file", user_type);
            while(fs_tab->page_allocated[i] != 0x00){
                i++;
            }
            dim = sizeof(data_node);
            pointer = (void*)fs_tab->page_pointer[i];
            memcpy(pointer, &data, dim);
            break;

        case __ZENITH_FOLDER___:
            init_folder(&folder);
            while(fs_tab->page_allocated[i] != 0x00){
                i++;
            }
            dim = sizeof(folder);
            pointer = (void*)fs_tab->page_pointer[i];
            memcpy(pointer, &folder, dim);
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
    data->contiguous_flag = 0x00;
    return;
}

void init_folder(z_folder * folder){
    folder->next = NULL;
    folder->node = NULL;
    return;
}

#ifdef ZENITH_MKFS_IMPLEMENTATION

int zenith_init_fs(int size){
    drive_space = (uint8_t*)malloc(sizeof(uint8_t)*size);     // define a space
    memcpy(drive_space,NULL,sizeof(uint8_t)*size);            // initialize it

    root fs_table;                                  // create fs table

    strcpy(fs_table.version_name,"zth_1.0");      // copy name
    fs_table.total_size = size;                     // write size
    fs_table.free_pages = size/8;                   // write free pages
    fs_table.used_space = 0;
    fs_table.first_node = NULL;                     // link first node to NULL

    // copy address for pages
    int j = 0;
    for(int i=0;i<size;i++){                       
        fs_table.page_pointer[i] = &drive_space[j];
        j+=256;
    }

    // set flags to 0
    for(int i=0;i<size;i++){
        fs_table.page_allocated[i] = 0x00;
    }

    int root_size = 0;
    int root_size_page = (int)sizeof(root) / 256*10 % 10;   // amount of pages used by the fs tab
    if(root_size_page > 0){
        root_size+=1;
    }
    root_size += sizeof(root) /256;

    for(int i=0;i<root_size;i++){
        fs_table.page_allocated[i] = 0xff;
    }
    
    zenith_node * node = (zenith_node*)zenith_malloc(&fs_table,__ZENITH_NODE__,__SUPER_USER__);
    z_folder * folder = (z_folder*)zenith_malloc(&fs_table,__ZENITH_FOLDER___,__SUPER_USER__);

    node->folder = folder;
    strcpy(node->name,"/");
    memcpy(drive_space,&fs_table,sizeof(root));

    return __ZENITH_INIT_COMPLETE__;
}

#endif

#endif
#endif
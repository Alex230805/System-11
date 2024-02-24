#ifndef ZENITH_H
#define ZENITH_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>


typedef struct{
    char * name;
    struct zenith_node * next;  // next address
    char * value;               // actual array of data
    int has_next;               // flag
}zenith_node_file;


typedef struct{
    char * name;
    void * node;                // node -> may be just a file or other folder
    void * node;
    void * node;
    void * node;
    void * node;
    void * node;
    void * node;
    void * node;
}zenith_folder; 


typedef struct{
    char * zenith_version;
    uint8_t enable_flag;
    struct * zenith_folder folder_0;
    struct * zenith_folder folder_1;
    struct * zenith_folder folder_2;
    struct * zenith_folder folder_3;
    struct * zenith_folder folder_4;
    struct * zenith_folder folder_5;
    struct * zenith_folder folder_6;
    struct * zenith_folder folder_7;
    uint8_t lb_free_m;
    uint8_t hb_free_m;
    uint8_t xlb_free_m;
    uint8_t xhb_free_m;

    int * allocated_page;           // array to keep track of allocated pointer
    void * page_pointer;            // array to keep track of available pointer
}zenith_root_map;

void* create_partition(void * memory_chunk_head, size_t size);

#ifdef ZENITH_IMPLEMENTATION

void* create_partition(void * memory_chunk_head, size_t size){
    int page_count = size/8;
    int allocated_page[page_count];
    void * page_pointer[page_count];
    struct zenith_root_map zenith_root;
    size_t base_sturct_size = sizeof(zenith_root_map);
    char * name = "1.0";
    int free_size = sizeof(size_t)*(size-base_sturct_size);

    memory_chunk_head = (void*)malloc(sizeof(size_t)*size);

    for(int i=0;i<page_count;i++){
        allocated_page[i] = 0;
    }

    for(int i=0;i<page_count;i++){
        page_pointer[i] = &memory_chunk_head[i];
    }

    zenith_root.allocated_page = allocated_page;
    zenith_root.page_pointer = page_pointer;

    zenith_root.folder_0 = (zenith_folder)page_pointer[16];
    zenith_root.folder_1 = (zenith_folder)page_pointer[17];
    zenith_root.folder_2 = (zenith_folder)page_pointer[18];
    zenith_root.folder_3 = (zenith_folder)page_pointer[19];
    zenith_root.folder_4 = (zenith_folder)page_pointer[20];
    zenith_root.folder_5 = (zenith_folder)page_pointer[21];
    zenith_root.folder_6 = (zenith_folder)page_pointer[22];
    zenith_root.folder_7 = (zenith_folder)page_pointer[23];


    allocated_page[16] = 0xff;
    allocated_page[17] = 0xff;
    allocated_page[18] = 0xff;
    allocated_page[19] = 0xff;
    allocated_page[20] = 0xff;
    allocated_page[21] = 0xff;
    allocated_page[22] = 0xff;
    allocated_page[23] = 0xff;

    strcpy(zenith_root.zenith_version, name);
    zenith_root.enable_flag = 0xff;


    // just for now
    zenith_root.lb_free_m = 0x00;
    zenith_root.hb_free_m = 0x00;
    zenith_root.xlb_free_m = 0x00;
    zenith_root.xlb_free_m = 0x00;

    memcpy(memory_chunk_head, zenith_root, base_sturct_size);
    return memory_chunk_head;
}



#endif

#endif
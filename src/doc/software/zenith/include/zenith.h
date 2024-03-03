#ifndef ZENITH_H
#define ZENITH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#define MAX_CAPACITY 4096

#define __ZENITH_MASK__ 0x00
#define __ZENITH_USER_MASK__ 0x10
#define __ZENITH_FOLDER_MASK__ 0x20
#define __ZENITH_DATA_MASK__ 0x30
#define __ZENITH_NODE_MASK__ 0x40

#define __Z_DEFAULT_USER_PERM__ 0b10000000
#define __Z_USER_READ_PERM__ 0b100000001
#define __Z_USER_WRITE_PERM__ 0b10000010
#define __Z_USER_EXECUTE_PERM__ 0b1000100

#define __Z_SUPER_PERM__ 0b10000000
#define __Z_SUPER_WRITE_PERM__ 0b10001000
#define __Z_SUPER_READ_PERM__ 0b10010000
#define __Z_SUPER_EXECUTE_PERM__ 0b11000000

#define __ZENITH_INIT_COMPLETE__ (__ZENITH_MASK__ | 0x00)
#define __ZENITH_LOW_LEVEL_SWAP__ (__ZENITH_MASK__ | 0x01)

#define __SUPER_USER__ (__ZENITH_USER_MASK__ | 0x0F)
#define __USER__ (__ZENITH_USER_MASK__ | 0x00)

#define __ZENITH_NODE__  0x20
#define __ZENITH_DATA_NODE__ 0x40
#define __ZENITH_FOLDER__ 0x80
#define __ROOT_FS_TAB__ 0xA0


#define __ZENITH_FOLDER_NESTING_FAILED__ (__ZENITH_FOLDER_MASK__ | 0x00)
#define __ZENITH_FOLDER_NESTING_FINISHED__ (__ZENITH_FOLDER_MASK__ | 0x01)
#define __ZENITH_FOLDER_NAME_ERROR__ (__ZENITH_FOLDER_MASK__ | 0x02)

#define __ZENITH_FILE_NESTING_COMPLETE__ (__ZENITH_DATA_MASK__ | 0x00)
#define __ZENITH_FILE_NAME_ERROR__ (__ZENITH_DATA_MASK__ | 0x01)
#define __ZENITH_FILE_CREATION_FAILED__ (__ZENITH_DATA_MASK__ | 0x02)
#define __ZENITH_FILE_LOADING_FAILED__ (__ZENITH_DATA_MASK__ | 0x03)

#define FALSE 0
#define TRUE 1



/**

#define ZENITH_IMPLEMENTATION

    - init_node fn
    - init_data_node fn
    - init_folder fn
    - zenith_malloc fn
    - zenith_free fn
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

    - zenith_insert_folder fn
    - zenith_init_root_dir fn
    - zenith_show_folder_cont fn
    - zenith_folder_navigate fn
    - zenith_mkdir fn

#define ZENITH_NAVIGATOR_FILE_IMPLEMENTATION

    - zenith_writef fn
    - zenith_loadf fn



**/


typedef struct z_folder{
    struct z_folder* next;   // next concatenated folder -> all part of the same node 
    void * node;            // the pointer data of the folder inside content
}z_folder;

typedef struct data_node{
    int user_perm;
    char name[16];
    uint8_t data_array[206];
    struct data_node* next_data;
    struct data_node* contiguous_data;
    int contiguous_flag;          // have next flag
}data_node;

typedef struct zenith_node{
    int user_perm;      // user perm
    char name[16];          // node name
    z_folder * folder;      // folder pointer list
    data_node * data;    // data poitner list
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


void* zenith_malloc(root * fs_tab,uint8_t type, int user_type);
void zenith_free(root*fs_tab, void * pointer);
void init_node(zenith_node * node, char * name, int type);
void init_data_node(data_node * data, char * name, int type);
void init_folder(z_folder * folder);

void zenith_flush_fstab(void* phd_adr,uint8_t* zenith_element ,int size);
void* zenith_get_data(void* phd_adr,int type);

int zenith_init_fs(int size, ...);
void zenith_print_volume_information(root*fs_tab);

int zenith_insert_folder(root*fs_tab,char* path, z_folder * folder);
void zenith_init_root_dir(root * fs_tab);
void zenith_show_folder_cont(root*fs_tab, char * path);
void* zenith_folder_navigate(root*fs_tab, char* path);
void zenith_mkdir(root*fs_tab, char* path, char* name, int perm);
int zenith_writef(root*fs_tab,uint8_t*f, char * path,char* name, int perm);
uint8_t* zenith_loadf(root*fs_tab, char *path, char*name);

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
            dim = sizeof(data_node);
            pointer = (void*)fs_tab->page_pointer[i];
            memcpy(pointer, &data, dim);
            fs_tab->free_pages -=1;
            fs_tab->used_space +=1;
            break;

        case __ZENITH_FOLDER__:
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

void zenith_free(root*fs_tab, void * pointer){
    int found = FALSE;
    int i=0;
    while(found != TRUE){
        if(fs_tab->page_pointer[i] == pointer){
            fs_tab->page_allocated[i] = 0x00;
            found = TRUE;
        }
        i++;
    }
    fs_tab->free_pages+=1;
    fs_tab->used_space-=1;
}

void init_node(zenith_node * node, char * name, int type){
    switch(type){
        case __SUPER_USER__:
            node->user_perm =__Z_SUPER_PERM__+__Z_SUPER_READ_PERM__+__Z_SUPER_WRITE_PERM__;
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
    //node->prev = NULL;
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
    data->next_data = NULL;
    data->contiguous_data = NULL;
    data->contiguous_flag = 0X00;
    return;
}

void init_folder(z_folder * folder){
    folder->next = NULL;
    folder->node = NULL;
    return;
}


void zenith_flush_fstab(void* phd_adr,uint8_t* zenith_element ,int size){
  uint8_t byte = 0;
    for(size_t i = 0; i<size;i++){
	byte = zenith_element[i];
    	//EEPROM.update(phd_adr+i,zenith_element[i]);
    }
    return;
    
}


void* zenith_get_data(void* phd_adr,int type){
    zenith_node * node;
    data_node * dnode;
    z_folder * folder;
    root * fs_tab;
    int size = 0;

    void* out = NULL;

    switch(type){
	case __ZENITH_NODE__:
		// here insert function to comunicate with the driver and get information from it. This example use the EEPROM function from Arduino library
		size = sizeof(zenith_node);
		node = (zenith_node*)malloc(sizeof(zenith_node));
		for(size_t i=0;i<size;i++){
			//memcpy(&node[i],&EEPROM.read(phd_adr+i), 1);
		}
		out = (void*)node;
		break;
	case __ZENITH_DATA_NODE__:
		size = sizeof(data_node);
		dnode = (data_node*)malloc(size);
		for(size_t i=0;i<size;i++){
			//memcpy(&dnode[i],&EEPROM.read(phd_adr+i), 1);
		}
		out = (void*)dnode;
		break;
	case __ZENITH_FOLDER__:
		size = sizeof(z_folder);
		folder = (z_folder*)malloc(size);
		for(size_t i=0;i<size;i++){
			//memcpy(&folder[i],&EEPROM.read(phd_adr+i), 1);
		}
		out = (void*)folder;

		break;
	case __ROOT_FS_TAB__:
		size = sizeof(root);
		fs_tab = (root*)malloc(size);
		for(size_t i=0;i<size;i++){
			//memcpy(&fs_tab[i],&EEPROM.read(phd_adr+i), 1);
		}
		out = (void*)fs_tab;
		break;
	default:
		break;
    }
    return NULL;
	
}

#ifdef ZENITH_NAVIGATOR_IMPLEMENTATION

int zenith_insert_folder(root*fs_tab,char* path, z_folder * folder){
    zenith_node * nodecpy = (zenith_node*)folder->node;
    char * name = nodecpy->name;

    int lenght_string = strlen(path);
    zenith_node * moving_node = (zenith_node*)zenith_folder_navigate(fs_tab, path);
    
    if(moving_node == NULL){
        return __ZENITH_FOLDER_NESTING_FAILED__;
    }
    z_folder * navigation_folder = moving_node->folder;
    if(navigation_folder!=NULL){
        while(navigation_folder!=NULL){
            zenith_node * current_node_pointer = (zenith_node*)navigation_folder->node;
            if(strcmp(current_node_pointer->name, name)== 0){
                return __ZENITH_FOLDER_NAME_ERROR__;
            }
            navigation_folder = navigation_folder->next;
        }   
    }
    navigation_folder = moving_node->folder;
    if(navigation_folder == NULL){
        moving_node->folder = folder;
        return __ZENITH_FOLDER_NESTING_FINISHED__;
    }
    while(navigation_folder->next != NULL){
        zenith_node * current_node_pointer = (zenith_node*)navigation_folder->node;
        navigation_folder = navigation_folder->next;
    }
    navigation_folder->next = folder;
    return __ZENITH_FOLDER_NESTING_FINISHED__;
}

void zenith_init_root_dir(root * fs_tab){
    zenith_node * node_0 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_0 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, __SUPER_USER__);
    strcpy(node_0->name, "usr");
    folder_0->node = node_0;

    zenith_node * node_1 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_1 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, __SUPER_USER__);
    strcpy(node_1->name, "share");
    folder_1->node = node_1;

    zenith_node * node_2 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_2 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, __SUPER_USER__);
    strcpy(node_2->name, "lib");
    folder_2->node = node_2;


    zenith_node * node_3 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_3 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, __SUPER_USER__);
    strcpy(node_3->name, "etc");
    folder_3->node = node_3;

    zenith_node * node_4 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_4 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, __SUPER_USER__);
    strcpy(node_4->name, "home");
    folder_4->node = node_4;

    zenith_node * node_5 = (zenith_node*)zenith_malloc(fs_tab, __ZENITH_NODE__, __SUPER_USER__);
    z_folder * folder_5 = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, __SUPER_USER__);
    strcpy(node_5->name, "bin");
    folder_5->node = node_5;

    if( zenith_insert_folder(fs_tab,"/", folder_0) == __ZENITH_FOLDER_NESTING_FAILED__ || \
        zenith_insert_folder(fs_tab,"/", folder_1) == __ZENITH_FOLDER_NESTING_FAILED__ || \
        zenith_insert_folder(fs_tab,"/", folder_2) == __ZENITH_FOLDER_NESTING_FAILED__ || \
        zenith_insert_folder(fs_tab,"/", folder_3) == __ZENITH_FOLDER_NESTING_FAILED__ || \
        zenith_insert_folder(fs_tab,"/", folder_4) == __ZENITH_FOLDER_NESTING_FAILED__ || \
        zenith_insert_folder(fs_tab,"/", folder_5) == __ZENITH_FOLDER_NESTING_FAILED__){

            zenith_free(fs_tab, node_0);
            zenith_free(fs_tab, folder_0);
            zenith_free(fs_tab, node_1);
            zenith_free(fs_tab, folder_1);
            zenith_free(fs_tab, node_2);
            zenith_free(fs_tab, folder_2);
            zenith_free(fs_tab, node_3);
            zenith_free(fs_tab, folder_3);
            zenith_free(fs_tab, node_4);
            zenith_free(fs_tab, folder_4);
            zenith_free(fs_tab, node_5);
            zenith_free(fs_tab, folder_5);
            printf("Segmentation fault: cannot get access to fs_tab, please retry or create a new partition\n");
    }

}

void zenith_mkdir(root*fs_tab, char* path,char* name,int perm){
    if(perm == __SUPER_USER__ || perm == __USER__){
        zenith_node * node = (zenith_node*)zenith_malloc(fs_tab,__ZENITH_NODE__, perm);
        z_folder * folder = (z_folder*)zenith_malloc(fs_tab, __ZENITH_FOLDER__, perm);
        strcpy(node->name,name);
        folder->node = node;

        switch(zenith_insert_folder(fs_tab, path, folder)){
            case __ZENITH_FOLDER_NESTING_FAILED__:
                zenith_free(fs_tab,node);
                zenith_free(fs_tab,folder);
                printf("Could not create a new directory: path %s is incorrect\n", path);
                break;
            case __ZENITH_FOLDER_NAME_ERROR__:
                zenith_free(fs_tab,node);
                zenith_free(fs_tab,folder);
                printf("Could not create a new directory: a folder named %s already exists in %s\n",name, path);
                break;
            default:break;
        }
    }
}

void zenith_show_folder_cont(root*fs_tab, char * path){
    zenith_node * node = (zenith_node*)zenith_folder_navigate(fs_tab, path);
    int i=0;
    if(node == NULL){
        printf("\nEmpty folder or no folder named %s\n", path);
    }else{
        printf("\nContent of %s:\n", path);
        printf("n - type - name - permission\n\n");
        z_folder * folder = node->folder;        
        while(folder != NULL){
            zenith_node * nn = (zenith_node*)folder->node;
            printf("%d D - %s/ - %d\n",i ,nn->name, nn->user_perm);
            folder = folder->next;
            i++;
        }
        data_node * data = node->data;
        while(data != NULL){
            printf("%d F - %s - %d\n",i ,data->name, data->user_perm);
            data = data->next_data;
            i++;
        }

    }
}

void* zenith_folder_navigate(root*fs_tab, char* path){
    int path_lenght = strlen(path);
    char path_cpy[path_lenght];
    int control_flag = FALSE;
    char  *tok = "/";
    strcpy(path_cpy,path);

    if(path_lenght > 1){
        char * token = strtok(path_cpy, tok);
        zenith_node * moving_node = fs_tab->first_node;

        while(token != NULL){

            control_flag = FALSE;
            z_folder * f = moving_node->folder;

            while(f != NULL && control_flag != TRUE){
                zenith_node * cache_node = (zenith_node*)f->node;
                if(strcmp(cache_node->name,token) == 0){
                    moving_node = (zenith_node*)f->node;
                    control_flag = TRUE;
                }else{
                    if(f->next != NULL){
                        f = f->next;
                    }else{
                        return NULL;
                    }
                    
                }
            }

            token = strtok(NULL,tok);
        }
        return moving_node;
    }
    return fs_tab->first_node;

}


#endif


#ifdef ZENITH_NAVIGATOR_FILE_IMPLEMENTATION

int zenith_writef(root*fs_tab,uint8_t*f, char* path, char*name,int perm){
    if(perm != __SUPER_USER__ && perm != __USER__){
        return __ZENITH_FILE_CREATION_FAILED__;
    }
    zenith_node * node = (zenith_node*)zenith_folder_navigate(fs_tab, path);
    if(node != NULL){
        data_node *d = (data_node*)zenith_malloc(fs_tab, __ZENITH_DATA_NODE__, __USER__);
        
        data_node * data_cpy = node->data;
        if(data_cpy!=NULL){
            while(data_cpy!=NULL){
                if(strcmp(data_cpy->name, name) == 0){
                    printf("Could not create file: a file named %s already exists in %s\n", name,path);
                    return __ZENITH_FILE_NAME_ERROR__;
                }
                data_cpy = data_cpy->next_data;
            }
        }
        data_cpy = node->data;
        
        int file_lenght = strlen(f);
        if(file_lenght > 206){
            int page_to_allocate = (int)(file_lenght/206)+1;
            int file_tracker = 0;
            d->contiguous_flag = 0xff;
            strcpy(d->name, name);
            int dyn_tracker = 0;
            while(dyn_tracker < 206){
                d->data_array[dyn_tracker] = f[file_tracker];
                if(file_tracker == file_lenght){
                    file_tracker = file_tracker;
                }else{
                    file_tracker+=1;
                }
                dyn_tracker+=1;
            }

            
            data_node * d_header = NULL;
            for(int i=0;i<page_to_allocate; i++){
                data_node * c_data = (data_node*)zenith_malloc(fs_tab, __ZENITH_DATA_NODE__, perm);
                strcpy(c_data->name,"");

                dyn_tracker = 0;
                while(dyn_tracker < 206){
                    c_data->data_array[dyn_tracker] = f[file_tracker];
                    if(file_tracker == file_lenght){
                        file_tracker = file_tracker;
                    }else{
                        file_tracker+=1;
                    }
                    dyn_tracker+=1;
                }

                if(d->contiguous_data != NULL){
                    d->contiguous_data->contiguous_data = c_data;
                    d->contiguous_data =  d->contiguous_data->contiguous_data;
                }else{
                    d->contiguous_data = c_data;
                    d_header = d->contiguous_data;
                }

            }
            d->contiguous_data = d_header;

            if(data_cpy == NULL){
                node->data = d;
            }else{
                while(data_cpy->next_data != NULL){
                    data_cpy = data_cpy->next_data;
                }
                data_cpy->next_data = d;
            }

            return __ZENITH_FILE_NESTING_COMPLETE__;
        }
        d->contiguous_flag = 0x00;
        strcpy(d->name, name);
        strcpy(d->data_array, f);
        

        if(data_cpy == NULL){
            node->data = d;
        }else{
            while(data_cpy->next_data != NULL){
                data_cpy = data_cpy->next_data;
            }
            data_cpy->next_data = d;
        }
        return __ZENITH_FILE_NESTING_COMPLETE__;
    } 
    return __ZENITH_FILE_CREATION_FAILED__;
}

uint8_t* zenith_loadf(root*fs_tab, char *path, char*name){
    uint8_t* out = NULL;
    zenith_node * node = (zenith_node*)zenith_folder_navigate(fs_tab, path);
    
    data_node* file_p = NULL;
    if(node->data != NULL){
        while(node->data!=NULL){
            if(strcmp(node->data->name, name) == 0){
                file_p = node->data;
            }
            node->data = node->data->next_data;
        }
    }else{
        return NULL;
    }
    if(file_p == NULL){
        return NULL;
    }

    if(file_p->contiguous_flag == 0xFF){
        data_node * h_data = file_p->contiguous_data;
        int size = 0;
        while(file_p->contiguous_data != NULL){
            size++;
            file_p->contiguous_data = file_p->contiguous_data->contiguous_data;
        }
        file_p->contiguous_data = h_data;
        int total_size = (size+1) * 206;
        out = (uint8_t*)malloc(sizeof(uint8_t)*total_size);
        
        size = 0;
        int dyn_pointer = 0;
        while(dyn_pointer<206){
            out[size] = file_p->data_array[dyn_pointer];
            dyn_pointer+=1;
            if(size == total_size-1){
                size = size;
            }else{
                size+=1;
            }
        }

        while(file_p->contiguous_data != NULL){
            dyn_pointer = 0;
            int lenght = strlen(file_p->contiguous_data->data_array);
             while(dyn_pointer<206){
                out[size] = file_p->contiguous_data->data_array[dyn_pointer];
                dyn_pointer+=1;
                if(size == total_size-1){
                    size = size;
                }else{
                    size+=1;
                }
            }
            file_p->contiguous_data = file_p->contiguous_data->contiguous_data;
        }
        out[size+1] = "\0";
        return out;
    }
    out = (uint8_t*)malloc(sizeof(uint8_t)*206);
    int size = 0;
    int lenght = strlen(file_p->data_array);
    while(size<lenght){
        out[size] = file_p->data_array[size];
        size+=1;
    }
    out[size+1] = "\0";
    return out;
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

    if(sizeof(root)%256 != 0){
        root_size+=1;
    }

    for(int i=0;i<root_size;i++){
        fs_table.page_allocated[i] = 0xff;
    }
    global_root_size = root_size*256;

    zenith_node * node = (zenith_node*)zenith_malloc(&fs_table,__ZENITH_NODE__,__SUPER_USER__);
    fs_table.first_node = node;
    strcpy(node->name,"/");

    fs_table.free_pages = fs_table.free_pages - root_size ;
    fs_table.used_space = root_size;
    zenith_init_root_dir(&fs_table);

    memcpy(drive_space, &fs_table, global_root_size);



    return __ZENITH_INIT_COMPLETE__;
}

void zenith_print_volume_information(root*fs_tab){
    printf("\n\nZ-System Volume Information\n\n");
    printf("Zenith version: %s\n", fs_tab->version_name);
    printf("Root name: %s\n", fs_tab->first_node->name);
    printf("Partition size (Kbytes): %d kb\n", (fs_tab->total_size)/1024);
    printf("Partition free space (in Kbytes, not accurate): %d kb\n", (int)(fs_tab->free_pages*256)/1024);
    printf("Partition used space (int Kbytes, not accurate): %d kb\n", (int)(fs_tab->used_space*256)/1024);
    printf("User perm: %d\n\n", fs_tab->first_node->user_perm);
}


#endif

#endif
#endif

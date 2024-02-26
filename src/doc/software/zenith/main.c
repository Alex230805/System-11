#include <stdio.h>
#include <stdlib.h>

/**

#define ZENITH_IMPLEMENTATION

    - init_node fn
    - init_data_node fn
    - init_folder fn
    - zenith malloc fn

#define ZENITH_MKFS_IMPLEMENTATION

    - zenith_init_fs fn
    - zenith_init_root_dir fn

**/
#define ZENITH_IMPLEMENTATION
#define ZENITH_MKFS_IMPLEMENTATION

#include "include/zenith.h"

#define DIM 1048576 // 1 mb

int main(void){
    if(zenith_init_fs(DIM) != __ZENITH_INIT_COMPLETE__){
        printf("Error while creating partition");
        return 1;
    }
    root fs_tab;
    memcpy(&fs_tab, drive_space, sizeof(root));
    printf("========== ZENITH FS TEST FS TAB ===========\n\n");
    printf("Zenith version: %s\n", fs_tab.version_name);
    printf("Root name: %s\n", fs_tab.first_node->name);
    printf("Partition size (bytes): %d\n", fs_tab.total_size);
    printf("Partition free pages: %d\n", fs_tab.free_pages);
    printf("Partition used space: %d\n", fs_tab.used_space);
    printf("User perm: %d\n", fs_tab.first_node->user_perm);
    printf("===========================================\n\n");
    free(drive_space);
    drive_space = NULL;
    return 0;
}
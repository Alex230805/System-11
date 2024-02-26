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

**/
#define ZENITH_IMPLEMENTATION
#define ZENITH_MKFS_IMPLEMENTATION

#include "include/zenith.h"

#define DIM 1048576 // 1 mb

uint8_t * drive_space;

int main(void){
    if(zenith_init_fs(DIM) != __ZENITH_INIT_COMPLETE__){
        printf("Error while creating partition");
        return 1;
    }
    root fs_tab_copy;
    memcpy(&fs_tab_copy, drive_space, sizeof(root));
    printf("Zeniht version name: %s\nPartition total size: %d", fs_tab_copy.version_name, fs_tab_copy.total_size);
    free(drive_space);
    drive_space = NULL;
    return 0;
}
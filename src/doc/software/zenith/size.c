#include <stdio.h>
#include <stdlib.h>

#define ZENITH_IMPLEMENTATION
#define ZENITH_NAVIGATOR_IMPLEMENTATION
#define ZENITH_NAVIGATOR_FILE_IMPLEMENTATION
#define ZENITH_MLFS_IMPLEMENTATION

#define VIRTUAL_PLATFORM

#include "./include/zenith.h"

int main(){
    printf("Root fs size: %d, Zenith node size: %d, Z-folder size: %d, Data_node size: %d\n",sizeof(root), sizeof(zenith_node), sizeof(z_folder), sizeof(data_node));
    return 0;
}

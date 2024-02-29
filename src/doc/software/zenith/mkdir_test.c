#include <stdio.h>
#include <stdlib.h>

#define ZENITH_IMPLEMENTATION
#define ZENITH_NAVIGATOR_IMPLEMENTATION
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
    zenith_print_volume_information(&fs_tab);


    zenith_mkdir(&fs_tab, "am", __USER__, "home");
    zenith_mkdir(&fs_tab, "desktop", __USER__,"home/am");
    zenith_mkdir(&fs_tab, "documents", __USER__,"home/am");
    zenith_mkdir(&fs_tab, "downloads", __USER__,"home/am");
    zenith_mkdir(&fs_tab, "images", __USER__,"home/am");
    zenith_mkdir(&fs_tab, "other", __USER__,"home/am");

    zenith_show_folder_cont(&fs_tab, "/");   
    zenith_show_folder_cont(&fs_tab, "/home");
    zenith_show_folder_cont(&fs_tab, "/am");
    zenith_show_folder_cont(&fs_tab, "/home/am");

    zenith_print_volume_information(&fs_tab);
    zenith_mkdir(&fs_tab, "other", __USER__,"home/am/poppo");
    zenith_print_volume_information(&fs_tab);
    
    free(drive_space);
    drive_space = NULL;
    return 0;
}
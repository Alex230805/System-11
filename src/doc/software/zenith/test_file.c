#include <stdio.h>
#include <stdlib.h>

#define ZENITH_IMPLEMENTATION
#define ZENITH_NAVIGATOR_IMPLEMENTATION
#define ZENITH_NAVIGATOR_FILE_IMPLEMENTATION
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

    char file[] = "Prova di testo e creazione file gne gne\n";
    char file2[] = "Prova di testo gne gne\n";
    char file3[] =  "Ola\n";
    
    zenith_mkdir(&fs_tab,"/home", "alessandro", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro", "Desktop", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro", "Downloads", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro", "Documents", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro", "Images", __USER__);

    zenith_writef(&fs_tab, file, "/home/alessandro/Documents", "poesia_1.txt", __USER__);
    zenith_writef(&fs_tab, file2, "/home/alessandro/Documents", "poesia_2.txt", __USER__);
    zenith_writef(&fs_tab, file3, "/home/alessandro/Documents", "poesia_3.txt", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro/Documents", "Dati", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro/Documents", "Dati", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro/Documents", "Progetti", __USER__);
    zenith_mkdir(&fs_tab,"/home/alessandro/Documents", "Altro", __USER__);

    zenith_mkdir(&fs_tab,"/home/alessandro/Desktop", "Software", __USER__);
    zenith_writef(&fs_tab, file, "/home/alessandro/Desktop", "poesia_1.txt", __USER__);
    zenith_writef(&fs_tab, file2, "/home/alessandro/Desktop", "poesia_2.txt", __USER__);
    
    // test duplicated name
    zenith_writef(&fs_tab, file3, "/home/alessandro/Desktop", "poesia_3.txt", __USER__);
    zenith_writef(&fs_tab, file3, "/home/alessandro/Desktop", "poesia_3.txt", __USER__);
    zenith_writef(&fs_tab, file3, "/home/alessandro/Documents", "poesia_3.txt", __USER__);
    
    zenith_show_folder_cont(&fs_tab, "/home/alessandro/Desktop");
    zenith_show_folder_cont(&fs_tab, "/home/alessandro/Documents");
    zenith_show_folder_cont(&fs_tab, "/home/alessandro/");
    zenith_show_folder_cont(&fs_tab, "/home");

    zenith_print_volume_information(&fs_tab);
    free(drive_space);
    drive_space = NULL;
    return 0;
}
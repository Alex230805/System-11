#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


    char big_file[] = \
    "C is a general-purpose computer programming language. It was created in the 1970s by Dennis Ritchie, and remains very widely used and influential.\
    By design, C's features cleanly reflect the capabilities of the targeted CPUs. It has found lasting use in operating systems, device drivers, and protocol stacks, but\
     its use in application software has been decreasing.[7] C is commonly used on computer architectures that range from the largest supercomputers to the smallest microcontrollers and embedded systems.\
    A successor to the programming language B, C was originally developed at Bell Labs by Ritchie between 1972 and 1973 to construct utilities running on Unix. It was applied to re-implementing the kernel\
    of the Unix operating system.[8] During the 1980s, C gradually gained popularity. It has become one of the most widely used programming languages,[9][10] with C compilers available for practically all\
    modern computer architectures and operating systems. The book The C Programming Language, co-authored by the original language designer, served for many years as the de facto standard for the language.\
    C has been standardized since 1989 by the American National Standards Institute (ANSI) and the International Organization for Standardization (ISO).\n";
    
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
    zenith_writef(&fs_tab, file3, "/home/alessandro/Desktop", "poesia_3.txt", __USER__);

    // test duplicated name
    zenith_writef(&fs_tab, file3, "/home/alessandro/Desktop", "poesia_3.txt", __USER__);
    zenith_writef(&fs_tab, file3, "/home/alessandro/Documents", "poesia_3.txt", __USER__);

    // test with big chunky file
    zenith_writef(&fs_tab, big_file, "/home/alessandro/Desktop", "c.txt", __USER__);


    zenith_show_folder_cont(&fs_tab, "/home/alessandro/Desktop");
    zenith_show_folder_cont(&fs_tab, "/home/alessandro/Documents");
    zenith_show_folder_cont(&fs_tab, "/home/alessandro/");
    zenith_show_folder_cont(&fs_tab, "/home");

    zenith_print_volume_information(&fs_tab);

    // test loading file into memory

    char loading_path[] = "/home/alessandro/Desktop";
    char file_name[] = "c.txt";
    uint8_t *out = zenith_loadf(&fs_tab,loading_path,file_name);
    printf("Content of %s/%s:\n\n", loading_path,file_name);
    int i =0;
    printf("%s", out);

    free(drive_space);
    drive_space = NULL;
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

#define ZENITH_IMPLEMENTATION
#include "include/zenith.h"


int main(void){
    void * head_pointer = create_partition(100000000);
    printf("Hello World");
    return 0;
}

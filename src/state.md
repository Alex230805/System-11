
# TO-DO list

### KERNEL

- kernel error message -> code implementation via hardware sys call
- define hardware design for driver implementation

### DRIVER

- implement i2c driver
- finish cf card implementation
- implement sd driver implementation
- define basic serial GPR implementation

### ZENITH

- define zenith data structure and list fs
- optimize zenith call routine
- implements basic operation for load file into memory

### BAHS

- implements a simple CLI to interact with the user
- implements a bunch of command to use the main system call like:
    - a text editor
    - a mkdir command
    - a rm command
    - a cd command
    - a ls command


# Kernel Variable Allocation table


<div class="list">

- 0 - 82 -> keyboard pointer and buffer

- 83 - 84 -> Dynamic Switching Pointer

- 85 - 88 -> CF card address bus <strong>Direct hrdw buffer dialog</strong>

- 89 -> Cf card data port <strong>Direct hrdw buffer dialog</strong> 

- 8A -> Video Card Data Port <strong>Direct hrdw buffer dialog</strong> 

- 8B -> Video Card Control Port <strong>Direct hrdw buffer dialog</strong> 

- 8C - 9E  -> Device and Status variable

- 9F -> Onboard Display Data Port <strong>Direct hrdw buffer dialog</strong> 

// A0 - FF -> Unused

- 0200 - 0280 -> kernel init module list

- 0280 - 02ff -> I/O Port || V.I.A. ??? <strong>Direct hrdw buffer dialog</strong> 

- 0300 - // -> FS and System data bus  

</div>

<style>

    .list{
        font-size:16px;
        padding: 5px;
    }

</style>

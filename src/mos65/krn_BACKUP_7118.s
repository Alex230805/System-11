; Kernel call macro

SYS_0=$00
SYS_1=$01
SYS_2=$02
SYS_3=$03
SYS_4=$04
SYS_5=$05
SYS_6=$06
SYS_7=$07
SYS_8=$08
SYS_9=$09
SYS_10=$0a
SYS_11=$0b
SYS_12=$0c

; Kernel kall assignment

SYSTEM_CALL_0=$00 ; here insert the system call like "SYSTEM_CALL_O=print_with_graphic_card"
SYSTEM_CALL_1=$00 ; or "SYSTEM_CALL_3=__ram_malloc"
SYSTEM_CALL_2=$00
SYSTEM_CALL_3=$00
SYSTEM_CALL_4=__ram_malloc
SYSTEM_CALL_5=__ram_free
SYSTEM_CALL_6=__ZENITH_SUBCALL ; based on x register for the  call id 
SYSTEM_CALL_7=$00
SYSTEM_CALL_8=$00
SYSTEM_CALL_9=$00
SYSTEM_CALL_10=$00
SYSTEM_CALL_11=$00
SYSTEM_CALL_12=$00

; Kernel Memory Layout 

L_RAM=$0000
L_RAM_END=$2fff
DEVICE=$3000
DEVICE_END=$3fff
H_RAM=$4000
H_RAM_END=$8fff


; kernel driver queue and dynamic pointer allocation

K_MODULE_ENUM=$0200

DYN_POINTER=$0300
DYN_POINTER_H=$0301


; System  I/O port/dev

DEV_1_READ_PORT_A_WRITE_PORT_B=$3000
DEV_1_WRITE_PORT_B_READ_PORT_A=$3001
DEV_1_DDRB=$3002
DEV_1_DDRA=$3003
DEV_1_SHIFT_REGISTER=$300A
DEV_1_AUX_CONTROL_REGISTER=$300B
DEV_1_PERIPHERALS_REGISTER=$300C
DEV_1_INTERRUPT_FLAG_REGISTER=$300D
DEV_1_INTERRUPT_ENABLE_REGISTER=$300E

DEV_2_READ_PORT_A_WRITE_PORT_B=($3000 & $0100)
DEV_2_WRITE_PORT_B_READ_PORT_A=$3001 & $0100
DEV_2_DDRB=($3002 & $0100)
DEV_2_DDRA=($3003 & $0100)
DEV_2_SHIFT_REGISTER=($300A & $0100)
DEV_2_AUX_CONTROL_REGISTER=($300B & $0100)
DEV_2_PERIPHERALS_REGISTER=($300C & $0100)
DEV_2_INTERRUPT_FLAG_REGISTER=($300D & $0100)
DEV_2_INTERRUPT_ENABLE_REGISTER=($300E & $0100)

DEV_3_READ_PORT_A_WRITE_PORT_B=($3000 & $0200)
DEV_3_WRITE_PORT_B_READ_PORT_A=($3001 & $0200)
DEV_3_DDRB=($3002 & $0200)
DEV_3_DDRA=($3003 & $0200)
DEV_3_SHIFT_REGISTER=($300A & $0200)
DEV_3_AUX_CONTROL_REGISTER=($300B & $0200)
DEV_3_PERIPHERALS_REGISTER=($300C & $0200)
DEV_3_INTERRUPT_FLAG_REGISTER=($300D & $0200)
DEV_3_INTERRUPT_ENABLE_REGISTER=($300E & $0200)


; MicroKernel Architecture


__K_BOOT:
    lda #$00    ; load 0 into a
    ldx #$00    ; load 0 into x

    ldy #K_MODULE_NUMBER_ON_BOOT    ; load module number index

    sta KERNEL_STATE            ; reset kernel state

    ; initialize file system variable for driver initialization

    sta ZENITH_ABLAYER_DEVICE_CALL_POINTER      ; initialie function list pointer
    sta ZENITH_CF_ID                            ; letter id for cd card
    sta ZENITH_SD_ID                            ; letter id for sd card
    sta ZENITH_DEVICE_LIST                      ; list of identified devices
    sta ZENITH_CURRENT_DEVIE                    ; current mounted devices

    ; Kenrel initialization, first step: read the kernel init module and enable it
    __call_and_init:
        lda K_MODULE_PRIORITY_LIST_HEAD, x
        sta DYN_POINTER
        inx
        lda K_MODULE_PRIORITY_LIST_HEAD, x
        sta DYN_POINTER_H
        jsr __k_initialize_device ; initialize device
        inx
        dey
        cpx #20
        beq __k_error_panic
        cpy #$00
        bne __call_and_init 
        beq __call_and_init_end
    __k_error_panic:
        lda #KERNEL_WR_0
        sta KERNEL_STATE
        jmp __kernel_panic          ; trigger kenrel panic
    __call_and_init_end:
        
    jsr ZENITH_INITFS   ; nitialize zenith FS

    jsr __K_RAMS ; initialize ram management system (RAMS)

    jmp __bash_cli

    __k_initialize_device:
        jmp (DYN_POINTER)   ; use rts from the init routine to go back in the initialization process
        lda #KERNEL_MISSING_POINTER
        sta KERNEL_STATE    
        jmp __k_error_panic ; if for some reasons this doesn't work trigger the ___k_error_state



;
;
;   Kernel panic routine
;
;


__kernel_panic:
    lda KERNEL_STATE
    cmp #KERNEL_WR_0            ; kernel init error
    beq __kernel_init_error     
    bne __kernel_missing_pointer_error

    __kernel_init_error:
        jmp __K_BOOT
    
    __kernel_missing_pointer_error: 
    
    cmp #KERNEL_MISSING_POINTER
    beq __kernel_missing_pointer_error_detected
    bne __zenith_init_error_check

    __kernel_missing_pointer_error_detected:
        nop
        ;
        ;  code to print error
        ;
        jmp __k_error_loop


    __zenith_init_error_check:
    
    cmp #KERNEL_ZENITH_INIT_ERROR ; kernel zenith init error
    beq __kernel_zenith_init_error_detected
    bne __kernel_out_of_memory_check

    __kernel_zenith_init_error_detected:
        nop
        ;
        ;    code to print error
        ;
        jmp __k_error_loop

    __kernel_out_of_memory_check:

    cmp #KERNEL_OUT_OF_MEMORY_ERROR ; kernel out memory error
    beq __kernel_out_of_memory_error_detected
    bne __kernel_check_out_of_memory_index

    __kernel_out_of_memory_error_detected:
        nop
        ;
        ;   code to print error
        ;
        jmp __k_error_loop

    __kernel_check_out_of_memory_index:

    cmp #MALLOC_OUT_OF_MEMORY_INDEX
    beq __kernel_malloc_out_of_memory_index_detected
    bne __kernel_force_error_panic

    __kernel_malloc_out_of_memory_index_detected:
        nop
        ;
        ; code to print error
        ;
        jmp __k_error_loop
    
    __kernel_force_error_panic:
        ;
        ;   code to print error
        ;
        jmp __k_error_loop

    
__k_error_loop:
    nop
    jmp __k_error_loop

;
;   Error msg
;


__kernel_missing_pointer_error_msg: 
<<<<<<< HEAD
    .word "Error: missing return pointer trigger kernel panic\n"

__zenith_init_error_msg:
    .word "Error: failed to start file system .. Retry\n"

__kernel_out_of_memory_error_msg:
    .word "Error: out of memory error\n"

__kernel_malloc_segmentation_fault_msg:
    .word "Malloc: segmentation fault\n"

__kernel_force_panic_msg:
    .word "forced Kernel panic status: check your configuration!\n"
=======
    .byte "Error: missing return pointer trigger kernel panic"

__zenith_init_error_msg:
    .byte "Error: failed to start file system .. Retry"

__kernel_out_of_memory_error_msg:
    .byte "Error: out of memory error"

__kernel_malloc_segmentation_fault_msg:
    .byte "Malloc: segmentation fault"

__kernel_force_panic_msg:
    .byte "forced Kernel panic status: check your configuration!"
>>>>>>> e244a6b5a0518a8e731c18aac886f6f355c819c7

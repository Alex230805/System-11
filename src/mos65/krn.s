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

SYSTEM_CALL_0=$00 ; here insert the system call
SYSTEM_CALL_1=$00
SYSTEM_CALL_2=$00
SYSTEM_CALL_3=$00
SYSTEM_CALL_4=$00
SYSTEM_CALL_5=$00
SYSTEM_CALL_6=$00
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
K_MODULE_PRIORITY_LIST_HEAD=$0201
K_MODULE_PRIORITY_LIST_END=$02ff

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

    sta ZENITH_CF_ID
    sta ZENITH_SD_ID   
    sta ZENITH_DEVICE_LIST
    sta ZENITH_CURRENT_DEVIE

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
        lda KERNEL_WR_0
        sta KERNEL_STATE
        jmp __K_BOOT
    __call_and_init_end:
    nop
    ;
    ;
    ;   Another kernel code here
    ;
    ;
    ; Jump to the bash


    __k_initialize_device:
        jmp (DYN_POINTER)   ; use rts from the init routine to go back in the initialization process
        jmp __k_error_panic ; if for some reasons this doesn't work trigger the ___k_error_state
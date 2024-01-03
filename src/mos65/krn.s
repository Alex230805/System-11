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
; MicroKernel Architecture


__K_BOOT:
    lda #$00
    ldx #$00
    ldy #K_MODULE_NUMBER_ON_BOOT

    ; Kenrel initialization, first step: read the kernel init module and enable it
    __call_and_init:
        lda K_MODULE_PRIORITY_LIST_HEAD, x
        sta DYN_POINTER
        inx
        lda K_MODULE_PRIORITY_LIST_HEAD, x
        sta DYN_POINTER_H
        jsr (DYN_POINTER)
        inx
        dey
        cpx #20
        beq __k_error_panic
        cpy #$00
        bne __call_and_init 
        beq __call_and_init_end
    __k_error_panic:
        nop
    ;
    ; Error panic code here
    ;
        jmp __K_BOOT
    __call_and_init_end:
    nop
    ;
    ;
    ;   Another kernel code here
    ;
    ;
    ; Jump to the bash
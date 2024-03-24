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
SYSTEM_CALL_3=__obd_write_with_adr
SYSTEM_CALL_4=__ram_malloc
SYSTEM_CALL_5=__ram_free
SYSTEM_CALL_6=__ZENITH_SUBCALL ; based on x register for the  call id 
SYSTEM_CALL_7=$00
SYSTEM_CALL_8=$00
SYSTEM_CALL_9=$00
SYSTEM_CALL_10=$00
SYSTEM_CALL_11=$00
SYSTEM_CALL_12=$00

; kernel driver queue and dynamic pointer allocation

DYN_POINTER=$83
DYN_POINTER_H=$84

VIA_0=$0280
VIA_0_IO_B=VIA_0
VIA_0_IO_A=VIA_0+1
VIA_0_DDRB=VIA_0+2  
VIA_0_DDRA=VIA_0+3 
VIA_0_T1CL=VIA_0+4
VIA_0_T1CH=VIA_0+5
VIA_0_T1LL=VIA_0+6
VIA_0_T1LH=VIA_0+7
VIA_0_T2CL=VIA_0+8
VIA_0_T2CH=VIA_0+9
VIA_0_SR=VIA_0+10
VIA_0_AUX=VIA_0+11
VIA_0_PER=VIA_0+12

VIA_1=VIA_1+16
VIA_1_IO_B=VIA_1
VIA_1_IO_A=VIA_1+1
VIA_1_DDRB=VIA_1+2  
VIA_1_DDRA=VIA_1+3 
VIA_1_T1CL=VIA_1+4
VIA_1_T1CH=VIA_1+5
VIA_1_T1LL=VIA_1+6
VIA_1_T1LH=VIA_1+7
VIA_1_T2CL=VIA_1+8
VIA_1_T2CH=VIA_1+9
VIA_1_SR=VIA_1+10
VIA_1_AUX=VIA_1+11
VIA_1_PER=VIA_1+12



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
    
    sta SYSTEM_BUS_READ_POINTER			; init system bus pointer
    sta SYSTEM_BUS_WRITE_POINTER		; init system bus pointer
    
    lda #$00		; load 00 into A
    sta VIA_0_DDRA	; set via 0 post to input
    sta VIA_1_DDRA	; set via 1 post to input 
    lda #$ff		; load ff into A
    sta VIA_0_DDRB	; set via 0 port to output
    sta VIA_1_DDRA	; set via 0 port to output


    ; Kenrel initialization: read the kernel init module and enable it
    
    __call_and_init:
        lda K_MODULE_PRIORITY_LIST_HEAD, x
        sta DYN_POINTER
        inx
        lda K_MODULE_PRIORITY_LIST_HEAD, x
        sta DYN_POINTER_H
        jsr __k_initialize_device ; initialize device
        inx
        dey
        cpy #$00
        bne __call_and_init 
        beq __call_and_init_end
    __k_error_panic:
        lda #KERNEL_WR_0
        sta KERNEL_STATE
        jmp __kernel_panic          ; trigger kenrel panic
    __call_and_init_end:
        
    jsr ZENITH_INITFS   ; nitialize zenith FS

    ;jsr __K_RAMS ; initialize ram management system (RAMS)

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
	; code to link the pointer of the error string
        lda #<__kernel_missing_pointer_error_msg
	sta DYNAMIC_POINTER
	lda #>__kernel_missing_pointer_error_msg

	lda #SYS_5
	jsr K_CALL
        jmp __k_error_loop


    __zenith_init_error_check:
    
    cmp #KERNEL_ZENITH_INIT_ERROR ; kernel zenith init error
    beq __kernel_zenith_init_error_detected
    bne __kernel_out_of_memory_check

    __kernel_zenith_init_error_detected:
	; code to link the pointer of the error string
        lda #<__zenith_init_error_msg
	sta DYNAMIC_POINTER
	lda #>__zenith_init_error_msg 
	
	lda #SYS_5
	jsr K_CALL
        jmp __k_error_loop

    __kernel_out_of_memory_check:

    cmp #KERNEL_OUT_OF_MEMORY_ERROR ; kernel out memory error
    beq __kernel_out_of_memory_error_detected
    bne __kernel_check_out_of_memory_index

    __kernel_out_of_memory_error_detected:
        lda #<__kernel_out_of_memory_error_msg
	sta DYNAMIC_POINTER
	lda #>__kernel_out_of_memory_error_msg
        
	lda #SYS_5
	jsr K_CALL
        jmp __k_error_loop

    __kernel_check_out_of_memory_index:

    cmp #MALLOC_OUT_OF_MEMORY_INDEX
    beq __kernel_malloc_out_of_memory_index_detected
    bne __kernel_force_error_panic

    __kernel_malloc_out_of_memory_index_detected:
        ; code to link the pointer of the error string
        
        lda #<__kernel_out_of_memory_error_msg
	sta DYNAMIC_POINTER
	lda #>__kernel_out_of_memory_error_msg
	lda #SYS_5
	jsr K_CALL
        jmp __k_error_loop
    
    __kernel_force_error_panic:
        lda #<__kernel_focrce_panic_msg
	sta DYNAMIC_POINTER
	lda #>__kernel_force_panic_msg	
	
	sta DYNAMIC_POINTER_H
        lda #SYS_5
	jsr K_CALL
        jmp __k_error_loop
    
__k_error_loop:
    nop
    jmp __k_error_loop

;
;   Error msg
;


 __kernel_missing_pointer_error_msg:
    .word "Error: missing return pointer trigger kernel panic\n"
    .byte $00

__zenith_init_error_msg:
    .word "Error: failed to start file system .. Retry\n"
    .byte $00


__kernel_out_of_memory_error_msg:
   .word "Error: out of memory error\n"
    .byte $00


__kernel_force_panic_msg:
    .word "forced Kernel panic status: check your configuration!\n"
    .byte $00

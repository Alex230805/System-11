CODE_SEGMENT=$9000
INTERRUPT_VECTOR=$fffe
NMI_VECTOR=$fffa
STARTUP_VECTOR=$fffc


K_MODULE_PRIORITY_LIST_HEAD=$0201
K_MODULE_PRIORITY_LIST_END=$02ff


K_MODULE_NUMBER_ON_BOOT=$05


 .org $00
 .word $00
 .org CODE_SEGMENT

boot:
    ldx #$00
    txs 
    cld
    clv
    clc
    sei
    ; 
    ; To change the kernel config, changing the bootloader configuration is needed over the necessity to add new kernel module.
    ; So the system, even if you add as many kernel module as you want, it cannot be used by the system until the bootloader update is occurred
    ;

    ; Kernel list sequence: set the default module to activate on boot, the kernel will automaticaly run the first entry
    lda #K_MODULE_NUMBER_ON_BOOT    ; load module number sequence
    sta K_MODULE_ENUM               ; store it for future kernel usage

    ldx K_MODULE_PRIORITY_LIST_HEAD

    ; load keyboard init module according to the driver selected
    lda #<__key_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx
    lda #>__key_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx
    ; same for video card

    lda #<__fx_card_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx
    lda #>__fx_card_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx

    ; same for cf card

    lda #<__cf_card_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx
    lda #>__cf_card_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx


    ; i2c initialization

    lda #<__i2c_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx
    lda #>__i2c_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx

        ; additional driver for general purpose:
    ; sd initialization

    lda #<__sd_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx
    lda #>__sd_init
    sta K_MODULE_PRIORITY_LIST_HEAD,x
    inx

    jmp __K_BOOT
    


; KERNEL FILE
 include "./mos65/krn.s"
 include "./mos65/krn_dyn_call.s"
 include "./mos65/krn_rams.s"
 include "./mos65/kstate.s"

; FILE SYSTEM FILE
 include "./filesystem/zenithfs/z_init.s"
 include "./filesystem/zenithfs/zenith_kcall.s"
 include "./filesystem/zenithfs/zenith_root.s"
 include "./filesystem/zenithfs/zenith_file.s"
 include "./filesystem/zenithfs/zenith_dir.s"
; DRIVER
 include "./driver/cf_card.s"
 include "./driver/fx_card.s"
 include "./driver/keyboard.s"
 include "./driver/sd.s"
 include "./driver/i2c.s"

; program and ui

 include "./cli/bash_int.s"

 .org NMI_VECTOR
 .word __key_read
 .org STARTUP_VECTOR
 .word boot
 .org INTERRUPT_VECTOR
 .word __key_read
CODE_SEGMENT=$9000
INTERRUPT_VECTOR=$fffe
NMI_VECTOR=$fffa
STARTUP_VECTOR=$fffc


K_MODULE_PRIORITY_LIST_HEAD=$0200
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

    ; load keyboard init module according to the driver selected
    lda #__key_init
    sta K_MODULE_PRIORITY_LIST_HEAD
    lda #__key_init+1
    sta K_MODULE_PRIORITY_LIST_HEAD+1

    ; same for video card

    lda #__fx_card_init
    sta K_MODULE_PRIORITY_LIST_HEAD+2
    lda #__fx_card_init+1
    sta K_MODULE_PRIORITY_LIST_HEAD+3

    ; same for cf card

    lda #__cf_card_init
    sta K_MODULE_PRIORITY_LIST_HEAD+4
    lda #__cf_card_init+1
    sta K_MODULE_PRIORITY_LIST_HEAD+5

    ; initialize generic perpherals

    lda #__ph_init
    sta K_MODULE_PRIORITY_LIST_HEAD+5
    lda #__ph_init
    sta K_MODULE_PRIORITY_LIST_HEAD+6

    ; additional driver for general purpose:
    ; sd initialization

    lda #__sd_init
    sta K_MODULE_PRIORITY_LIST_HEAD+7
    lda #__sd_init
    sta K_MODULE_PRIORITY_LIST_HEAD+8


    jmp __K_BOOT


; KERNEL FILE
 include "./mos65/krn.s"
 include "./mos65/krn_dyn_call.s"
 include "./mos65/krn_ramfs.s"
 include "./mos65/krn_reg.s"
 include "./mos65/kstate.s"

; FILE SYSTEM FILE
 include "./filesystem/zenithfs/zenith_kcall.s"
 include "./filesystem/zenithfs/zenith_root.s"
 include "./filesystem/zenithfs/zenith_file.s"
 include "./filesystem/zenithfs/zenith_dir.s"
; DRIVER
 include "./driver/cf_card.s"
 include "./driver/fx_card.s"
 include "./driver/keyboard.s"
 include "./driver/peripherals.s"
 include "./driver/sd.s"


 .org NMI_VECTOR
 .word __SYS_INTERRUPT
 .org STARTUP_VECTOR
 .word boot
 .org INTERRUPT_VECTOR
 .word __SYS_INTERRUPT
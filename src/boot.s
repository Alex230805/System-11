CODE_SEGMENT=$9000
INTERRUPT_VECTOR=$fffe
NMI_VECTOR=$fffa
STARTUP_VECTOR=$fffc

 .org $00
 .word $00
 .org CODE_SEGMENT

boot:
    nop         ; code here: implement dynamic call and kernel registry  priority for microkernel module. 
    ; 
    ; To change the kernel config, changing the bootloader configuration is needed over the necessity yo add new kernel module.
    ; So the system, even if you add as many kernel module as you want, it cannot be used by the system until the bootloader update is occurred
    ;
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
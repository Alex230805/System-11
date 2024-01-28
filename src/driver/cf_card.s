; Cf Card support


CF_CARD_LB=$86          ; low byte address
CF_CARD_HB=$87          ; hight byte address
CF_CARD_XLB=$88         ; x low byte address
CF_CARD_XHB=$89         ; x hight byte address

CF_CARD_DATA_PORT=$8A   ; data port

__cf_card_init:
    ; apply the start address for cf card
    lda #$00
    sta CF_CARD_LB
    sta CF_CARD_HB
    sta CF_CARD_XLB
    sta CF_CARD_XHB


    ; same with variable vector
    sta V_CF_CARD_LB
    sta V_CF_CARD_HB
    sta V_CF_CARD_XLB
    sta V_CF_CARD_XHB

    lda #$FF
    sta CF_CARD_ENABLE     ; enable CF card status flag

    ; zenith FS section

    ldx ZENITH_DEVICE_LIST ; load zenith device list enum
    cpx #$05                ; compare with 5 ( the number must be inferior to that )
    beq __cf_zenith_abort_addition  ; if ar equal then abort assignment
    lda #"A"                 ; else load a into accumulator

    clc
    adc ZENITH_DEVICE_LIST   ; sum it with the device list enum
    sta ZENITH_CF_ID            ; save letter
    
    inx                       ; increment enum
    stx ZENITH_DEVICE_LIST    ; save device list

    jmp __cf_zenith_store_function_pointer  ; next start saving function pointer

    __cf_zenith_abort_addition:
        lda #$FF                            ; store alert status into zenith_cf_letter
        sta ZENITH_CF                     
        rts                                 ; return from call

     __cf_zenith_store_function_pointer:
    
    ldx ZENITH_ABLAYER_DEVICE_CALL_POINTER  ; load device list pointer

    ; save address for the 1st call
    lda #__cf_set_address                 
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #__cf_set_address+1
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx

    ; save address for the 2nd call
    lda #__cf_card_read_content
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #__cf_card_read_content+1
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx


    ; save address for the 3rd call
    lda #__cf_card_write_content
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #__cf_card_write_content+1
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx

    ; save address for the 4th call
    lda #__cf_card_get_device_propriety
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #__cf_card_get_device_propriety+1
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx

    ; save latest pointer position
    stx ZENITH_ABLAYER_DEVICE_CALL_POINTER

    rts         ; return from subroutine


__cf_set_address:
    pha                     ; push accumulator into stack

    lda V_CF_CARD_LB        ; load low byte variable into a
    sta CF_CARD_LB          ; store it into cf card address register

    lda V_CF_CARD_HB
    sta CF_CARD_HB

    lda V_CF_CARD_XLB
    sta CF_CARD_XLB

    lda V_CF_CARD_XHB
    sta CF_CARD_XHB

    pla                     ; pull accumulator from stack
    rts                     ; return

__cf_card_read_content:
    lda CF_CARD_DATA_PORT
    rts

__cf_card_write_content:
    sta CF_CARD_DATA_PORT
    rts

__cf_card_get_device_propriety:
    nop
    rts
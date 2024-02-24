; sd card support


__sd_init:
    ; initialize address variable
    lda #$00
    sta V_SD_CARD_LB
    sta V_SD_CARD_HB

    sta SD_CARD_MOUNTED ; mounted flag for sd card
    ;
    ;
    ;   i2c interface comunication for setting up the sd driver
    ;

    lda #$FF                ; enable device
    sta SD_CARD_ENABLE      ; store it


    ; zenith fx section

    ldx ZENITH_DEVICE_LIST ; load zenith device list enum
    cpx #$05                ; compare with 5 ( the number must be inferior to that )
    beq __sd_zenith_abort_addition  ; if ar equal then abort assignment
    lda #"A"                 ; else load a into accumulator

    clc
    adc ZENITH_DEVICE_LIST   ; sum it with the device list enum
    sta ZENITH_SD_ID            ; save letter
    
    inx                       ; increment enum
    stx ZENITH_DEVICE_LIST    ; save device list

    jmp __sd_zenith_store_function_pointer  ; next start saving function pointer

    __sd_zenith_abort_addition:
        lda #$FF                            ; store alert status into zenith_cf_letter
        sta ZENITH_SD_ID                    
        rts                                 ; return from call

     __sd_zenith_store_function_pointer:
    
    ldx ZENITH_ABLAYER_DEVICE_CALL_POINTER  ; load device list pointer

    ; save address for the 1st call
    lda #<__sd_card_set_address                 
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #>__sd_card_set_address
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx

    ; save address for the 2nd call
    lda #<__sd_card_read_content
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #>__sd_card_read_content
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx


    ; save address for the 3rd call
    lda #<__sd_card_write_content
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #>__sd_card_write_content
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx

    ; save address for the 4th call
    lda #<__sd_card_get_device_propriety
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx
    lda #>__sd_card_get_device_propriety
    sta ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
    inx

    ; save latest pointer position
    stx ZENITH_ABLAYER_DEVICE_CALL_POINTER

    rts         ; return from subroutine

__sd_card_set_address:
    nop
    rts

__sd_card_read_content:
    ldx KEYBOARD_WRITE_POINTER
    jsr __i2c_read
    rts

__sd_card_write_content:
    ldx KEYBOARD_READ_POINTER
    jsr __i2c_write
    rts

__sd_card_get_device_propriety:
    nop
    rts
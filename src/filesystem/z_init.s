
;
;   Init Zenith FS
;


ZENTH_INITFS:

    lda #$00
    cmp ZENITH_DEVICE_LIST
    beq __zenith_init_error_no_d
    bne __zenith_device_check
    __zenith_init_error_no_d:
        jmp __zenith_init_error                ; no such device


    __zenith_device_check:                  ; check if cf card is the main device
        lda ZENITH_CF_ID
        sbc #"A"
        cmp #$00
        bne __zeniht_check_next
        beq __zenith_mount_cf_card          ; if so then mount it

        __zenith_mount_cf_card:
            jmp __zenith_mount_cf_card_fn


        __zeniht_check_next:                    ; if not then check if sd card is main device
            lda ZENITH_SD_ID
            sbc #"A"
            cmp #$00
            bne __zenith_no_main_device
            beq __zenith_mount_sd_card          ; if so then mount it!

            __zenith_mount_sd_card:
                jmp __zenith_mount_sd_card_fn

            __zenith_no_main_device:           ; if not then trigger kernel error
                jmp __zenith_init_error

        __zenith_mount_cf_card_fn:
            jsr __zenith_mount_set_function_pointer

            lda #"A"                    ; set cf card id into current device
            sta ZENITH_CURRENT_DEVIE

            lda #$ff                    ; set mounted flag for cf card
            sta CF_CARD_MOUNTED

            jmp __zenith_end_initialization

        __zenith_mount_sd_card:
            jsr __zenith_mount_set_function_pointer

            lda #"A"                    ; set cf card id into current device
            sta ZENITH_CURRENT_DEVIE

            lda #$ff                    ; set mounted flag for cf card
            sta SD_CARD_MOUNTED
            
            jmp __zenith_end_initialization

    __zenith_end_initialization:
        rts                             ; return into kernel boot 





    __zenith_mount_set_function_pointer:        ; save function pointer for device function
        ldx #$00
        
        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x   ; set address function
        sta ZENITH_DEVICE_CALL_1_lb
        inx
        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
        sta ZENITH_DEVICE_CALL_1_hb
        inx

        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x   ; read function
        sta ZENITH_DEVICE_CALL_2_lb
        inx
        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
        sta ZENITH_DEVICE_CALL_2_hb
        inx

        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x   ; write function
        sta ZENITH_DEVICE_CALL_3_lb
        inx
        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
        sta ZENITH_DEVICE_CALL_3_hb
        inx

        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x   ; get info function
        sta ZENITH_DEVICE_CALL_4_lb
        inx
        lda ZENITH_ABLAYER_DEVICE_CALL_HEAD,x
        sta ZENITH_DEVICE_CALL_4_hb

        rts

__zenith_init_error:                        ; error paic for zenith initialization failure
        lda #KERNEL_ZENITH_INIT_ERROR
        sta KERNEL_STATE
        jmp __kenrel_panic  
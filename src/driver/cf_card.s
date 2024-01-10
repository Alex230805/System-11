; Cf Card support


CF_CARD_LB=$86          ; low byte address
CF_CARD_HB=$87          ; hight byte address
CF_CARD_XLB=$88         ; x low byte address
CF_CARD_XHB=$89         ; x hight byte address

CF_CARD_DATA_PORT=$8A   ; data port

__cf_card_init:
    lda #$00
    sta CF_CARD_LB
    sta CF_CARD_HB
    sta CF_CARD_XLB
    sta CF_CARD_XHB

    sta V_CF_CARD_LB
    sta V_CF_CARD_HB
    sta V_CF_CARD_XLB
    sta V_CF_CARD_XHB

    lda #$FF
    sta CF_CARD_ENABLE     ; enable CF card status flag
    rts


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
    nop

__cf_card_write_content:
    nop

__cf_card_get_device_propriety:
    nop
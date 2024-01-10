; Keyboard driver

KEYBOARD_READ_POINTER=$00 ; variable to keep track the keyboard buffer
KEYBOARD_WRITE_POINTER=$01 ; 

KEYBOARD_INTERFACE=DEV_1_READ_PORT_A_WRITE_PORT_B ; put the interface here

KEYBOARD_BUFFER_LENGHT=$80 ; lenght of the keyboard buffer
KEYBOARD_BUFFER_START=$02 ; keyboard buffer start

__key_read=INTERRUPT_VECTOR

__key_init:
    lda #$00
    sta KEYBOARD_READ_POINTER
    sta KEYBOARD_WRITE_POINTER

    ;
    ; another code to setting up the interface
    ;
    ;
    
    lda #$ff
    sta KEYBOARD_ENABLE     ; set enable status for keyboard
    rts


__key_read:
    php ; push statush into stack
    pha ; push accumulator into stack
    txa ; transfer x into accumulator
    pha ; push accumulator
    tya ; transfer y into accumulator
    pha ; push a into acculator

    sei ; set interrupt disable and clear flags
    clv
    cld

__key_read:
    ldx KEYBOARD_WRITE_POINTER ; load keyoard buffer write pointer
    lda KEYBOARD_INTERFACE  ; read from keyboard interface
    sta KEYBOARD_BUFFER_START, x ; store character into the buffer
    inx                           ; increment x pointer
    cpx KEYBOARD_BUFFER_LENGHT  ; check if X is not above the 128 char limit
    beq __keyboard_reset_index_keyboard  ; if is greater then reset the pointer
    bne __keyboard_store_current_value ; if not then store normaly the current value

    __keyboard_reset_index_keyboard:
        ldx #$00              ; load 0 into X
    __keyboard_store_current_value:

    stx KEYBOARD_WRITE_POINTER  ; store buffer write pointer value into ram

__key_end_read:
    pla ; pull accumulator from stack
    tay ; transfer accumulator into Y
    pla ; pull accumulator from stack
    tax ; transfer accumulator into X
    pla ; pull accumulator from stack
    php ; pull status from stack
    cli ; clear interrupt disable
    rts  ; return from subroutine
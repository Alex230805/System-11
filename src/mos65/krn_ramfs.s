;
;   Kernel RAM management
;


RAM_PAGE=$50 ; the amount of space in total


LOW_MEMORY_BANKING_REGISTER=$83
HIGHT_MEMORY_BANKING_REGISTER=$84


__K_RAMS:
    lda #RAM_PAGE
    sta RAM_FREE_PAGE                       ; reset the free page counter

    lda #$00
    sta RAM_ALLOCATED_PAGE                  ; reset the allocated ram counter

    lda #$00
    sta LOW_MEMORY_BANKING_REGISTER         ; setting up the default memory configuration
    sta HIGHT_MEMORY_BANKING_REGISTER

    sta RAM_PAGE_ALLOC_POINTER              ; reset the page alloc pointer

    jsr __k_ram_init_set_page_address_table ; routine to set address table used by malloc and free

    rts

; MALLOC

;
;   y -> the amount of page to allocate
;

__ram_malloc:
    clv ; clear overflow
    tya 
    pha

    lda RAM_FREE_PAGE           ; load ram free page
    cmp #$00                    ; compare with 0
    beq __ram_malloc_abort      ; if equal then abort operation
    bne __ram_malloc_start      ; if not then start malloc routine

    __ram_malloc_abort:
        jmp __ram_end_malloc    ; abort operation

    __ram_malloc_start:         ; check if there is enough space

    clc                         ; clear carry
    pla                         ; pull y content
    pha                         ; and re-save it


    __ram_malloc_check_for_free_page:
        ldx #$00
        lda RAM_PAGE_ALLOC_POINTER,x
        cmp #$00
        beq __ram_malloc_check_for_free_page_y_index
        bne __ram_out_of_request

        __ram_malloc_check_for_free_page_y_index:
            clc
            stx RAM_LB_CALCULUS_ADDRESS
            cpy RAM_LB_CALCULUS_ADDRESS
            bcc __ram_malloc_check_desired_amount
            bcs __ram_malloc_check_for_free_page 

            __ram_malloc_check_desired_amount:
                clc
                clv
                sty RAM_LB_CALCULUS_ADDRESS
                cpx RAM_LB_CALCULUS_ADDRESS
                beq __ram_malloc_page_disp
                bne __ram_malloc_continue_to_check_for_page

                __ram_malloc_continue_to_check_for_page:
                    jmp __ram_malloc_check_for_free_page

    __ram_out_of_request:       ; out of request -> abort operation
        jmp __ram_end_malloc

    __ram_malloc_page_disp:

        adc RAM_ALLOCATED_PAGE      ; update allocated page counter
        sta RAM_ALLOCATED_PAGE      ; store it 

        lda RAM_FREE_PAGE           ; load free counter
        sec                         ; set carry
        sbc RAM_ALLOCATED_PAGE      ; subtract with allocated page counter
        sta RAM_FREE_PAGE           ; save free page counter


        ldx RAM_PAGE_ALLOC_POINTER
        stx RAM_LB_CALCULUS_ADDRESS     ; save for later

        lda RAM_PAGE_ALLOC_POINTER
        adc #"A"
        
        sta RAM_ALLOCATED_ASSINGED_LETTER

        clv
        clc

        __ram_malloc_assign_char_vector:
            sta RAM_PAGE_ALLOCATED_FLAG_HEAD,x
            inx
            dey
            cpy #$00
            beq __ram_malloc_end_char_assigment
            bne __ram_malloc_assign_char_vector

        __ram_malloc_end_char_assigment:
            
            stx RAM_PAGE_ALLOC_POINTER

            ldx RAM_LB_CALCULUS_ADDRESS

            lda RAM_PAGE_PADDING_HEAD,x
            sta RAM_RETURNED_ALLOCATED_FIRST_POINTER_LB
            inx
            lda RAM_PAGE_PADDING_HEAD,x
            sta RAM_RETURNED_ALLOCATED_FIRST_POINTER_HB
            dex

            clc
            pla 
            adc #$RAM_LB_CALCULUS_ADDRESS
            asl
            tax
            dex

            lda RAM_PAGE_PADDING_HEAD,x
            sta RAM_RETURNED_ALLOCATED_LAST_POINTER_LB
            inx
            lda RAM_PAGE_PADDING_HEAD,x
            sta RAM_RETURNED_ALLOCATED_LAST_POINTER_HB

    __ram_end_malloc:
        rts 


__ram_free:

    rts



__k_ram_init_set_page_address_table:
    ldy #$50
    ldx #$00
    clc
    lda #$00
    sta RAM_LB_CALCULUS_ADDRESS
    lda #$40
    sta RAM_HB_CALCULUS_ADDRESS



    __k_ram_offset_by_one:
        lda RAM_LB_CALCULUS_ADDRESS
        clc
        adc #$ff
        sta RAM_LB_CALCULUS_ADDRESS
        adc #$00
        sta RAM_HB_CALCULUS_ADDRESS

    __k_ram_calculate_offset:

        lda RAM_LB_CALCULUS_ADDRESS
        sta RAM_PAGE_PADDING_HEAD,x
        inx
        lda RAM_HB_CALCULUS_ADDRESS
        sta RAM_PAGE_PADDING_HEAD,x
        inx

        lda RAM_LB_CALCULUS_ADDRESS
        clc
        adc #$ff
        sta RAM_LB_CALCULUS_ADDRESS
        adc #$00
        sta RAM_HB_CALCULUS_ADDRESS
        inx


        dey

        cpy #$00
        beq __k_ram_end_init_page
        bne __k_ram_continue_calculate_offset

        __k_ram_continue_calculate_offset:
            jmp __k_ram_offset_by_one
        
        __k_ram_end_init_page:
    rts                             ; return to initialization
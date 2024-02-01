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


    lda #$00
    __reset_allocation_list:
        sta RAM_PAGE_ALLOCATED_FLAG_HEAD,x
        inx
        cpx RAM_PAGE_ALLOCATED_FLAG_HEAD
        bne __reset_allocation_list
        beq __k_ram_init_map_paging

    __k_ram_init_map_paging:
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

    ldx #$00
    stx RAM_PAGE_ALLOC_POINTER  ; reset pointer

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
    
    __ram_malloc_check_for_free_page_reset_pointer:
        pha                     ; push a into stack
        lda #$00                ; load $00 into a
        sta RAM_PAGE_ALLOC_POINTER  ; store it in ram
        pla                     ; pull accumulator from stack
        jmp __ram_malloc_check_for_free_page


    __ram_malloc_check_for_free_page_inc_index: 
        clc
        inx                     ; inc x
        cpx #$80                ; max page limit
        bcs __ram_out_of_request    ; if it's greater than limit
    __ram_malloc_check_for_free_page:
        lda RAM_PAGE_ALLOCATED_FLAG_HEAD,x      ; load content of flag register for allocated ram
        cmp #$00                                ; compare with the free cell id
        beq __ram_malloc_check_for_free_consec_page    ; if equal check the page disponibility
        bne __ram_malloc_check_for_free_page_inc_index  ; if not repeat operation with the next cell

        __ram_malloc_check_for_free_consec_page:
            clc                                 ; clear carry
            clv                                 ; clear overflow
            
            inc RAM_PAGE_ALLOC_POINTER          ; increment pointer
            cpy RAM_PAGE_ALLOC_POINTER         ; compare the pointer with the desire page ammount
            bcc __ram_malloc_check_for_free_page_inc_index   ; if its less then continue to check 
            beq __ram_malloc_page_disp                      ; if it's equal then go to alloc page routine
            jmp __ram_end_malloc

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
            adc RAM_LB_CALCULUS_ADDRESS
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

;
; a -> pre-assinged ram id to clear
;
;
;   EX: if I use malloc to alloc 12 page of consecutive ram space,
;   it will return an id to identify the memory space for a programm.
;   Let's say that the id would be "A", if loaded program end, i need to call
;   the "free" function to check the ram space with the "A" id and unflag it
;   for make the assigned ram page ready to be used by another program. So to set it free
;
;
__ram_free:
    ldx #$00                                ; load x with 0
    stx RAM_PAGE_ALLOC_POINTER              ; reset page pointer

    jmp __ram_free_start_check_for_id       ; start checking for id
    
    __ram_free_start_check_for_id_inc_x:
        clc                                 ; clear carry
        inx                                 ; inc x
        cpx #$80                            ; compar with the max page limit
        bcs __ram_free_end_free             ; if it's greater than limit

    __ram_free_start_check_for_id:
        cmp RAM_PAGE_ALLOCATED_FLAG_HEAD,x      ; compare the page cell value with the a content
        beq __ram_free_unflag_id                ; if it's equal then unflag-it
        bne __ram_free_start_check_for_id_inc_x ; if not start check nex one

    __ram_free_unflag_id:
        pha                                     ; push a int oaccumulator
        lda #$00                                ; load 0 into accumulator
        sta RAM_PAGE_ALLOCATED_FLAG_HEAD,x      ; reset flag
        inc RAM_PAGE_ALLOC_POINTER              ; increment page count
        pla                                     ; pull a from accumulator
        jmp __ram_free_start_check_for_id_inc_x     ; continue to check

    __ram_free_end_free:
        lda #$00                                ; load a with 0
        cmp RAM_PAGE_ALLOC_POINTER              ; check if any page was deleted
        beq __ram_free_end_no_such_segment      ; if not then strigger a new kernel state
        bne __ram_free_end_call                 ; if so simply exit

        __ram_free_end_no_such_segment:
            lda #FREE_NO_SUCH_SEGMENT           ; load free error 
            sta KERNEL_STATE
        
    __ram_free_end_call:
        rts                                     ; return from subroutine



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
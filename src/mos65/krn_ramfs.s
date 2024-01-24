;
;   Kernel RAM management
;


RAM_PAGE=$50 ; the amount of space in total


LOW_MEMORY_BANKING_REGISTER=$83
HIGHT_MEMORY_BANKING_REGISTER=$84


__K_RAM_MN:
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

;
;   y -> the amount of page to allocate
;

__ram_malloc:
    clv ; clear overflow
    tya ; save y content
    pha ; into stack

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
    cmp RAM_FREE_PAGE           ; compare with the free page counter
    bcc __ram_malloc_page_disp  ; check if is less
    bcs __ram_out_of_request    ; check if the request is greater than free pages

    __ram_out_of_request:       ; out of request -> abort operation
        jmp __ram_end_malloc

    __ram_malloc_page_disp:

    adc RAM_ALLOCATED_PAGE      ; update allocated page counter
    sta RAM_ALLOCATED_PAGE      ; store it 

    lda RAM_FREE_PAGE           ; load free counter
    sec                         ; set carry
    sbc RAM_ALLOCATED_PAGE      ; subtract with allocated page counter
    sta RAM_FREE_PAGE           ; save free page counter


    ldx RAM_PAGE_ALLOC_POINTER  ; load current ram vector flag pointer

    clc                         ; clear carrt
    lda RAM_PAGE_ALLOC_POINTER         ;  Assiign progressive id to any program in memory
    adc #"A"
    cmp #$00                    ; check for error
    bvs __out_of_memory_error
    bvc __ram_malloc_assing_vector

    __out_of_memory_error:
        lda #KERNEL_OUT_OF_MEMORY_ERROR     ; kernel panic -> out of memory error
        sta KERNEL_STATE
        jmp __K_BOOT

    __ram_malloc_assing_vector:
        sta RAM_ALLOCATED_ASSINGED_LETTER    ; assing letter
    __ram_malloc_start_alloc:               
        cpy #$00                            ;  "while() do {} function"
        beq __end_flag_allocation
        bne __ram_malloc_assing_vector_continue

        __ram_malloc_assing_vector_continue:
            sta RAM_PAGE_ALLOCATED_FLAG_HEAD,x
            dey
            inx
            jmp __ram_malloc_start_alloc

        __end_flag_allocation:

            txy

            pla ; pull  from stack and put it into a
            
            sta RAM_RETURNED_ALLOCATED_LAST_POINTER_LB ; just store it for a moment

            sta RAM_PAGE_ALLOC_POINTER  ; use this store cache data
            tya 
            sec ; set carry flag
            sbc RAM_PAGE_ALLOC_POINTER 

            sty RAM_PAGE_ALLOC_POINTER  ; store the real page allocated pointer

            clc
            clv

            tax ; transfer the original starting point
            pha ; for stay safe it's better to save the original starting point


            ; save pointer
            lda RAM_PAGE_POINTER_VECTOR_HEAD,x
            sta RAM_RETURNED_ALLOCATED_FIRST_POINTER_LB
            inx
            lda RAM_PAGE_POINTER_VECTOR_HEAD,x
            sta RAM_RETURNED_ALLOCATED_FIRST_POINTER_HB
            inx

            pla 
            tax
            lda RAM_RETURNED_ALLOCATED_LAST_POINTER_LB  ; grab the saved index
            asl             ; multiply by 2 
            bvs __ram_malloc_error_out_of_memory_index
            bvc __ram_malloc_calculate_index

            __ram_malloc_error_out_of_memory_index:
                lda #MALLOC_OUT_OF_MEMORY_INDEX
                sta KERNEL_STATE
                jmp __K_BOOT

            __ram_malloc_calculate_index:
                sec
                sbc #$01

                clc
                clv

                stx RAM_RETURNED_ALLOCATED_LAST_POINTER_LB ; store the x index just for now here
                adc RAM_RETURNED_ALLOCATED_LAST_POINTER_LB  ; calculate the index of 
                
                bvs __ram_malloc_error_out_of_memory_index
                bvc __ram_malloc_get_pointer_by_index

                __ram_malloc_get_pointer_by_index:

                    tax ; transfer the resoult index into x


                    ; save pointer
                    lda RAM_PAGE_POINTER_VECTOR_HEAD,x
                    sta RAM_RETURNED_ALLOCATED_LAST_POINTER_LB
                    inx
                    lda RAM_PAGE_POINTER_VECTOR_HEAD,x
                    sta RAM_RETURNED_ALLOCATED_LAST_POINTER_HB

    __ram_end_malloc:
        rts 


__ram_free:
    rts



__k_ram_init_set_page_address_table:

    rts                             ; return to initialization
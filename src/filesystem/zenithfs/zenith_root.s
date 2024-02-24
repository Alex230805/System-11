


_Z_ROOT:
    jsr __zenith_get_info

    ;
    ;   code here
    ;
    
    rts


;
;   main routine
;

__zenith_set_address:
    jmp (ZENITH_DEVICE_CALL_1_lb)
    jmp __zenith_bus_error

__zenith_read_data:
    jmp (ZENITH_DEVICE_CALL_2_lb)
    jmp __zenith_bus_error


__zenith_write_data:
    jmp (ZENITH_DEVICE_CALL_3_lb)
    jmp __zenith_bus_error

__zenith_get_info:
    jmp (ZENITH_DEVICE_CALL_4_lb)
    jmp __zenith_bus_error



__zenith_bus_error:
    lda #ZENITH_BUS_ERROR
    sta KERNEL_STATE
    jmp __kernel_panic
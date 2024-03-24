;
;	Onboard display driver
;


OBD_DATA_PORT=$9f



_obd_init:
	; init onboard display
	nop ; some code here

	lda #$ff
	sta OB_DISPLAY_ENABLE
	rts


_obd_write:
	; print content present in the system bus
	ldx SYSTEM_BUS_READ_POINTER

	_obd_start_write:
	    cpx SYSYEM_BUS_WRITE_POINTER
	    beq _obd_end_write

	    lda SYSTEM_BUS_START,x 
	    sta OBD_DATA_PORT
	    inx
	    jmp _obd_start_write
	    
	_obd_end_write:
	    rts

_obd_write_char:
	sta OBD_DATA_PORT	; store character in the display data port
	rts

_obd_write_with_adr:
	; write a line using an indirect pointer
	ldx #$00
	
	_obd_start_write_with_adr:
	lda (DYN_POINTER), x
	cmp $00
	beq _obd_end_write_with_adr
	sta OBD_DATA_PORT
	inx
	jmp _obd_start_write_with_adr
	
	_obd_end_write_with_adr:
	rts

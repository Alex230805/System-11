;
;
;   Zenith SubCall Manager
;
;

; File routine

z_fopen=$00
z_fclose=$01

z_fread=$02
z_fwrite=$03

z_remove=$0C

;
;
;   x -> zenith sub call instruction
;
;

__ZENITH_SUBCALL:
    cpx #z_fopen
    beq __z_fopen_redirect
    bne __z_check_fclose

    __z_fopen_redirect:
        jsr __z_fopen
        jmp __ZENITH_SUBCALL_END

    __z_check_fclose:
    cpx #z_fclose
    beq __z_fclose_redirect
    bne __z_check_fread

    __z_fclose_redirect:
        jsr __z_fclose
        jmp __ZENITH_SUBCALL_END

    __z_check_fread:
    cpx #z_fread
    beq __z_fread_redirect
    bne __z_check_fwrite

    __z_fread_redirect:
        jsr __z_fread
        jmp __ZENITH_SUBCALL_END

    __z_check_fwrite:
    cpx #z_fwrite
    beq __z_fwrite_redirect
    bne __z_check_remove

    __z_fwrite_redirect:
        jsr __z_fwrite
        jmp __ZENITH_SUBCALL_END

    __z_check_remove:
    cpx #z_remove
    beq __z_remove_redirect
    bne __ZENITH_SUBCALL_END

    __z_remove_redirect:
        jsr __z_remove
        jmp __ZENITH_SUBCALL_END

__ZENITH_SUBCALL_END:
    rts
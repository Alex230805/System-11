

K_KALL=__K_DYN_SYSCALL

;
;
; kernel dynamic call dispacement: based on x register
;
;
;   Example of system call:
;   
;   ...
;   ldx #SYS_0
;   jsr K_KALL
;   ...
;

__K_DYN_SYSCALL:

    __syscall_0_check:
    cpx #SYS_0
    beq __syscall_0_redirect
    bne __syscall_1_check
    __syscall_0_redirect:
        jsr SYSTEM_CALL_0
        jmp __K_DYN_SYSCALL_END

    __syscall_1_check:
    cpx #SYS_1
    beq __syscall_1_redirect
    bne __syscall_2_check
    __syscall_1_redirect:
        jsr SYSTEM_CALL_1
        jmp __K_DYN_SYSCALL_END
    
    __syscall_2_check:
    cpx #SYS_2
    beq __syscall_2_redirect
    bne __syscall_3_check
    __syscall_2_redirect:
        jsr SYSTEM_CALL_2
        jmp __K_DYN_SYSCALL_END
    
    __syscall_3_check:
    cpx #SYS_3
    beq __syscall_3_redirect
    bne __syscall_4_check
    __syscall_3_redirect:
        jsr SYSTEM_CALL_3
        jmp __K_DYN_SYSCALL_END
    
    __syscall_4_check:
    cpx #SYS_4
    beq __syscall_4_redirect
    bne __syscall_5_check
    __syscall_4_redirect:
        jsr SYSTEM_CALL_4
        jmp __K_DYN_SYSCALL_END

    __syscall_5_check:
    cpx #SYS_5
    beq __syscall_5_redirect
    bne __syscall_6_check
    __syscall_5_redirect:
        jsr SYSTEM_CALL_5
        jmp __K_DYN_SYSCALL_END

    __syscall_6_check:
    cpx #SYS_6
    beq __syscall_6_redirect
    bne __syscall_7_check
    __syscall_6_redirect:
        jsr SYSTEM_CALL_6
        jmp __K_DYN_SYSCALL_END

    __syscall_7_check:
    cpx #SYS_7
    beq __syscall_7_redirect
    bne __syscall_8_check
    __syscall_7_redirect:
        jsr SYSTEM_CALL_7
        jmp __K_DYN_SYSCALL_END

    __syscall_8_check:
    cpx #SYS_8
    beq __syscall_8_redirect
    bne __syscall_9_check
    __syscall_8_redirect:
        jsr SYSTEM_CALL_8
        jmp __K_DYN_SYSCALL_END

    __syscall_9_check:
    cpx #SYS_9
    beq __syscall_9_redirect
    bne __syscall_10_check
    __syscall_9_redirect:
        jsr SYSTEM_CALL_9
        jmp __K_DYN_SYSCALL_END

    __syscall_10_check:
    cpx #SYS_10
    beq __syscall_10_redirect
    bne __syscall_11_check
    __syscall_10_redirect:
        jsr SYSTEM_CALL_10
        jmp __K_DYN_SYSCALL_END

    __syscall_11_check:
    cpx #SYS_11
    beq __syscall_11_redirect
    bne __syscall_12_check
    __syscall_11_redirect:
        jsr SYSTEM_CALL_11
        jmp __K_DYN_SYSCALL_END

    __syscall_12_check:
    cpx #SYS_12
    beq __syscall_12_redirect
    jmp __K_DYN_SYSCALL_END
    __syscall_12_redirect:
        jsr SYSTEM_CALL_12
        jmp __K_DYN_SYSCALL_END

__K_DYN_SYSCALL_END:
    rts             ; return from kernel call


;
;   Kernel state for device and OS
;
KERNEL_WR_0=$01 ; kernel boot error

KERNEL_STATE=$8B        ; kernel state

;
; Kernel variable header
;

KERNEL_VARIABLE_HEADER=$8C  ;kernel variable header

; keyboard device state

KEYBOARD_ENABLE=KERNEL_VARIABLE_HEADER

; cf card driver pointer and vector for device state

V_CF_CARD_LB=KERNEL_VARIABLE_HEADER+1          ; low byte address variable
V_CF_CARD_HB=KERNEL_VARIABLE_HEADER+2          ; hight byte address variable
V_CF_CARD_XLB=KERNEL_VARIABLE_HEADER+3         ; x low byte address variable
V_CF_CARD_XHB=KERNEL_VARIABLE_HEADER+4         ; x hight byte address variable

CF_CARD_ENABLE=KERNEL_VARIABLE_HEADER+5

CF_CARD_MOUNTED=KERNEL_VARIABLE_HEADER+6       ; cf card flag for check if the device is mounted

; Graphics card status

FX_CARD_ENABLE=KERNEL_VARIABLE_HEADER+7        ; enable status for cf card
FX_CARD_STATUS=KERNEL_VARIABLE_HEADER+8        ; status for cf card

FX_CARD_MEMORY_MOUNTED=KERNEL_VARIABLE_HEADER+9 ; memory bank mounted currently
FX_CARD_MEMORY_FREE=KERNEL_VARIABLE_HEADER+10   ; total memory free for current bank
FX_CARD_MEMORY_USED=KERNEL_VARIABLE_HEADER+11 ; memory used from current bank
FX_CARD_PAGE_FREE=KERNEL_VARIABLE_HEADER+12 ; page free (all bank)


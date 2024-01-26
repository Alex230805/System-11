;
;   Kernel state for device and OS
;
KERNEL_WR_0=$01 ; kernel boot error

KERNEL_ZENITH_INIT_ERROR=$11    ; zenith boot error
ZENITH_BUS_ERROR=$12            ; zenith bus error

KERNEL_OUT_OF_MEMORY_ERROR=$21  ; kernel out of memory error
MALLOC_OUT_OF_MEMORY_INDEX=$22  ; kernel out of memory index

FREE_NO_SUCH_SEGMENT=$23        ; semment not found

KERNEL_MISSING_POINTER=$31 ; missing general kernel return pointer 


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

V_SD_CARD_LB=KERNEL_VARIABLE_HEADER+7
V_SD_CARD_HB=KERNEL_VARIABLE_HEADER+8

SD_CARD_ENABLE=KERNEL_VARIABLE_HEADER+9

SD_CARD_MOUNTED=KERNEL_VARIABLE_HEADER+10

; Graphics card status

FX_CARD_ENABLE=KERNEL_VARIABLE_HEADER+11        ; enable status for cf card
FX_CARD_STATUS=KERNEL_VARIABLE_HEADER+12        ; status for cf card

FX_CARD_MEMORY_MOUNTED=KERNEL_VARIABLE_HEADER+13 ; memory bank mounted currently
FX_CARD_MEMORY_FREE=KERNEL_VARIABLE_HEADER+14   ; total memory free for current bank
FX_CARD_MEMORY_USED=KERNEL_VARIABLE_HEADER+15 ; memory used from current bank
FX_CARD_PAGE_FREE=KERNEL_VARIABLE_HEADER+16 ; page free (all bank)

; Zenith FS


ZENITH_VARIABLE_HEADER=$0302        ; ZENITH_VARIABLE_HEADER

ZENITH_CURRENT_DEVIE=ZENITH_VARIABLE_HEADER ; current device mounted ( enumerative rom A to D )

ZENITH_DEVICE_CALL_1_lb=ZENITH_VARIABLE_HEADER+1   ; pointer to device driver function: type "set address"
ZENITH_DEVICE_CALL_1_hb=ZENITH_VARIABLE_HEADER+2

ZENITH_DEVICE_CALL_2_lb=ZENITH_VARIABLE_HEADER+3   ; pointer to device dirver function: type "read data"
ZENITH_DEVICE_CALL_2_hb=ZENITH_VARIABLE_HEADER+4

ZENITH_DEVICE_CALL_3_lb=ZENITH_VARIABLE_HEADER+5   ; pointer to device function: type "write data"
ZENITH_DEVICE_CALL_3_hb=ZENITH_VARIABLE_HEADER+6

ZENITH_DEVICE_CALL_4_lb=ZENITH_VARIABLE_HEADER+7   ; pointer to device function:type "get info"
ZENITH_DEVICE_CALL_4_hb=ZENITH_VARIABLE_HEADER+8

;
;
;   Zenith device function call
;
;   When a device get initialized a flag will be set true to identify a ready state.
;   Zenith FS is going to read those set of flag to identify a list of avaliable devices and any device
;   have to copy his function pointer into the zenith abstraction layer list ( in order by activation ).
;

ZENITH_ABLAYER_DEVICE_CALL_HEAD=ZENITH_VARIABLE_HEADER+9   ; zenith list to store device function
ZENITH_ABLAYER_DEVICE_CALL_END=ZENITH_VARIABLE_HEADER+41

ZENITH_ABLAYER_DEVICE_CALL_POINTER=ZENITH_VARIABLE_HEADER+42 ; current pointer of list

; Zenith device: identify driver with a letter

ZENTH_CF_ID=ZENITH_VARIABLE_HEADER+43
ZENITH_SD_ID=ZENITH_VARIABLE_HEADER+44

ZENITH_DEVICE_LIST=ZENITH_VARIABLE_HEADER+45 ; variable to identify used device letter

;
;
;   Ram Management
;
;



SYSTEM_RAM_HEADER=ZENITH_VARIABLE_HEADER+45

RAM_ALLOCATED_PAGE=SYSTEM_RAM_HEADER

RAM_FREE_PAGE=SYSTEM_RAM_HEADER+1

RAM_PAGE_PADDING_HEAD=SYSTEM_RAM_HEADER+2

RAM_PAGE_PADDING_VECTOR_END=SYSTEM_RAM_HEADER+162

RAM_PAGE_ALLOCATED_FLAG_HEAD=SYSTEM_RAM_HEADER+163

RAM_PAGE_ALLOCATED_FLAG_END=SYSTEM_RAM_HEADER+243

RAM_PAGE_ALLOC_POINTER=SYSTEM_RAM_HEADER+244


; the user when write a program need to carry this out 

RAM_RETURNED_ALLOCATED_FIRST_POINTER_LB=SYSTEM_RAM_HEADER+245
RAM_RETURNED_ALLOCATED_FIRST_POINTER_HB=SYSTEM_RAM_HEADER+246

RAM_RETURNED_ALLOCATED_LAST_POINTER_LB=SYSTEM_RAM_HEADER+247
RAM_RETURNED_ALLOCATED_LAST_POINTER_HB=SYSTEM_RAM_HEADER+248

RAM_ALLOCATED_ASSINGED_LETTER=SYSTEM_RAM_HEADER+248

RAM_LB_CALCULUS_ADDRESS=SYSTEM_RAM_HEADER+249
RAM_HB_CALCULUS_ADDRESS=SYSTEM_RAM_HEADER+250

;
;
;
;
;
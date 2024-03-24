; fx card driver


FX_CARD_PORT=$8A 

FX_CARD_CONTROL_PORT = $8B


__fx_card_init:
    nop
    ;
    ;
    ;   update the graphics card variable 
    ;
    ;   enable graphics card
    ;
    rts

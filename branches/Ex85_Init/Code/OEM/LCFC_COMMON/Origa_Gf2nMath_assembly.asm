
//$INCLUDE( XC800.inc )

MEMTYPE_CODE    EQU        0xFF
MEMTYPE_XDATA   EQU        0x01


        PUBLIC  gf2n_add_asm
        PUBLIC  _gf2n_add_asm

 
; V001 2008-01-09 SK: *
; V002 2011-09-07 ENE Nick
;   //ENE nick +>
gf2n_add_asm:
_gf2n_add_asm:
        ;--- prepare DPTRs                      ;
        mov     DPH,R2                          ; to Operand1.
        mov     DPL,R3                          ;
        ;--- do summing                         ;
        mov     R3,#17                          ; 17 bytes to process.
 _gf2n_add_asm__loop:                           ;
        movx    A,@DPTR
        inc     DPTR
        mov     R2,A                            ; get Operand 1
        push    DPH
        push    DPL
        mov     DPH,R4
        mov     DPL,R5
        movx     A,@DPTR
        xrl     A,R2                            ; Operand1.
        mov     R2,A                            ;
        inc     DPTR
        mov     R4,DPH
        mov     R5,DPL
        mov     DPH,R6
        mov     DPL,R7
        mov     A,R2
        movx    @DPTR,A
        inc     DPTR
        mov     R6,DPH
        mov     R7,DPL
        pop     DPL
        pop     DPH
        djnz    R3,_gf2n_add_asm__loop          ; check for all bytes done.
        ret
//ENE nick <+


END



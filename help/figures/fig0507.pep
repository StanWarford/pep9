;File: fig0507.pep
;Computer Systems, Fourth Edition
;Figure 5.7
;
         LDWA    0x000D,d    ;A <- first number
         ADDA    0x000F,d    ;Add the two numbers
         ORA     0x0011,d    ;Convert sum to character
         STBA    0xFC49,d    ;Output the character
         STOP
         .WORD   5           ;Decimal 5
         .WORD   3           ;Decimal 3
         .WORD   0x0030      ;Mask for ASCII char
         .END

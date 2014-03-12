;File: fig0513.pep
;Computer Systems, Fourth Edition
;Figure 5.13
;
         BR      0x0009      ;Branch around data
         .WORD   0xFFFE      ;First
         .BYTE   0x00        ;Second
         .BYTE   'U'         ;Third
         .WORD   1136        ;Fourth
;
         DECO    0x0003,d    ;Interpret First as decimal
         CHARO   '\n',i
         DECO    0x0005,d    ;Interpret Second and Third as decimal
         CHARO   '\n',i
         CHARO   0x0006,d    ;Interpret Third as character
         CHARO   0x0008,d    ;Interpret Fourth as character
         STOP
         .END

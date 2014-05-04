;File: fig0511.pep
;Computer Systems, Fourth Edition
;Figure 5.11
;
         BR      0x0005      ;Branch around data
         .BLOCK  2           ;Storage for one integer
;
         DECI    0x0003,d    ;Get the number
         DECO    0x0003,d    ;and output it
         LDBA    ' ',i       ;Output " + 1 = "
         STBA    0xFC49,d
         LDBA    '+',i
         STBA    0xFC49,d
         LDBA    ' ',i
         STBA    0xFC49,d
         LDBA    '1',i
         STBA    0xFC49,d
         LDBA    ' ',i
         STBA    0xFC49,d
         LDBA    '=',i
         STBA    0xFC49,d
         LDBA    ' ',i
         STBA    0xFC49,d
         LDWA    0x0003,d    ;A <- the number
         ADDA    1,i         ;Add one to it
         STWA    0x0003,d    ;Store the sum
         DECO    0x0003,d    ;Output the sum
         STOP
         .END

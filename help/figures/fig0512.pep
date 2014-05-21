;File: fig0512.pep
;Computer Systems, Fifth Edition
;Figure 5.12
;
         BR      0x000D      ;Branch around data
         .BLOCK  2           ;Storage for one integer
         .ASCII  " + 1 = \x00"
;
         DECI    0x0003,d    ;Get the number
         DECO    0x0003,d    ;and output it
         STRO    0x0005,d    ;Output " + 1 = "
         LDWA    0x0003,d    ;A <- the number
         ADDA    1,i         ;Add one to it
         STWA    0x0003,d    ;Store the sum
         DECO    0x0003,d    ;Output the sum
         STOP
         .END

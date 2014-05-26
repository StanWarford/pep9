;File: fig0512.pep
;Computer Systems, Fifth edition
;Figure 5.12
;
         BR      0x0005      ;Branch around data
         .BLOCK  2           ;Storage for one integer
;
         DECI    0x0003,d    ;Get the number
         DECO    0x0003,d    ;and output it
         STRO    0x001B,d    ;Output " + 1 = "
         LDWA    0x0003,d    ;A <- the number
         ADDA    1,i         ;Add one to it
         STWA    0x0003,d    ;Store the sum
         DECO    0x0003,d    ;Output the sum
         STOP                
         .ASCII  " + 1 = \x00"
         .END                  

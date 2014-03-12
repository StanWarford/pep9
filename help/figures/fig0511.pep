;File: fig0511.pep
;Computer Systems, Fourth Edition
;Figure 5.11
;
         BR      0x0005      ;Branch around data
         .BLOCK  2           ;Storage for one integer
;
         DECI    0x0003,d    ;Get the number
         DECO    0x0003,d    ;and output it
         CHARO   ' ',i       ;Output " + 1 = "
         CHARO   '+',i       
         CHARO   ' ',i       
         CHARO   '1',i       
         CHARO   ' ',i       
         CHARO   '=',i       
         CHARO   ' ',i       
         LDA     0x0003,d    ;A <- the number
         ADDA    1,i         ;Add one to it
         STA     0x0003,d    ;Store the sum
         DECO    0x0003,d    ;Output the sum
         STOP                
         .END                  

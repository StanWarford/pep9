;File: fig0506.pep
;Computer Systems, Fourth Edition
;Figure 5.6
;
         CHARI   0x000D,d    ;Input first character
         CHARI   0x000E,d    ;Input second character
         CHARO   0x000E,d    ;Output second character
         CHARO   0x000D,d    ;Output first character
         STOP                
         .BLOCK  1           ;Storage for first char
         .BLOCK  1           ;Storage for second char
         .END                  

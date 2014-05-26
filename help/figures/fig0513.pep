;File: fig0513.pep
;Computer Systems, Fifth edition
;Figure 5.13
;
         BR      0x0009      ;Branch around data
         .WORD   0xFFFE      ;First
         .BYTE   0x00        ;Second
         .BYTE   'U'         ;Third
         .WORD   1136        ;Fourth
;
         DECO    0x0003,d    ;Interpret First as decimal
         LDBA    '\n',i      
         STBA    0xFC16,d    
         DECO    0x0005,d    ;Interpret Second and Third as decimal
         STBA    0xFC16,d    
         LDBA    '\n',i      
         HEXO    0x0005,d    ;Interpret Second and Third as hexadecimal
         LDBA    '\n',i      
         STBA    0xFC16,d    
         LDBA    0x0006,d    ;Interpret Third as character
         STBA    0xFC16,d    
         LDBA    0x0008,d    ;Interpret Fourth as character
         STBA    0xFC16,d    
         STOP                
         .END                  

;File: fig0601.pep
;Computer Systems, Fifth edition
;Figure 6.1
;
         LDBA    'B',i       ;move B to stack
         STBA    -1,s        
         LDBA    'M',i       ;move M to stack
         STBA    -2,s        
         LDBA    'W',i       ;move W to stack
         STBA    -3,s        
         LDWA    335,i       ;move 335 to stack
         STWA    -5,s        
         LDBA    'i',i       ;move i to stack
         STBA    -6,s        
         SUBSP   6,i         ;push 6 bytes onto stack
         LDBA    5,s         ;output B
         STBA    charOut,d   
         LDBA    4,s         ;output M
         STBA    charOut,d   
         LDBA    3,s         ;output W
         STBA    charOut,d   
         DECO    1,s         ;output 335
         LDBA    0,s         ;output i
         STBA    charOut,d   
         ADDSP   6,i         ;pop 6 bytes off stack
         STOP                
         .END                  
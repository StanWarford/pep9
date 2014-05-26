;File: fig0601.pep
;Computer Systems, Fifth edition
;Figure 6.1
;
         LDBA    'B',i       ;push B
         STBA    -1,s        
         LDBA    'M',i       ;push M
         STBA    -2,s        
         LDBA    'W',i       ;push W
         STBA    -3,s        
         LDWA    335,i       ;push 335
         STWA    -5,s        
         LDBA    'i',i       ;push i
         STBA    -6,s        
         SUBSP   6,i         ;6 bytes on the run-time stack
         LDBA    5,s         ;output B
         STBA    charOut,d   
         LDBA    4,s         ;output M
         STBA    charOut,d   
         LDBA    3,s         ;output W
         STBA    charOut,d   
         DECO    1,s         ;output 335
         LDBA    0,s         ;output i
         STBA    charOut,d   
         ADDSP   6,i         ;deallocate stack storage
         STOP                
         .END                  

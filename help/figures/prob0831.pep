;File: prob0831.pep
;Computer Systems, Fifth edition
;Problem 8.31
;
         LDWA    5,i         ;Push 5
         STWA    -2,s        
         LDWA    9,i         ;Push 9
         STWA    -4,s        
         SUBSP   4,i         
         STACKADD            ;Add 5 + 9
         DECO    0,s         ;Output top of stack
         ADDSP   2,i         ;Pop the sum
         STOP                
         .END                  

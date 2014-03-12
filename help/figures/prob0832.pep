;File: prob0832.pep
;Computer Systems, Fourth edition
;Problem 8.32
;
         LDA     5,i         ;Push 5
         STA     -2,s        
         LDA     9,i         ;Push 9
         STA     -4,s        
         SUBSP   4,i         
         STACKADD            ;Add 5 + 9
         DECO    0,s         ;Output top of stack
         ADDSP   2,i         ;Pop the sum
         STOP                
         .END                  

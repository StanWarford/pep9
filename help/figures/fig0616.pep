;File: fig0616.pep
;Computer Systems, Fourth edition
;Figure 6.16
         BR      main        
n1:      .BLOCK  2           ;#2d
n2:      .BLOCK  2           ;#2d
n3:      .BLOCK  2           ;#2d
;
main:    DECI    n2,d        
         DECI    n3,d        
         LDA     n2,d        
         CPA     n3,d        
         BRLT    L1          
         DECI    n1,d        
         LDA     n1,d        
         CPA     n3,d        
         BRLT    L7          
         BR      L6          
         STA     n3,d        
L1:      DECI    n1,d        
         LDA     n2,d        
         CPA     n1,d        
         BRLT    L5          
         DECO    n1,d        
         DECO    n2,d        
L2:      DECO    n3,d        
         STOP                
L3:      DECO    n2,d        
         DECO    n3,d        
         BR      L9          
L4:      DECO    n1,d        
         DECO    n2,d        
         STOP                
         STA     n1,d        
L5:      LDA     n3,d        
         CPA     n1,d        
         BRLT    L3          
         DECO    n2,d        
         DECO    n1,d        
         BR      L2          
L6:      DECO    n3,d        
         LDA     n1,d        
         CPA     n2,d        
         BRLT    L4          
         BR      L8          
L7:      DECO    n1,d        
         DECO    n3,d        
         DECO    n2,d        
         STOP                
L8:      DECO    n2,d        
L9:      DECO    n1,d        
         STOP                
         .END                  
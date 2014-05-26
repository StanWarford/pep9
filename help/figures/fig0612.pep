;File: fig0612.pep
;Computer Systems, Fifth edition
;Figure 6.12
;
         BR      main        
cop:     .BLOCK  2           ;global variable #2d
driver:  .BLOCK  2           ;global variable #2d
;
main:    LDWA    0,i         ;cop = 0
         STWA    cop,d       
         LDWA    40,i        ;driver = 40
         STWA    driver,d    
do:      LDWA    cop,d       ;cop += 25
         ADDA    25,i        
         STWA    cop,d       
         LDWA    driver,d    ;driver += 20
         ADDA    20,i        
         STWA    driver,d    
while:   LDWA    cop,d       ;while (cop < driver)
         CPWA    driver,d    
         BRLT    do          
         DECO    cop,d       ;printf("%d", cop)
         STOP                
         .END                  
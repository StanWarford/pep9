;File: fig0612.pep
;Computer Systems, Fourth edition
;Figure 6.12
;
         BR      main        
cop:     .BLOCK  2           ;global variable #2d
driver:  .BLOCK  2           ;global variable #2d
;
main:    LDA     0,i         ;cop = 0
         STA     cop,d       
         LDA     40,i        ;driver = 40
         STA     driver,d    
do:      LDA     cop,d       ;   cop += 25
         ADDA    25,i        
         STA     cop,d       
         LDA     driver,d    ;   driver += 20
         ADDA    20,i        
         STA     driver,d    
while:   LDA     cop,d       ;while (cop < driver)
         CPA     driver,d    
         BRLT    do          
         DECO    cop,d       ;cout << cop
         STOP                
         .END                  
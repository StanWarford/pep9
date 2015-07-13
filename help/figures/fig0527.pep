;File: fig0527.pep
;Computer Systems, Fifth edition
;Figure 5.27
;
         BR      main        
bonus:   .EQUATE 10          ;constant
exam1:   .BLOCK  2           ;global variable #2d
exam2:   .BLOCK  2           ;global variable #2d
score:   .BLOCK  2           ;global variable #2d
;
main:    DECI    exam1,d     ;scanf("%d %d", &exam1, &exam2)
         DECI    exam2,d     
         LDWA    exam1,d     ;score = (exam1 + exam2) / 2 + bonus
         ADDA    exam2,d     
         ASRA                
         ADDA    bonus,i     
         STWA    score,d     
         STRO    msg,d       ;printf("score = %d\n", score)
         DECO    score,d     
         LDBA    '\n',i      
         STBA    charOut,d   
         STOP                
msg:     .ASCII  "score = \x00"
         .END                  

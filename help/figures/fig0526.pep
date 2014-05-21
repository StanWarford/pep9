;File: fig0526.pep
;Computer Systems, Fifth edition
;Figure 5.26
;
         BR      main        
bonus:   .EQUATE 5           ;constant
exam1:   .BLOCK  2           ;global variable #2d
exam2:   .BLOCK  2           ;global variable #2d
score:   .BLOCK  2           ;global variable #2d
;
main:    DECI    exam1,d     ;scanf("%d %d", &exam1, &exam2)
         DECI    exam2,d     
         LDWA    exam1,d     ;score = (exam1
         ADDA    exam2,d     ;   + exam2)
         ASRA                ;   / 2
         ADDA    bonus,i     ;   + bonus
         STWA    score,d     
         STRO    msg,d       ;printf("score = %d\n", score)
         DECO    score,d     
         LDBA    '\n',i      
         STBA    charOut,d   
         STOP                
msg:     .ASCII  "score = \x00"
         .END                  
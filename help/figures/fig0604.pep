;File: fig0604.pep
;Computer Systems, Fifth edition
;Figure 6.4
;
         BR      main        
bonus:   .EQUATE 10          ;constant
exam1:   .EQUATE 4           ;local variable #2d
exam2:   .EQUATE 2           ;local variable #2d
score:   .EQUATE 0           ;local variable #2d
;
main:    SUBSP   6,i         ;push #exam1 #exam2 #score
         DECI    exam1,s     ;scanf("%d %d", &exam1, &exam2)
         DECI    exam2,s     
         LDWA    exam1,s     ;score = (exam1 + exam2) / 2 + bonus
         ADDA    exam2,s     
         ASRA                
         ADDA    bonus,i     
         STWA    score,s     
         STRO    msg,d       ;printf("score = %d\n", score)
         DECO    score,s     
         LDBA    '\n',i      
         STBA    charOut,d   
         ADDSP   6,i         ;pop #score #exam2 #exam1
         STOP                
msg:     .ASCII  "score = \x00"
         .END                  
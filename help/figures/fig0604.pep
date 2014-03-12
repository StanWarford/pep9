;File: fig0604.pep
;Computer Systems, Fourth edition
;Figure 6.4
;
         BR      main        
bonus:   .EQUATE 5           ;constant
exam1:   .EQUATE 4           ;local variable #2d
exam2:   .EQUATE 2           ;local variable #2d
score:   .EQUATE 0           ;local variable #2d
;
main:    SUBSP   6,i         ;allocate #exam1 #exam2 #score
         DECI    exam1,s     ;cin >> exam1
         DECI    exam2,s     ;   >> exam2
         LDA     exam1,s     ;score = (exam1
         ADDA    exam2,s     ;   + exam2)
         ASRA                ;   / 2
         ADDA    bonus,i     ;   + bonus
         STA     score,s     
         STRO    msg,d       ;cout << "score = "
         DECO    score,s     ;   << score
         CHARO   '\n',i      ;   << endl
         ADDSP   6,i         ;deallocate #score #exam2 #exam1
         STOP                
msg:     .ASCII  "score = \x00"
         .END                  
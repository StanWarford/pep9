;File: fig0606.pep
;Computer Systems, Fourth edition
;Figure 6.6
;
         BR      main        
number:  .EQUATE 0           ;local variable #2d
;
main:    SUBSP   2,i         ;allocate #number
         DECI    number,s    ;cin >> number
if:      LDA     number,s    ;if (number < 0)
         BRGE    endIf       
         LDA     number,s    ;   number = -number
         NEGA                
         STA     number,s    
endIf:   DECO    number,s    ;cout << number
         ADDSP   2,i         ;deallocate #number
         STOP                
         .END                  
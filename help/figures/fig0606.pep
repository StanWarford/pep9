;File: fig0606.pep
;Computer Systems, Fifth edition
;Figure 6.6
;
         BR      main        
number:  .EQUATE 0           ;local variable #2d
;
main:    SUBSP   2,i         ;push #number
         DECI    number,s    ;scanf("%d", &number)
if:      LDWA    number,s    ;if (number < 0)
         BRGE    endIf       
         LDWA    number,s    ;number = -number
         NEGA                
         STWA    number,s    
endIf:   DECO    number,s    ;printf("%d", number)
         ADDSP   2,i         ;pop #number
         STOP                
         .END                  
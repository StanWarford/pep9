;File: fig0610.pep
;Computer Systems, Fifth edition
;Figure 6.10
;
         BR      main        
letter:  .BLOCK  1           ;global variable #1c
;
main:    LDBA    charIn,d    ;scanf("%c", &letter)
         STBA    letter,d    
while:   LDBA    letter,d    ;while (letter != '*')
         CPBA    '*',i       
         BREQ    endWh       
if:      CPBA    ' ',i       ;if (letter == ' ')
         BRNE    else        
         LDBA    '\n',i      ;printf("\n")
         STBA    charOut,d   
         BR      endIf       
else:    STBA    charOut,d   ;printf("%c", letter)
endIf:   LDBA    charIn,d    ;scanf("%c", &letter)
         STBA    letter,d    
         BR      while       
endWh:   STOP                
         .END                  
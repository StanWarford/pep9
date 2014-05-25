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
         LDBA    '[',i       ;printf("[%c]\n", letter)
         STBA    charOut,d   
         LDBA    letter,d    
         STBA    charOut,d   
         LDBA    ']',i       
         STBA    charOut,d   
         LDBA    '\n',i      
         STBA    charOut,d   
         LDBA    charIn,d    ;scanf("%c", &letter)
         STBA    letter,d    
         BR      while       
endWh:   STOP                
         .END                  
;File: fig0610.pep
;Computer Systems, Fourth edition
;Figure 6.10
;
         BR      main        
letter:  .BLOCK  1           ;global variable #1c
;
main:    CHARI   letter,d    ;cin >> letter
         LDA     0x0000,i    
while:   LDBYTEA letter,d    ;while (letter != '*')
         CPA     '*',i       
         BREQ    endWh       
         CHARO   letter,d    ;   cout << letter
         CHARI   letter,d    ;   cin >> letter
         BR      while       
endWh:   STOP                
         .END                  
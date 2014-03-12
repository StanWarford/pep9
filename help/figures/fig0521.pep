;File: fig0521.pep
;Computer Systems, Fourth edition
;Figure 5.21
;
         BR      main        
ch:      .BLOCK  1           ;global variable #1c
j:       .BLOCK  2           ;global variable #2d
;
main:    CHARI   ch,d        ;cin >> ch
         DECI    j,d         ;   >> j
         LDA     j,d         ;j += 5
         ADDA    5,i         
         STA     j,d         
         LDBYTEA ch,d        ;ch++
         ADDA    1,i         
         STBYTEA ch,d        
         CHARO   ch,d        ;cout << ch
         CHARO   '\n',i      ;   << endl
         DECO    j,d         ;   << j
         CHARO   '\n',i      ;   << endl
         STOP                
         .END                  
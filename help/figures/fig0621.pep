;File: fig0621.pep
;Computer Systems, Fourth edition
;Figure 6.21
;
         BR      main        
numPts:  .BLOCK  2           ;global variable #2d
value:   .BLOCK  2           ;global variable #2d
j:       .BLOCK  2           ;global variable #2d
;
;******* void printBar (int n)
n:       .EQUATE 4           ;formal parameter #2d
k:       .EQUATE 0           ;local variable #2d
printBar:SUBSP   2,i         ;allocate #k
         LDA     1,i         ;for (k = 1
         STA     k,s         
for1:    CPA     n,s         ;k <= n
         BRGT    endFor1     
         CHARO   '*',i       ;   cout << '*'
         LDA     k,s         ;k++)
         ADDA    1,i         
         STA     k,s         
         BR      for1        
endFor1: CHARO   '\n',i      ;cout << endl
         RET2                ;deallocate #k, pop retAddr
;
;******* main ()
main:    DECI    numPts,d    ;cin >> numPts
         LDA     1,i         ;for (j = 1
         STA     j,d         
for2:    CPA     numPts,d    ;j <= numPts
         BRGT    endFor2     
         DECI    value,d     ;   cin >> value
         LDA     value,d     ;   call by value
         STA     -2,s        
         SUBSP   2,i         ;   push #n
         CALL    printBar    ;   push retAddr
         ADDSP   2,i         ;   pop #n
         LDA     j,d         ;j++)
         ADDA    1,i         
         STA     j,d         
         BR      for2        
endFor2: STOP                
         .END                  
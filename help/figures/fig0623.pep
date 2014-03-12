;File: fig0623.pep
;Computer Systems, Fourth edition
;Figure 6.23
;
         BR      main        
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
numPts:  .EQUATE 4           ;local variable #2d
value:   .EQUATE 2           ;local variable #2d
j:       .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;allocate #numPts #value #j
         DECI    numPts,s    ;cin >> numPts
         LDA     1,i         ;for (j = 1
         STA     j,s         
for2:    CPA     numPts,s    ;j <= numPts
         BRGT    endFor2     
         DECI    value,s     ;   cin >> value
         LDA     value,s     ;   call by value
         STA     -2,s        
         SUBSP   2,i         ;   push #n
         CALL    printBar    ;   push retAddr
         ADDSP   2,i         ;   pop #n
         LDA     j,s         ;j++)
         ADDA    1,i         
         STA     j,s         
         BR      for2        
endFor2: ADDSP   6,i         ;deallocate #j #value #numPts
         STOP                
         .END                  
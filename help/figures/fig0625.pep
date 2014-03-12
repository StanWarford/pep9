;File: fig0625.pep
;Computer Systems, Fourth edition
;Figure 6.25
;
         BR      main        
;
;******* int binomCoeff (int n, int k)
retVal:  .EQUATE 10          ;returned value #2d
n:       .EQUATE 8           ;formal parameter #2d
k:       .EQUATE 6           ;formal parameter #2d
y1:      .EQUATE 2           ;local variable #2d
y2:      .EQUATE 0           ;local variable #2d
binCoeff:SUBSP   4,i         ;allocate #y1 #y2
if:      LDA     k,s         ;if ((k == 0)
         BREQ    then        
         LDA     n,s         ;|| (n == k))
         CPA     k,s         
         BRNE    else        
then:    LDA     1,i         ;return 1
         STA     retVal,s    
         RET4                ;deallocate #y2 #y1, pop retAddr
else:    LDA     n,s         ;push n - 1
         SUBA    1,i         
         STA     -4,s        
         LDA     k,s         ;push k
         STA     -6,s        
         SUBSP   6,i         ;push #retVal #n #k
         CALL    binCoeff    
ra2:     ADDSP   6,i         ;pop #k #n #retVal
         LDA     -2,s        ;y1 = binomCoeff (n - 1, k)
         STA     y1,s        
         LDA     n,s         ;push n - 1
         SUBA    1,i         
         STA     -4,s        
         LDA     k,s         ;push k - 1
         SUBA    1,i         
         STA     -6,s        
         SUBSP   6,i         ;push #retVal #n #k
         CALL    binCoeff    
ra3:     ADDSP   6,i         ;pop #k #n #retVal
         LDA     -2,s        ;y2 = binomCoeff (n - 1, k - 1)
         STA     y2,s        
         LDA     y1,s        ;return y1 + y2
         ADDA    y2,s        
         STA     retVal,s    
endIf:   RET4                ;deallocate #y2 #y1, pop retAddr
;
;******* main ()
main:    STRO    msg,d       ;cout << "binCoeff (3, 1) = "
         LDA     3,i         ;push 3
         STA     -4,s        
         LDA     1,i         ;push 1
         STA     -6,s        
         SUBSP   6,i         ;push #retVal #n #k
         CALL    binCoeff    
ra1:     ADDSP   6,i         ;pop #k #n #retVal
         DECO    -2,s        ;<< binCoeff (3, 1)
         CHARO   '\n',i      ;cout << endl
         STOP                
msg:     .ASCII  "binCoeff (3, 1) = \x00"
         .END                  
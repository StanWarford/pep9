;File: fig0625.pep
;Computer Systems, Fifth edition
;Figure 6.25
;
         BR      main        
;
;******* int binomCoeff(int n, int k)
retVal:  .EQUATE 10          ;return value #2d
n:       .EQUATE 8           ;formal parameter #2d
k:       .EQUATE 6           ;formal parameter #2d
y1:      .EQUATE 2           ;local variable #2d
y2:      .EQUATE 0           ;local variable #2d
binCoeff:SUBSP   4,i         ;push #y1 #y2
if:      LDWA    k,s         ;if ((k == 0)
         BREQ    then        
         LDWA    n,s         ;|| (n == k))
         CPWA    k,s         
         BRNE    else        
then:    LDWA    1,i         ;return 1
         STWA    retVal,s    
         ADDSP   4,i         ;pop #y2 #y1
         RET                 
else:    LDWA    n,s         ;move n - 1
         SUBA    1,i         
         STWA    -4,s        
         LDWA    k,s         ;move k
         STWA    -6,s        
         SUBSP   6,i         ;push #retVal #n #k
         CALL    binCoeff    ;binCoeff(n - 1, k)
ra2:     ADDSP   6,i         ;pop #k #n #retVal
         LDWA    -2,s        ;y1 = binomCoeff(n - 1, k)
         STWA    y1,s        
         LDWA    n,s         ;move n - 1
         SUBA    1,i         
         STWA    -4,s        
         LDWA    k,s         ;move k - 1
         SUBA    1,i         
         STWA    -6,s        
         SUBSP   6,i         ;push #retVal #n #k
         CALL    binCoeff    ;binomCoeff(n - 1, k - 1)
ra3:     ADDSP   6,i         ;pop #k #n #retVal
         LDWA    -2,s        ;y2 = binomCoeff(n - 1, k - 1)
         STWA    y2,s        
         LDWA    y1,s        ;return y1 + y2
         ADDA    y2,s        
         STWA    retVal,s    
endIf:   ADDSP   4,i         ;pop #y2 #y1
         RET                 
;
;******* main()
main:    STRO    msg,d       ;printf("binCoeff(3, 1) = %d\n",
         LDWA    3,i         ;move 3
         STWA    -4,s        
         LDWA    1,i         ;move 1
         STWA    -6,s        
         SUBSP   6,i         ;push #retVal #n #k
         CALL    binCoeff    ;binCoeff(3, 1)
ra1:     ADDSP   6,i         ;pop #k #n #retVal
         DECO    -2,s        
         LDBA    '\n',i      
         STBA    charOut,d   
         STOP                
msg:     .ASCII  "binCoeff(3, 1) = \x00"
         .END                  
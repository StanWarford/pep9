;File: fig0621.pep
;Computer Systems, Fifth edition
;Figure 6.21
;
         BR      main        
numPts:  .BLOCK  2           ;global variable #2d
value:   .BLOCK  2           ;global variable #2d
j:       .BLOCK  2           ;global variable #2d
;
;******* void printBar(int n)
n:       .EQUATE 4           ;formal parameter #2d
k:       .EQUATE 0           ;local variable #2d
printBar:SUBSP   2,i         ;push #k
         LDWA    1,i         ;for (k = 1
         STWA    k,s         
for1:    CPWA    n,s         ;k <= n
         BRGT    endFor1     
         LDBA    '*',i       ;printf("*")
         STBA    charOut,d   
         LDWA    k,s         ;k++)
         ADDA    1,i         
         STWA    k,s         
         BR      for1        
endFor1: LDBA    '\n',i      ;printf("\n")
         STBA    charOut,d   
         ADDSP   2,i         ;pop #k
         RET                 
;
;******* main()
main:    DECI    numPts,d    ;scanf("%d", &numPts)
         LDWA    1,i         ;for (j = 1
         STWA    j,d         
for2:    CPWA    numPts,d    ;j <= numPts
         BRGT    endFor2     
         DECI    value,d     ;scanf("%d", &value)
         LDWA    value,d     ;move value
         STWA    -2,s        
         SUBSP   2,i         ;push #n
         CALL    printBar    ;printBar(value)
         ADDSP   2,i         ;pop #n
         LDWA    j,d         ;j++)
         ADDA    1,i         
         STWA    j,d         
         BR      for2        
endFor2: STOP                
         .END                  
;File: fig0623.pep
;Computer Systems, Fifth edition
;Figure 6.23
;
         BR      main        
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
numPts:  .EQUATE 4           ;local variable #2d
value:   .EQUATE 2           ;local variable #2d
j:       .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;push #numPts #value #j
         DECI    numPts,s    ;scanf("%d", &numPts)
         LDWA    1,i         ;for (j = 1
         STWA    j,s         
for2:    CPWA    numPts,s    ;j <= numPts
         BRGT    endFor2     
         DECI    value,s     ;scanf("%d", &value)
         LDWA    value,s     ;move value
         STWA    -2,s        
         SUBSP   2,i         ;push #n
         CALL    printBar    ;printBar(value)
         ADDSP   2,i         ;pop #n
         LDWA    j,s         ;j++)
         ADDA    1,i         
         STWA    j,s         
         BR      for2        
endFor2: ADDSP   6,i         ;pop #j #value #numPts
         STOP                
         .END                  
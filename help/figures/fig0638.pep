;File: fig0638.pep
;Computer Systems, Fifth edition
;Figure 6.38
;
         BR      main
;
;******* getVect(int v[], int *n)
v:       .EQUATE 6           ;formal parameter #2h
n:       .EQUATE 4           ;formal parameter #2h
j:       .EQUATE 0           ;local variable #2d
getVect: SUBSP   2,i         ;push storage for #j
         DECI    n,sf        ;scanf("%d", n)
         LDWX    0,i         ;for (j = 0
         STWX    j,s
for1:    CPWX    n,sf        ;   j < *n
         BRGE    endFor1
         ASLX                ;   an integer is two bytes
         DECI    v,sfx       ;   scanf("%d", &v[j])
         LDWX    j,s         ;   j++)
         ADDX    1,i
         STWX    j,s
         BR      for1
endFor1: ADDSP   2,i         ;pop #j
;
;******* putVect(int v[], int n)
v2:      .EQUATE 6           ;formal parameter #2h
n2:      .EQUATE 4           ;formal parameter #2d
j2:      .EQUATE 0           ;local variable #2d
putVect: SUBSP   2,i         ;push storage for #j2
         LDWX    0,i         ;for (j = 0
         STWX    j2,s
for2:    CPWX    n2,s        ;   j < n
         BRGE    endFor2
         ASLX                ;   an integer is two bytes
         DECO    v2,sfx      ;   printf("%d ", v[j])
         LDBA    ' ',i
         STBA    charOut,d
         LDWX    j2,s        ;   i++)
         ADDX    1,i
         STWX    j2,s
         BR      for2
endFor2: LDBA    '\n',i      ;printf("\n")
         STBA    charOut,d
         ADDSP   2,i         ;pop #j2
         RET
;
;******* main()
vector:  .EQUATE 2           ;local variable #2d8a
numItms: .EQUATE 0           ;local variable #2d
main:    SUBSP   18,i        ;push storage for #vector #numItms
         MOVSPA              ;push address of vector
         ADDA    vector,i
         STWA    -2,s
         MOVSPA              ;push address of numItms
         ADDA    numItms,i
         STWA    -4,s
         SUBSP   4,i         ;push #v #n
         CALL    getVect     ;getVect(vector, &numItms)
         ADDSP   4,i         ;pop #n #v
         MOVSPA              ;push address of vector
         ADDA    vector,i
         STWA    -2,s
         LDWA    numItms,s   ;push value of numItms
         STWA    -4,s
         SUBSP   4,i         ;push #v2 #n2
         CALL    putVect     ;putVect(vector, numItms)
         ADDSP   4,i         ;pop #n2 #v2
         ADDSP   18,i        ;pop #numItms #vector
         STOP
         .END

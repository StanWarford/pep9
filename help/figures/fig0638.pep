;File: fig0638.pep
;Computer Systems, Fourth edition
;Figure 6.38
;
         BR      main        
;
;******* getVect (int v[], int& n)
v:       .EQUATE 6           ;formal parameter #2h
n:       .EQUATE 4           ;formal parameter #2h
j:       .EQUATE 0           ;local variable #2d
getVect: SUBSP   2,i         ;allocate #j
         DECI    n,sf        ;cin >> n
         LDX     0,i         ;for (j = 0
         STX     j,s         
for1:    CPX     n,sf        ;   j < n
         BRGE    endFor1     
         ASLX                ;   an integer is two bytes
         DECI    v,sxf       ;   cin >> v[j]
         LDX     j,s         ;   j++)
         ADDX    1,i         
         STX     j,s         
         BR      for1        
endFor1: RET2                ;pop #j and retAddr
;
;******* putVect (int v[], int n)
v2:      .EQUATE 6           ;formal parameter #2h
n2:      .EQUATE 4           ;formal parameter #2d
j2:      .EQUATE 0           ;local variable #2d
putVect: SUBSP   2,i         ;allocate #j2
         LDX     0,i         ;for (j = 0
         STX     j2,s        
for2:    CPX     n2,s        ;   j < n
         BRGE    endFor2     
         ASLX                ;   an integer is two bytes
         DECO    v2,sxf      ;   cout << v[j]
         CHARO   ' ',i       ;      << ' '
         LDX     j2,s        ;   i++)
         ADDX    1,i         
         STX     j2,s        
         BR      for2        
endFor2: CHARO   '\n',i      ;cout << endl
         RET2                ;pop #j2 and retAddr
;
;******* main ()
vector:  .EQUATE 2           ;local variable #2d8a
numItms: .EQUATE 0           ;local variable #2d
main:    SUBSP   18,i        ;allocate #vector #numItms
         MOVSPA              ;push address of vector
         ADDA    vector,i    
         STA     -2,s        
         MOVSPA              ;push address of numItms
         ADDA    numItms,i   
         STA     -4,s        
         SUBSP   4,i         ;push #v #n
         CALL    getVect     ;getVect (vector, numItms)
         ADDSP   4,i         ;pop #n #v
         MOVSPA              ;push address of vector
         ADDA    vector,i    
         STA     -2,s        
         LDA     numItms,s   ;push value of numItms
         STA     -4,s        
         SUBSP   4,i         ;push #v2 #n2
         CALL    putVect     ;putVect (vector, numItms)
         ADDSP   4,i         ;pop #n2 #v2
         ADDSP   18,i        ;deallocate #numItms #vector
         STOP                
         .END                  
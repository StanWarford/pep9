;File: fig0636.pep
;Computer Systems, Fifth edition
;Figure 6.36
;
         BR      main        
;
;******* main ()
vector:  .EQUATE 2           ;local variable #2d4a
j:       .EQUATE 0           ;local variable #2d
main:    SUBSP   10,i        ;push #vector #j
         LDWX    0,i         ;for (j = 0
         STWX    j,s         
for1:    CPWX    4,i         ;j < 4
         BRGE    endFor1     
         ASLX                ;two bytes per integer
         DECI    vector,sx   ;scanf("%d", &vector[j])
         LDWX    j,s         ;j++)
         ADDX    1,i         
         STWX    j,s         
         BR      for1        
endFor1: LDWX    3,i         ;for (j = 3
         STWX    j,s         
for2:    CPWX    0,i         ;j >= 0
         BRLT    endFor2     
         DECO    j,s         ;printf("%d %d\n", j, vector[j])
         LDBA    ' ',i       
         STBA    charOut,d   
         ASLX                ;two bytes per integer
         DECO    vector,sx   
         LDBA    '\n',i      
         STBA    charOut,d   
         LDWX    j,s         ;j--)
         SUBX    1,i         
         STWX    j,s         
         BR      for2        
endFor2: ADDSP   10,i        ;pop #j #vector
         STOP                
         .END                  
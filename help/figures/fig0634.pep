;File: fig0634.pep
;Computer Systems, Fifth edition
;Figure 6.34
;
         BR      main        
vector:  .BLOCK  8           ;global variable #2d4a
j:       .BLOCK  2           ;global variable #2d
;
;******* main()
main:    LDWX    0,i         ;for (j = 0
         STWX    j,d         
for1:    CPWX    4,i         ;j < 4
         BRGE    endFor1     
         ASLX                ;two bytes per integer
         DECI    vector,x    ;scanf("%d", &vector[j])
         LDWX    j,d         ;j++)
         ADDX    1,i         
         STWX    j,d         
         BR      for1        
endFor1: LDWX    3,i         ;for (j = 3
         STWX    j,d         
for2:    CPWX    0,i         ;j >= 0
         BRLT    endFor2     
         DECO    j,d         ;printf("%d %d\n", j, vector[j])
         LDBA    ' ',i       
         STBA    charOut,d   
         ASLX                ;two bytes per integer
         DECO    vector,x    
         LDBA    '\n',i      
         STBA    charOut,d   
         LDWX    j,d         ;j--)
         SUBX    1,i         
         STWX    j,d         
         BR      for2        
endFor2: STOP                
         .END                  
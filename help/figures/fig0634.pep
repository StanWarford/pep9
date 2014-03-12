;File: fig0634.pep
;Computer Systems, Fourth edition
;Figure 6.34
;
         BR      main        
vector:  .BLOCK  8           ;global variable #2d4a
j:       .BLOCK  2           ;global variable #2d
;
;******* main ()
main:    LDX     0,i         ;for (j = 0
         STX     j,d         
for1:    CPX     4,i         ;   j < 4
         BRGE    endFor1     
         ASLX                ;   an integer is two bytes
         DECI    vector,x    ;   cin >> vector[j]
         LDX     j,d         ;   j++)
         ADDX    1,i         
         STX     j,d         
         BR      for1        
endFor1: LDX     3,i         ;for (j = 3
         STX     j,d         
for2:    CPX     0,i         ;   j >= 0
         BRLT    endFor2     
         DECO    j,d         ;   cout << j
         CHARO   ' ',i       ;      << ' '
         ASLX                ;   an integer is two bytes
         DECO    vector,x    ;      << vector[j]
         CHARO   '\n',i      ;      << endl
         LDX     j,d         ;   j--)
         SUBX    1,i         
         STX     j,d         
         BR      for2        
endFor2: STOP                
         .END                  
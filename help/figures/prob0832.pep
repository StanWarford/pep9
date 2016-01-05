;File: prob0831.pep
;Computer Systems, Fifth edition
;Problem 8.32
;
         BR      main        ;branch around data
first:   .BLOCK  2           ;global variable
second:  .BLOCK  2           ;global variable
xor:     .BLOCK  2           ;global variable
flags:   .BLOCK  1           ;flags from XOR
;
;******* main()
main:    DECI    first,d     ;input first
         DECI    second,d    ;input second
         LDWA    first,d     ;move first
         XORA    second,d    ;XOR accumulator
         STWA    xor,d       ;store result into xor
         MOVFLGA             ;store flags
         STBA    flags,d     
         STRO    msg1,d      
         HEXO    first,d     
         STRO    msg2,d      
         HEXO    second,d    
         STRO    msg3,d      
         HEXO    xor,d       
         LDBA    '\n',i      
         STBA    charOut,d   
testN:   LDBA    flags,d     ;test N
         ANDA    0x0008,i    
         BREQ    outN0       
         STRO    msgN1,d     ;output "N = 1"
         BR      testZ       
outN0:   STRO    msgN0,d     ;output "N = 0"
testZ:   LDBA    flags,d     ;test Z
         ANDA    0x0004,i    
         BREQ    outZ0       
         STRO    msgZ1,d     ;output "Z = 1"
         BR      halt        
outZ0:   STRO    msgZ0,d     ;output "Z = 0"
halt:    STOP                
msg1:    .ASCII  "     \x00" 
msg2:    .ASCII  "\nXOR  \x00"
msg3:    .ASCII  "\n---------\n     \x00"
msgN0:   .ASCII  "\nN = 0\n\x00"
msgN1:   .ASCII  "\nN = 1\n\x00"
msgZ0:   .ASCII  "Z = 0\n\x00"
msgZ1:   .ASCII  "Z = 1\n\x00"
msgV0:   .ASCII  "V = 0\n\x00"
msgV1:   .ASCII  "V = 1\n\x00"
msgC0:   .ASCII  "C = 0\n\x00"
msgC1:   .ASCII  "C = 1\n\x00"
         .END                  

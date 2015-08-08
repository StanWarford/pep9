;File: prob0826.pep
;Computer Systems, Fifth edition
;Problem 8.26 test program
;
         BR      main        ;Branch around data
num:     .BLOCK  2           ;Global variable
flags:   .BLOCK  1           ;Flags from double shift
;
main:    DECI    num,d       ;Input decimal value
         STRO    msg1,d      ;Output original value
         DECO    num,d       
         LDBA    '\n',i      
         STBA    charOut,d   
         LDWA    num,d       ;Test ASL2 instruction
         ASL2                
         STWA    num,d       
         MOVFLGA             ;Store flags
         STBA    flags,d     
         STRO    msg2,d      ;Output shifted value
         DECO    num,d       
         LDBA    '\n',i      
         STBA    charOut,d   
testN:   LDBA    flags,d     ;Test N
         ANDA    0x0008,i    
         BREQ    outN0       
         STRO    msgN1,d     ;Output "N = 1"
         BR      testZ       
outN0:   STRO    msgN0,d     ;Output "N = 0"
testZ:   LDBA    flags,d     ;Test Z
         ANDA    0x0004,i    
         BREQ    outZ0       
         STRO    msgZ1,d     ;Output "Z = 1"
         BR      testV       
outZ0:   STRO    msgZ0,d     ;Output "Z = 0"
testV:   LDBA    flags,d     ;Test V
         ANDA    0x0002,i    
         BREQ    outV0       
         STRO    msgV1,d     ;Output "V = 1"
         BR      testC       
outV0:   STRO    msgV0,d     ;Output "V = 0"
testC:   LDBA    flags,d     ;Test C
         ANDA    0x0001,i    
         BREQ    outC0       
         STRO    msgC1,d     ;Output "C = 1"
         BR      halt        
outC0:   STRO    msgC0,d     ;Output "C = 0"
halt:    STOP                
msg1:    .ASCII  "Original value = \x00"
msg2:    .ASCII  "Shifted value = \x00"
msgN0:   .ASCII  "N = 0\n\x00"
msgN1:   .ASCII  "N = 1\n\x00"
msgZ0:   .ASCII  "Z = 0\n\x00"
msgZ1:   .ASCII  "Z = 1\n\x00"
msgV0:   .ASCII  "V = 0\n\x00"
msgV1:   .ASCII  "V = 1\n\x00"
msgC0:   .ASCII  "C = 0\n\x00"
msgC1:   .ASCII  "C = 1\n\x00"
         .END                  
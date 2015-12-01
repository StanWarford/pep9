;File: prob0831.pep
;Computer Systems, Fifth edition
;Problem 8.31
;
         BR      main        ;branch around data
first:   .BLOCK  2           ;global variable
second:  .BLOCK  2           ;global variable
sum:     .BLOCK  2           ;global variable
flags:   .BLOCK  1           ;flags from addition
;
;******* main()
main:    DECI    first,d     ;input first
         DECI    second,d    ;input second
         LDWA    first,d     ;move first
         STWA    -2,s        
         LDWA    second,d    ;move second
         STWA    -4,s        
         SUBSP   4,i         ;push first second
         STKADD              ;stack add
         MOVFLGA             ;store flags
         STBA    flags,d     
         ADDSP   2,i         ;pop sum
         DECO    first,d     ;output "first + second = "
         STRO    msg1,d      
         DECO    second,d    
         STRO    msg2,d      
         DECO    -2,s        ;output top of stack
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
         BR      testV       
outZ0:   STRO    msgZ0,d     ;output "Z = 0"
testV:   LDBA    flags,d     ;test V
         ANDA    0x0002,i    
         BREQ    outV0       
         STRO    msgV1,d     ;output "V = 1"
         BR      testC       
outV0:   STRO    msgV0,d     ;output "V = 0"
testC:   LDBA    flags,d     ;test C
         ANDA    0x0001,i    
         BREQ    outC0       
         STRO    msgC1,d     ;output "C = 1"
         BR      halt        
outC0:   STRO    msgC0,d     ;output "C = 0"
halt:    STOP                
msg1:    .ASCII  " + \x00"   
msg2:    .ASCII  " = \x00"   
msgN0:   .ASCII  "N = 0\n\x00"
msgN1:   .ASCII  "N = 1\n\x00"
msgZ0:   .ASCII  "Z = 0\n\x00"
msgZ1:   .ASCII  "Z = 1\n\x00"
msgV0:   .ASCII  "V = 0\n\x00"
msgV1:   .ASCII  "V = 1\n\x00"
msgC0:   .ASCII  "C = 0\n\x00"
msgC1:   .ASCII  "C = 1\n\x00"
         .END                  
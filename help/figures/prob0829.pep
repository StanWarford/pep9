;File: prob0829.pep
;Computer Systems, Fifth edition
;Problem 8.29
;
         BR      main        
num1:    .BLOCK  2           ;num1
num1ad:  .ADDRSS num1        ;Address of num1
num1adad:.ADDRSS num1ad      ;Address of address of num1
num2:    .BLOCK  2           ;num2
num2ad:  .ADDRSS num2        ;Address of num2
num2adad:.ADDRSS num2ad      ;Address of address of num2
;
main:    LDWA    27,i        ;Load accumulator
         STWADI              ;Store num1 double indirect
         .ADDRSS num1adad    
         LDWA    34,i        ;Load accumulator
         STWADI              ;Store num2 double indirect
         .ADDRSS num2adad    
         STRO    msg1,d      
         DECO    num1,d      ;Output num1
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg2,d      
         DECO    num2,d      ;Output num2
         LDBA    '\n',i      
         STBA    charOut,d   
         STOP                
msg1:    .ASCII  "num1 should equal 27. num1: \x00"
msg2:    .ASCII  "num2 should equal 34. num2: \x00"
         .END                  
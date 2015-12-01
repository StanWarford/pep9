;File: prob0829.pep
;Computer Systems, Fifth edition
;Problem 8.29
;
         BR      main        
num1:    .BLOCK  2           ;num1
num1ad:  .ADDRSS num1        ;address of num1
num1adad:.ADDRSS num1ad      ;address of address of num1
num2:    .BLOCK  2           ;num2
num2ad:  .ADDRSS num2        ;address of num2
num2adad:.ADDRSS num2ad      ;address of address of num2
;
main:    LDWA    27,i        ;load accumulator
         STWADI              ;store num1 double indirect
         .ADDRSS num1adad    
         LDWA    34,i        ;load accumulator
         STWADI              ;store num2 double indirect
         .ADDRSS num2adad    
         STRO    msg1,d      
         DECO    num1,d      ;output num1
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg2,d      
         DECO    num2,d      ;output num2
         LDBA    '\n',i      
         STBA    charOut,d   
         STOP                
msg1:    .ASCII  "num1 should equal 27. num1: \x00"
msg2:    .ASCII  "num2 should equal 34. num2: \x00"
         .END                  

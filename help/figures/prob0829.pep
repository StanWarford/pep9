;File: prob0829.pep
;Computer Systems, Fifth edition
;Problem 8.29
;
         BR      main       
num1:    .BLOCK  2          ;num1
num1ad:  .ADDRSS num1       ;Address of num1
num1adad:.ADDRSS num1ad     ;Address of address of num1
num2:    .BLOCK  2          ;num2
num2ad:  .ADDRSS num2       ;Address of num2
num2adad:.ADDRSS num2ad     ;Address of address of num2
main:    LDA     27,i       ;Load accumulator
         STADI              ;Store num1 double indirect
         .ADDRSS num1adad
         LDA     34,i       ;Load accumulator
         STADI              ;Store num2 double indirect
         .ADDRSS num2adad
         DECO    num1,d     ;Output num1
         CHARO   ' ',i
         DECO    num2,d     ;Output num2
         CHARO   '\n',i
         STOP
         .END

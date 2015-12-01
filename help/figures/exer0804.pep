;File: exer0804.pep
;Computer Systems, Fifth edition
;Exercise 8.4
;
         BR      main        ;branch around data
num:     .BLOCK  2           ;global variable
main:    DECI    num,d       ;input decimal value
         DECO    num,d       ;output decimal value
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg,d       ;output message
         STOP                
msg:     .ASCII  "That's all.\n\x00"
         .END                  
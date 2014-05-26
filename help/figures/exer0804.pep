;File: exer0804.pep
;Computer Systems, Fifth edition
;Exercise 8.4
;
         BR      main        ;Branch around data
num:     .BLOCK  2           ;Global variable
main:    DECI    num,d       ;Input decimal value
         DECO    num,d       ;Output decimal value
         LDBA    '\n',i
         STBA    charOut,d
         STRO    msg,d       ;Output message
         STOP                
msg:     .ASCII  "That's all.\n\x00"
         .END                  

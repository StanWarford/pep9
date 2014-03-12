;File: exer0804.pep
;Computer Systems, Fourth edition
;Exercise 8.4
;
         BR      main        ;Branch around data
num:     .BLOCK  2           ;Global variable
main:    DECI    num,d       ;Input decimal value
         DECO    num,d       ;Output decimal value
         CHARO   '\n',i      
         STRO    msg,d       ;Output message
         STOP                
msg:     .ASCII  "That's all.\n\x00"
         .END                  

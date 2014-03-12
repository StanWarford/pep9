;File: fig0515.pep
;Computer Systems, Fourth edition
;Figure 5.15
;
         BR      main        ;Branch around data
num:     .BLOCK  2           ;Storage for one integer #2d
msg:     .ASCII  " + 1 = \x00"
;
main:    DECI    num,d       ;Get the number
         DECO    num,d       ;and output it
         STRO    msg,d       ;Output ' + 1 = '
         LDA     num,d       ;A <- the number
         ADDA    1,i         ;Add one to it
         STA     num,d       ;Store the sum
         DECO    num,d       ;Output the sum
         STOP                
         .END                  

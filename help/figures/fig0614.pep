;File: fig0614.pep
;Computer Systems, Fifth edition
;Figure 6.14
;
         BR      main        
j:       .EQUATE 0           ;local variable #2d
;
main:    SUBSP   2,i         ;push #j
         LDWA    0,i         ;for (j = 0
         STWA    j,s         
for:     CPWA    3,i         ;j < 3
         BRGE    endFor      
         STRO    msg,d       ;printf("j = %d\n", j)
         DECO    j,s         
         LDBA    '\n',i      
         STBA    charOut,d   
         LDWA    j,s         ;j++)
         ADDA    1,i         
         STWA    j,s         
         BR      for         
endFor:  ADDSP   2,i         ;pop #j
         STOP                
msg:     .ASCII  "j = \x00"  
         .END                  
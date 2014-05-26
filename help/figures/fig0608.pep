;File: fig0608.pep
;Computer Systems, Fifth edition
;Figure 6.8
;
         BR      main        
limit:   .EQUATE 100         ;constant
num:     .EQUATE 0           ;local variable #2d
;
main:    SUBSP   2,i         ;push #num
         DECI    num,s       ;scanf("%d", &num)
if:      LDWA    num,s       ;if (num >= limit)
         CPWA    limit,i     
         BRLT    else        
         STRO    msg1,d      ;printf("high\n")
         BR      endIf       
else:    STRO    msg2,d      ;printf("low\n")
endIf:   ADDSP   2,i         ;pop #num
         STOP                
msg1:    .ASCII  "high\n\x00"
msg2:    .ASCII  "low\n\x00" 
         .END                  
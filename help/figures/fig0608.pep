;File: fig0608.pep
;Computer Systems, Fourth edition
;Figure 6.8
;
         BR      main        
limit:   .EQUATE 100         ;constant
num:     .EQUATE 0           ;local variable #2d
;
main:    SUBSP   2,i         ;allocate #num
         DECI    num,s       ;cin >> num
if:      LDA     num,s       ;if (num >= limit)
         CPA     limit,i     
         BRLT    else        
         STRO    msg1,d      ;   cout << "high"
         BR      endIf       
else:    STRO    msg2,d      ;   cout << "low"
endIf:   ADDSP   2,i         ;deallocate #num
         STOP                
msg1:    .ASCII  "high\x00"  
msg2:    .ASCII  "low\x00"   
         .END
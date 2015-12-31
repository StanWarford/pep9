;File: fig0646.pep
;Computer Systems, Fifth edition
;Figure 6.46
;
         BR      main        
first:   .EQUATE 0           ;struct field #1c
last:    .EQUATE 1           ;struct field #1c
age:     .EQUATE 2           ;struct field #2d
gender:  .EQUATE 4           ;struct field #1c
bill:    .BLOCK  5           ;globals #first #last #age #gender
;
;******* main()
main:    LDWX    first,i     ;scanf("%c%c%d %c", &bill.first,
         LDBA    charIn,d    
         STBA    bill,x      
         LDWX    last,i      ;&bill.last,
         LDBA    charIn,d    
         STBA    bill,x      
         LDWX    age,i       ;&bill.age,
         DECI    bill,x      
         LDWX    gender,i    ;&bill.gender)
         LDBA    charIn,d    
         STBA    bill,x      
         STRO    msg0,d      ;printf("Initials: %c%c\n",
         LDWX    first,i     ;bill.first,
         LDBA    bill,x      
         STBA    charOut,d   
         LDWX    last,i      ;bill.last)
         LDBA    bill,x      
         STBA    charOut,d   
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg1,d      ;printf("Age:  %d\n",
         LDWX    age,i       ;bill.age)
         DECO    bill,x      
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg2,d      ;printf("Gender: ")
         LDWX    gender,i    ;if (bill.gender == 'm')
         LDBA    bill,x      
         CPBA    'm',i       
         BRNE    else        
         STRO    msg3,d      ;printf("male\n")
         BR      endIf       
else:    STRO    msg4,d      ;printf("female\n")
endIf:   STOP                
msg0:    .ASCII  "Initials: \x00"
msg1:    .ASCII  "Age: \x00" 
msg2:    .ASCII  "Gender: \x00"
msg3:    .ASCII  "male\n\x00"
msg4:    .ASCII  "female\n\x00"
         .END                  

;File: fig0645.pep
;Computer Systems, Fourth edition
;Figure 6.45
;
         BR      main        
first:   .EQUATE 0           ;struct field #1c
last:    .EQUATE 1           ;struct field #1c
age:     .EQUATE 2           ;struct field #2d
gender:  .EQUATE 4           ;struct field #1c
bill:    .BLOCK  5           ;global variable #first #last #age #gender
;
;******* main ()
main:    LDX     first,i     ;cin >> bill.first
         CHARI   bill,x      
         LDX     last,i      ;   >>bill.last
         CHARI   bill,x      
         LDX     age,i       ;   >>bill.age
         DECI    bill,x      
         LDX     gender,i    ;   >>bill.gender
         CHARI   bill,x      
         STRO    msg0,d      ;cout << "Initials: "
         LDX     first,i     ;   << bill.first
         CHARO   bill,x      
         LDX     last,i      ;   << bill.last
         CHARO   bill,x      
         CHARO   '\n',i      ;   << endl
         STRO    msg1,d      ;cout << "Age: "
         LDX     age,i       ;   << bill.age
         DECO    bill,x      
         CHARO   '\n',i      ;   << endl;
         STRO    msg2,d      ;cout << "Gender: "
         LDX     gender,i    ;if (bill.gender == 'm')
         LDA     0,i         
         LDBYTEA bill,x      
         CPA     'm',i       
         BRNE    else        
         STRO    msg3,d      ;   cout << "male\n"
         BR      endIf       
else:    STRO    msg4,d      ;   cout << "female\n"
endIf:   STOP                
msg0:    .ASCII  "Initials: \x00"
msg1:    .ASCII  "Age: \x00" 
msg2:    .ASCII  "Gender: \x00"
msg3:    .ASCII  "male\n\x00"
msg4:    .ASCII  "female\n\x00"
         .END                  
;File: fig0640.pep
;Computer Systems, Fourth edition
;Figure 6.40
;
         BR      main        
;
;******* main ()
guess:   .EQUATE 0           ;local variable #2d
main:    SUBSP   2,i         ;allocate #guess
         STRO    msgIn,d     ;cout << "Pick a number 0..3: "
         DECI    guess,s     ;cin >> Guess
         LDX     guess,s     ;switch (Guess)
         ASLX                ;addresses occupy two bytes
         BR      guessJT,x   
guessJT: .ADDRSS case0       
         .ADDRSS case1       
         .ADDRSS case2       
         .ADDRSS case3       
case0:   STRO    msg0,d      ;cout << "Not close"
         BR      endCase     ;break
case1:   STRO    msg1,d      ;cout << "Close"
         BR      endCase     ;break
case2:   STRO    msg2,d      ;cout << "Right on"
         BR      endCase     ;break
case3:   STRO    msg3,d      ;cout << "Too high"
endCase: CHARO   '\n',i      ;count << endl
         ADDSP   2,i         ;deallocate #guess
         STOP                
msgIn:   .ASCII  "Pick a number 0..3: \x00"
msg0:    .ASCII  "Not close\x00"
msg1:    .ASCII  "Close\x00" 
msg2:    .ASCII  "Right on\x00"
msg3:    .ASCII  "Too high\x00"
         .END                  
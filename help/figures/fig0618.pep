;File: fig0618.pep
;Computer Systems, Fourth edition
;Figure 6.18
;
         BR      main        
;
;******* void printTri ()
printTri:STRO    msg1,d      ;cout << "*"
         CHARO   '\n',i      ;   << endl
         STRO    msg2,d      ;cout << "**"
         CHARO   '\n',i      ;   << endl
         STRO    msg3,d      ;cout << "***"
         CHARO   '\n',i      ;   << endl
         RET0                
msg1:    .ASCII  "*\x00"     
msg2:    .ASCII  "**\x00"    
msg3:    .ASCII  "***\x00"   
;
;******* int main ()
main:    CALL    printTri    ;printTri ()
         CALL    printTri    ;printTri ()
         CALL    printTri    ;printTri ()
         STOP                
         .END                  
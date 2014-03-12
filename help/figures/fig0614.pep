;File: fig0614.pep
;Computer Systems, Fourth edition
;Figure 6.14
;
         BR      main        
j:       .EQUATE 0           ;local variable #2d
;
main:    SUBSP   2,i         ;allocate #j
         LDA     0,i         ;for (j = 0
         STA     j,s         
for:     CPA     3,i         ;   j < 3
         BRGE    endFor      
         STRO    msg,d       ;   cout << "j = "
         DECO    j,s         ;      << j
         CHARO   '\n',i      ;      << endl
         LDA     j,s         ;   j++)
         ADDA    1,i         
         STA     j,s         
         BR      for         
endFor:  STRO    msg,d       ;cout << "j = "
         DECO    j,s         ;   << j
         CHARO   '\n',i      ;   << endl
         ADDSP   2,i         ;deallocate #j
         STOP                
msg:     .ASCII  "j = \x00"  
         .END                  
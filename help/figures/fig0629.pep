;File: fig0629.pep
;Computer Systems, Fourth edition
;Figure 6.29
;
         BR      main        
;
;******* void rect (int& p, int w, int h)
p:       .EQUATE 6           ;formal parameter #2h
w:       .EQUATE 4           ;formal parameter #2d
h:       .EQUATE 2           ;formal parameter #2d
rect:    LDA     w,s         ;p = (w + h) * 2
         ADDA    h,s         
         ASLA                
         STA     p,sf        
endIf:   RET0                ;pop retAddr
;
;******* main ()
perim:   .EQUATE 4           ;local variable #2d
width:   .EQUATE 2           ;local variable #2d
height:  .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;allocate #perim #width #height
         STRO    msg1,d      ;cout << "Enter width: "
         DECI    width,s     ;cin >> width
         STRO    msg2,d      ;cout << "Enter height: "
         DECI    height,s    ;cin >> height
         MOVSPA              ;push the address of perim
         ADDA    perim,i     
         STA     -2,s        
         LDA     width,s     ;push the value of width
         STA     -4,s        
         LDA     height,s    ;push the value of height
         STA     -6,s        
         SUBSP   6,i         ;push #p #w #h
         CALL    rect        ;rect (perim, width, height)
ra1:     ADDSP   6,i         ;pop #h #w #p
         STRO    msg3,d      ;cout << "perim = "
         DECO    perim,s     ;     << perim
         CHARO   '\n',i      ;     << endl
         ADDSP   6,i         ;deallocate #height #width #perim
         STOP                
msg1:    .ASCII  "Enter width: \x00"
msg2:    .ASCII  "Enter height: \x00"
msg3:    .ASCII  "perim = \x00"
         .END                  
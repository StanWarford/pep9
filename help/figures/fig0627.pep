;File: fig0627.pep
;Computer Systems, Fourth edition
;Figure 6.27
;
         BR      main
a:       .BLOCK  2          ;global variable #2d
b:       .BLOCK  2          ;global variable #2d
;
;******* void swap (int& r, int& s)
r:       .EQUATE 6          ;formal parameter #2h
s:       .EQUATE 4          ;formal parameter #2h
temp:    .EQUATE 0          ;local variable #2d
swap:    SUBSP   2,i        ;allocate #temp
         LDA     r,sf       ;temp = r
         STA     temp,s
         LDA     s,sf       ;r = s
         STA     r,sf
         LDA     temp,s     ;s = temp
         STA     s,sf
         RET2               ;deallocate #temp, pop retAddr
;
;******* void order (int& x, int& y)
x:       .EQUATE 4          ;formal parameter #2h
y:       .EQUATE 2          ;formal parameter #2h
order:   LDA     x,sf       ;if (x > y)
         CPA     y,sf
         BRLE    endIf
         LDA     x,s        ;   push x
         STA     -2,s
         LDA     y,s        ;   push y
         STA     -4,s
         SUBSP   4,i        ;   push #r #s
         CALL    swap       ;   swap (x, y)
         ADDSP   4,i        ;   pop #s #r
endIf:   RET0               ;pop retAddr
         
;
;******* main ()
main:    STRO    msg1,d     ;cout << "Enter an integer: "
         DECI    a,d        ;cin >> a
         STRO    msg1,d     ;cout << "Enter an integer: "
         DECI    b,d        ;cin >> b
         LDA     a,i        ;push the address of a
         STA     -2,s
         LDA     b,i        ;push the address of b
         STA     -4,s
         SUBSP   4,i        ;push #x #y
         CALL    order      ;order (a, b)
ra1:     ADDSP   4,i        ;pop #y #x
         STRO    msg2,d     ;cout << "Ordered they are: "
         DECO    a,d        ;     << a
         STRO    msg3,d     ;     << ", "
         DECO    b,d        ;     << b
         CHARO   '\n',i     ;     << endl
         STOP
msg1:    .ASCII  "Enter an integer: \x00"
msg2:    .ASCII  "Ordered they are: \x00"
msg3:    .ASCII  ", \x00"
         .END   

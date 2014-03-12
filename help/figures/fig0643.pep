;File: fig0643.pep
;Computer Systems, Fourth edition
;Figure 6.43
;
         BR      main        
;
;******* main ()
a:       .EQUATE 4           ;local variable #2h
b:       .EQUATE 2           ;local variable #2h
c:       .EQUATE 0           ;local variable #2h
main:    SUBSP   6,i         ;allocate #a #b #c
         LDA     2,i         ;a = new int
         CALL    new         ;#a
         STX     a,s         
         LDA     5,i         ;*a = 5
         STA     a,sf        
         LDA     2,i         ;b = new int
         CALL    new         ;#b
         STX     b,s         
         LDA     3,i         ;*b = 3
         STA     b,sf        
         LDA     a,s         ;c = a
         STA     c,s         
         LDA     b,s         ;a = b
         STA     a,s         
         LDA     2,i         ;*a = 2 + *c
         ADDA    c,sf        
         STA     a,sf        
         STRO    msg0,d      ;cout << "*a = "
         DECO    a,sf        ;   << *a
         CHARO   '\n',i      ;   << endl
         STRO    msg1,d      ;cout << "*b = "
         DECO    b,sf        ;   << *b
         CHARO   '\n',i      ;   << endl
         STRO    msg2,d      ;cout << "*c = "
         DECO    c,sf        ;   << *c
         CHARO   '\n',i      ;   << endl
         ADDSP   6,i         ;deallocate #c #b #a
         STOP                
msg0:    .ASCII  "*a = \x00" 
msg1:    .ASCII  "*b = \x00" 
msg2:    .ASCII  "*c = \x00" 
;
;******* operator new
;        Precondition: A contains number of bytes
;        Postcondition: X contains pointer to bytes
new:     LDX     hpPtr,d     ;returned pointer
         ADDA    hpPtr,d     ;allocate from heap
         STA     hpPtr,d     ;update hpPtr
         RET0                
hpPtr:   .ADDRSS heap        ;address of next free byte
heap:    .BLOCK  1           ;first byte in the heap
         .END                  
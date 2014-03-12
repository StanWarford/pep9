;File: fig0641.pep
;Computer Systems, Fourth edition
;Figure 6.41
;
         BR      main        
a:       .BLOCK  2           ;global variable #2h
b:       .BLOCK  2           ;global variable #2h
c:       .BLOCK  2           ;global variable #2h
;
;******* main ()
main:    LDA     2,i         ;a = new int
         CALL    new         ;#a
         STX     a,d         
         LDA     5,i         ;*a = 5
         STA     a,n         
         LDA     2,i         ;b = new int
         CALL    new         ;#b
         STX     b,d         
         LDA     3,i         ;*b = 3
         STA     b,n         
         LDA     a,d         ;c = a
         STA     c,d         
         LDA     b,d         ;a = b
         STA     a,d         
         LDA     2,i         ;*a = 2 + *c
         ADDA    c,n         
         STA     a,n         
         STRO    msg0,d      ;cout << "*a = "
         DECO    a,n         ;   << *a
         CHARO   '\n',i      ;   << endl
         STRO    msg1,d      ;cout << "*b = "
         DECO    b,n         ;   << *b
         CHARO   '\n',i      ;   << endl
         STRO    msg2,d      ;cout << "*c = "
         DECO    c,n         ;   << *c
         CHARO   '\n',i      ;   << endl
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

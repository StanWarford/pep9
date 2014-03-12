;File: fig0647.pep
;Computer Systems, Fourth edition
;Figure 6.47
;
         BR      main        
data:    .EQUATE 0           ;struct field #2d
next:    .EQUATE 2           ;struct field #2h
;
;******* main ()
first:   .EQUATE 4           ;local variable #2h
p:       .EQUATE 2           ;local variable #2h
value:   .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;allocate #first #p #value
         LDA     0,i         ;first = 0
         STA     first,s     
         DECI    value,s     ;cin >> value
while:   LDA     value,s     ;while (value != -9999)
         CPA     -9999,i     
         BREQ    endWh       
         LDA     first,s     ;   p = first
         STA     p,s         
         LDA     4,i         ;   first = new node
         CALL    new         ;   allocate #data #next
         STX     first,s     
         LDA     value,s     ;   first->data = value
         LDX     data,i      
         STA     first,sxf   
         LDA     p,s         ;   first->next = p
         LDX     next,i      
         STA     first,sxf   
         DECI    value,s     ;   cin >> value
         BR      while       
endWh:   LDA     first,s     ;for (p = first
         STA     p,s         
for:     LDA     p,s         ;   p != 0
         CPA     0,i         
         BREQ    endFor      
         LDX     data,i      ;   cout << p->data
         DECO    p,sxf       
         CHARO   ' ',i       ;      << ' '
         LDX     next,i      ;   p = p->next)
         LDA     p,sxf       
         STA     p,s         
         BR      for         
endFor:  ADDSP   6,i         ;deallocate #value #p #first
         STOP                
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
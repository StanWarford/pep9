;File: fig0648.pep
;Computer Systems, Fifth edition
;Figure 6.48
;
         BR      main        
data:    .EQUATE 0           ;struct field #2d
next:    .EQUATE 2           ;struct field #2h
;
;******* main ()
first:   .EQUATE 4           ;local variable #2h
p:       .EQUATE 2           ;local variable #2h
value:   .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;push #first #p #value
         LDWA    0,i         ;first = 0
         STWA    first,s     
         DECI    value,s     ;scanf("%d", &value);
while:   LDWA    value,s     ;while (value != -9999)
         CPWA    -9999,i     
         BREQ    endWh       
         LDWA    first,s     ;p = first
         STWA    p,s         
         LDWA    4,i         ;first = (struct node *) malloc(sizeof(struct node))
         CALL    malloc      ;allocate #data #next
         STWX    first,s     
         LDWA    value,s     ;first->data = value
         LDWX    data,i      
         STWA    first,sfx   
         LDWA    p,s         ;first->next = p
         LDWX    next,i      
         STWA    first,sfx   
         DECI    value,s     ;scanf("%d", &value)
         BR      while       
endWh:   LDWA    first,s     ;for (p = first
         STWA    p,s         
for:     LDWA    p,s         ;p != 0
         CPWA    0,i         
         BREQ    endFor      
         LDWX    data,i      ;printf("%d ", p->data)
         DECO    p,sfx       
         LDBA    ' ',i       
         STBA    charOut,d   
         LDWX    next,i      ;p = p->next)
         LDWA    p,sfx       
         STWA    p,s         
         BR      for         
endFor:  ADDSP   6,i         ;pop #value #p #first
         STOP                
;
;******* malloc()
;        Precondition: A contains number of bytes
;        Postcondition: X contains pointer to bytes
malloc:  LDWX    hpPtr,d     ;returned pointer
         ADDA    hpPtr,d     ;allocate from heap
         STWA    hpPtr,d     ;update hpPtr
         RET                 
hpPtr:   .ADDRSS heap        ;address of next free byte
heap:    .BLOCK  1           ;first byte in the heap
         .END                  

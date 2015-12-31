;File: fig0644.pep
;Computer Systems, Fifth edition
;Figure 6.44
;
         BR      main        
;
;******* main()
a:       .EQUATE 4           ;local variable #2h
b:       .EQUATE 2           ;local variable #2h
c:       .EQUATE 0           ;local variable #2h
main:    SUBSP   6,i         ;push #a #b #c
         LDWA    2,i         ;a = (int *) malloc(sizeof(int))
         CALL    malloc      ;allocate #2d
         STWX    a,s         
         LDWA    5,i         ;*a = 5
         STWA    a,sf        
         LDWA    2,i         ;b = (int *) malloc(sizeof(int))
         CALL    malloc      ;allocate #2d
         STWX    b,s         
         LDWA    3,i         ;*b = 3
         STWA    b,sf        
         LDWA    a,s         ;c = a
         STWA    c,s         
         LDWA    b,s         ;a = b
         STWA    a,s         
         LDWA    2,i         ;*a = 2 + *c
         ADDA    c,sf        
         STWA    a,sf        
         STRO    msg0,d      ;printf("*a = %d\n", *a)
         DECO    a,sf        
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg1,d      ;printf("*b = %d\n", *b)
         DECO    b,sf        
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msg2,d      ;printf("*c = %d\n", *c)
         DECO    c,sf        
         LDBA    '\n',i      
         STBA    charOut,d   
         ADDSP   6,i         ;pop #c #b #a
         STOP                
msg0:    .ASCII  "*a = \x00" 
msg1:    .ASCII  "*b = \x00" 
msg2:    .ASCII  "*c = \x00" 
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

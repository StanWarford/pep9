;File: fig0642.pep
;Computer Systems, Fifth edition
;Figure 6.42
;
         BR      main
a:       .BLOCK  2           ;global variable #2h
b:       .BLOCK  2           ;global variable #2h
c:       .BLOCK  2           ;global variable #2h
;
;******* main ()
main:    LDWA    2,i         ;a = (int *) malloc(sizeof(int))
         CALL    malloc      ;allocate #2d
         STWX    a,d
         LDWA    5,i         ;*a = 5
         STWA    a,n
         LDWA    2,i         ;b = (int *) malloc(sizeof(int))
         CALL    malloc      ;allocate #2d
         STWX    b,d
         LDWA    3,i         ;*b = 3
         STWA    b,n
         LDWA    a,d         ;c = a
         STWA    c,d
         LDWA    b,d         ;a = b
         STWA    a,d
         LDWA    2,i         ;*a = 2 + *c
         ADDA    c,n
         STWA    a,n
         STRO    msg0,d      ;printf("*a = %d\n", *a)
         DECO    a,n
         LDBA    '\n',i
         STBA    charOut,d
         STRO    msg1,d      ;printf("*b = %d\n", *b)
         DECO    b,n
         LDBA    '\n',i
         STBA    charOut,d
         STRO    msg2,d      ;printf("*c = %d\n", *c)
         DECO    c,n
         LDBA    '\n',i
         STBA    charOut,d
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

;File: fig0629.pep
;Computer Systems, Fifth edition
;Figure 6.29
;
         BR      main        
;
;******* void rect(int *p, int w, int h)
p:       .EQUATE 6           ;formal parameter #2h
w:       .EQUATE 4           ;formal parameter #2d
h:       .EQUATE 2           ;formal parameter #2d
rect:    LDWA    w,s         ;*p = (w + h) * 2
         ADDA    h,s         
         ASLA                
         STWA    p,sf        
         RET                 
;
;******* main()
perim:   .EQUATE 4           ;local variable #2d
width:   .EQUATE 2           ;local variable #2d
height:  .EQUATE 0           ;local variable #2d
main:    SUBSP   6,i         ;push #perim #width #height
         STRO    msg1,d      ;printf("Enter width: ")
         DECI    width,s     ;scanf("%d", &width)
         STRO    msg2,d      ;printf("Enter height: ")
         DECI    height,s    ;scanf("%d", &height)
         MOVSPA              ;move &perim
         ADDA    perim,i     
         STWA    -2,s        
         LDWA    width,s     ;move width
         STWA    -4,s        
         LDWA    height,s    ;move height
         STWA    -6,s        
         SUBSP   6,i         ;push #p #w #h
         CALL    rect        ;rect(&perim, width, height)
ra1:     ADDSP   6,i         ;pop #h #w #p
         STRO    msg3,d      ;printf("Perimeter = %d\n", perim);
         DECO    perim,s     
         LDBA    '\n',i      
         STBA    charOut,d   
         ADDSP   6,i         ;pop #height #width #perim
         STOP                
msg1:    .ASCII  "Enter width: \x00"
msg2:    .ASCII  "Enter height: \x00"
msg3:    .ASCII  "Perimeter = \x00"
         .END                  
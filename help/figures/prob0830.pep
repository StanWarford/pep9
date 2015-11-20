;File: prob0830.pep
;Computer Systems, Fifth edition
;Problem 8.30
;
         BR      main        
dBool:   .BLOCK  2           ;global variable #2d
;
;******* void bTest()
sBool:   .EQUATE 0           ;local variable #2d
bTest:   SUBSP   2,i         ;push #sBool
         STRO    msgFs,d     ;Test direct addressing
         LDWA    0,i         ;sBool = 0
         STWA    dBool,s     
         BOOLO   dBool,s     
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgTs,d     
         LDWA    2,i         ;sBool = 2
         STWA    dBool,s     
         BOOLO   dBool,s     
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgTs,d     
         LDWA    -2,i        ;sBool = -2
         STWA    dBool,s     
         BOOLO   dBool,s     
         LDBA    '\n',i      
         STBA    charOut,d   
         ADDSP   2,i         ;pop #sBool
         RET                 
;
main:    STRO    msgFi,d     ;Test immediate addressing
         BOOLO   0,i         ;Test with 0
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgTi,d     
         BOOLO   2,i         ;Test with 2
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgTi,d     
         BOOLO   -2,i        ;Test with -2
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgFd,d     ;Test direct addressing
         LDWA    0,i         ;dBool = 0
         STWA    dBool,d     
         BOOLO   dBool,d     
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgTd,d     
         LDWA    2,i         ;dBool = 2
         STWA    dBool,d     
         BOOLO   dBool,d     
         LDBA    '\n',i      
         STBA    charOut,d   
         STRO    msgTd,d     
         LDWA    -2,i        ;dBool = -2
         STWA    dBool,d     
         BOOLO   dBool,d     
         LDBA    '\n',i      
         STBA    charOut,d   
         CALL    bTest       
         STOP                
msgFi:   .ASCII  "Immediate, should be false: \x00"
msgTi:   .ASCII  "Immediate, should be true: \x00"
msgFd:   .ASCII  "Direct, should be false: \x00"
msgTd:   .ASCII  "Direct, should be true: \x00"
msgFs:   .ASCII  "Stack-relative, should be false: \x00"
msgTs:   .ASCII  "Stack-relative, should be true: \x00"
         .END                  
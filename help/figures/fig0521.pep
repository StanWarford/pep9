;File: fig0521.pep
;Computer Systems, Fourth edition
;Figure 5.21
;
         BR      main
ch:      .BLOCK  1           ;global variable #1c
j:       .BLOCK  2           ;global variable #2d
;
main:    LDBA    charIn,d    ;cin >> ch
         STBA    ch,d
         DECI    j,d         ;   >> j
         LDWA    j,d         ;j += 5
         ADDA    5,i
         STWA    j,d
         LDBA    ch,d        ;ch++
         ADDA    1,i
         STBA    ch,d
         LDBA    ch,d        ;cout << ch
         STBA    charOut,d
         LDBA    '\n',i      ;   << endl
         STBA    charOut,d
         DECO    j,d         ;   << j
         LDBA    '\n',i      ;   << endl
         STBA    charOut,d
         STOP
         .END

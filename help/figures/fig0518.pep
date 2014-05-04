;File: fig0518.pep
;Computer Systems, Fourth Edition
;Figure 5.18
;
         STRO    msg,d
         LDBA    '\n',i
         STBA    charOut,d
         STOP
msg:     .ASCII  "Love\x00"
         .END

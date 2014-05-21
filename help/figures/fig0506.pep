;File: fig0506.pep
;Computer Systems, Fifth Edition
;Figure 5.6
;
         LDBA    0xFC15,d    ;Input first character
         STBA    0x0013,d    ;Store first character
         LDBA    0xFC15,d    ;Input second character
         STBA    0xFC16,d    ;Output second character
         LDBA    0x0013,d    ;Load first character
         STBA    0xFC16,d    ;Output first character
         STOP
         .BLOCK  1           ;Storage for first character
         .END

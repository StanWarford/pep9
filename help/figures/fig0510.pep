;File: fig0510.pep
;Computer Systems, Fourth Edition
;Figure 5.10
;
         LDBA    'H',i       ;Output 'H'
         STBA    0xFC49,d
         LDBA    'i',i       ;Output 'i'
         STBA    0xFC49,d
         STOP
         .END

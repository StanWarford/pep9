;Stan Warford
;May 1, 2016
;A program to output "Hi"
;
         LDBA    0x000D,d    ;Output 'H'
         STBA    0xFC4F,d
         LDBA    0x000E,d    ;Output 'i'
         STBA    0xFC4F,d
         STOP
         .ASCII  "Hi"
         .END

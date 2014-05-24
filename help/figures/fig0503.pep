;Stan Warford
;May 1, 2016
;A program to output "Hi"
;
         LDBA    0x000D,d    ;Load byte accumulator 'H'
         STBA    0xFC16,d    ;Store byte accumulator output port
         LDBA    0x000E,d    ;Load byte accumulator 'i'
         STBA    0xFC16,d    ;Store byte accumulator output port
         STOP                ;Stop
         .ASCII  "Hi"        ;ASCII "Hi" characters
         .END

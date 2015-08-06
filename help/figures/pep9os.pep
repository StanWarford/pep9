;******* Pep/9 Operating System, 2015/05/17
;
TRUE:    .EQUATE 1
FALSE:   .EQUATE 0
;
;******* Operating system RAM
osRAM:   .BLOCK  128         ;System stack area
wordTemp:.BLOCK  1           ;Temporary word storage
byteTemp:.BLOCK  1           ;Least significant byte of wordTemp
addrMask:.BLOCK  2           ;Addressing mode mask
opAddr:  .BLOCK  2           ;Trap instruction operand address
charIn:  .BLOCK  1           ;Memory-mapped input device
charOut: .BLOCK  1           ;Memory-mapped output device
;
;******* Operating system ROM
         .BURN   0xFFFF
;
;******* System Loader
;Data must be in the following format:
;Each hex number representing a byte must contain exactly two
;characters. Each character must be in 0..9, A..F, or a..f and
;must be followed by exactly one space. There must be no
;leading spaces at the beginning of a line and no trailing
;spaces at the end of a line. The last two characters in the
;file must be lowercase zz, which is used as the terminating
;sentinel by the loader.
;
loader:  LDWX    0,i         ;X <- 0
;
getChar: LDBA    charIn,d    ;Get first hex character
         CPBA    'z',i       ;If end of file sentinel 'z'
         BREQ    stopLoad    ;  then exit loader routine
         CPBA    '9',i       ;If characer <= '9', assume decimal
         BRLE    shift       ;  and right nybble is correct digit
         ADDA    9,i         ;else convert nybble to correct digit
shift:   ASLA                ;Shift left by four bits to send
         ASLA                ;  the digit to the most significant
         ASLA                ;  position in the byte
         ASLA
         STBA    byteTemp,d  ;Save the most significant nybble
         LDBA    charIn,d    ;Get second hex character
         CPBA    '9',i       ;If characer <= '9', assume decimal
         BRLE    combine     ;  and right nybble is correct digit
         ADDA    9,i         ;else convert nybble to correct digit
combine: ANDA    0x000F,i    ;Mask out the left nybble
         ORA     wordTemp,d  ;Combine both hex digits in binary
         STBA    0,x         ;Store in Mem[X]
         ADDX    1,i         ;X <- X + 1
         LDBA    charIn,d    ;Skip blank or <LF>
         BR      getChar     ;
;
stopLoad:STOP                ;
;
;******* Trap handler
oldIR:   .EQUATE 9           ;Stack address of IR on trap
;
trap:    LDBX    oldIR,s     ;X <- trapped IR
         CPBX    0x0028,i    ;If X >= first nonunary trap opcode
         BRGE    nonUnary    ;  trap opcode is nonunary
;
unary:   ANDX    0x0001,i    ;Mask out all but rightmost bit
         ASLX                ;Two bytes per address
         CALL    unaryJT,x   ;Call unary trap routine
         RETTR               ;Return from trap
;
unaryJT: .ADDRSS opcode26    ;Address of NOP0 subroutine
         .ADDRSS opcode27    ;Address of NOP1 subroutine
;
nonUnary:ASRX                ;Trap opcode is nonunary
         ASRX                ;Discard addressing mode bits
         ASRX
         SUBX    5,i         ;Adjust so that NOP opcode = 0
         ASLX                ;Two bytes per address
         CALL    nonUnJT,x   ;Call nonunary trap routine
return:  RETTR               ;Return from trap
;
nonUnJT: .ADDRSS opcode28    ;Address of NOP subroutine
         .ADDRSS opcode30    ;Address of DECI subroutine
         .ADDRSS opcode38    ;Address of DECO subroutine
         .ADDRSS opcode40    ;Address of HEXO subroutine
         .ADDRSS opcode48    ;Address of STRO subroutine
;
;******* Assert valid trap addressing mode
oldIR4:  .EQUATE 13          ;oldIR + 4 with two return addresses
assertAd:LDBA    1,i         ;A <- 1
         LDBX    oldIR4,s    ;X <- OldIR
         ANDX    0x0007,i    ;Keep only the addressing mode bits
         BREQ    testAd      ;000 = immediate addressing
loop:    ASLA                ;Shift the 1 bit left
         SUBX    1,i         ;Subtract from addressing mode count
         BRNE    loop        ;Try next addressing mode
testAd:  ANDA    addrMask,d  ;AND the 1 bit with legal modes
         BREQ    addrErr
         RET                 ;Legal addressing mode, return
addrErr: LDBA    '\n',i
         STBA    charOut,d
         LDWA    trapMsg,i   ;Push address of error message
         STWA    -2,s
         SUBSP   2,i         ;Call print subroutine
         CALL    prntMsg
         STOP                ;Halt: Fatal runtime error
trapMsg: .ASCII  "ERROR: Invalid trap addressing mode.\x00"
;
;******* Set address of trap operand
oldX4:   .EQUATE 7           ;oldX + 4 with two return addresses
oldPC4:  .EQUATE 9           ;oldPC + 4 with two return addresses
oldSP4:  .EQUATE 11          ;oldSP + 4 with two return addresses
setAddr: LDBX    oldIR4,s    ;X <- old instruction register
         ANDX    0x0007,i    ;Keep only the addressing mode bits
         ASLX                ;Two bytes per address
         BR      addrJT,x
addrJT:  .ADDRSS addrI       ;Immediate addressing
         .ADDRSS addrD       ;Direct addressing
         .ADDRSS addrN       ;Indirect addressing
         .ADDRSS addrS       ;Stack-relative addressing
         .ADDRSS addrSF      ;Stack-relative deferred addressing
         .ADDRSS addrX       ;Indexed addressing
         .ADDRSS addrSX      ;Stack-indexed addressing
         .ADDRSS addrSFX     ;Stack-deferred indexed addressing
;
addrI:   LDWX    oldPC4,s    ;Immediate addressing
         SUBX    2,i         ;Oprnd = OprndSpec
         STWX    opAddr,d
         RET
;
addrD:   LDWX    oldPC4,s    ;Direct addressing
         SUBX    2,i         ;Oprnd = Mem[OprndSpec]
         LDWX    0,x
         STWX    opAddr,d
         RET
;
addrN:   LDWX    oldPC4,s    ;Indirect addressing
         SUBX    2,i         ;Oprnd = Mem[Mem[OprndSpec]]
         LDWX    0,x
         LDWX    0,x
         STWX    opAddr,d
         RET
;
addrS:   LDWX    oldPC4,s    ;Stack-relative addressing
         SUBX    2,i         ;Oprnd = Mem[SP + OprndSpec]
         LDWX    0,x
         ADDX    oldSP4,s
         STWX    opAddr,d
         RET
;
addrSF:  LDWX    oldPC4,s    ;Stack-relative deferred addressing
         SUBX    2,i         ;Oprnd = Mem[Mem[SP + OprndSpec]]
         LDWX    0,x
         ADDX    oldSP4,s
         LDWX    0,x
         STWX    opAddr,d
         RET
;
addrX:   LDWX    oldPC4,s    ;Indexed addressing
         SUBX    2,i         ;Oprnd = Mem[OprndSpec + X]
         LDWX    0,x
         ADDX    oldX4,s
         STWX    opAddr,d
         RET
;
addrSX:  LDWX    oldPC4,s    ;Stack-indexed addressing
         SUBX    2,i         ;Oprnd = Mem[SP + OprndSpec + X]
         LDWX    0,x
         ADDX    oldX4,s
         ADDX    oldSP4,s
         STWX    opAddr,d
         RET
;
addrSFX: LDWX    oldPC4,s    ;Stack-deferred indexed addressing
         SUBX    2,i         ;Oprnd = Mem[Mem[SP + OprndSpec] + X]
         LDWX    0,x
         ADDX    oldSP4,s
         LDWX    0,x
         ADDX    oldX4,s
         STWX    opAddr,d
         RET
;
;******* Opcode 0x26
;The NOP0 instruction.
opcode26:RET
;
;******* Opcode 0x27
;The NOP1 instruction.
opcode27:RET
;
;******* Opcode 0x28
;The NOP instruction.
opcode28:LDWA    0x0001,i    ;Assert i
         STWA    addrMask,d
         CALL    assertAd
         RET
;
;******* Opcode 0x30
;The DECI instruction.
;Input format: Any number of leading spaces or line feeds are
;allowed, followed by '+', '-' or a digit as the first character,
;after which digits are input until the first nondigit is
;encountered. The status flags N,Z and V are set appropriately
;by this DECI routine. The C status flag is not affected.
;
oldNZVC: .EQUATE 15          ;Stack address of NZVC on interrupt
;
total:   .EQUATE 11          ;Cumulative total of DECI number
asciiCh: .EQUATE 10          ;asciiCh, one byte
valAscii:.EQUATE 8           ;Value(asciiCh)
isOvfl:  .EQUATE 6           ;Overflow boolean
isNeg:   .EQUATE 4           ;Negative boolean
state:   .EQUATE 2           ;State variable
temp:    .EQUATE 0
;
init:    .EQUATE 0           ;Enumerated values for state
sign:    .EQUATE 1
digit:   .EQUATE 2
;
opcode30:LDWA    0x00FE,i    ;Assert d, n, s, sf, x, sx, sfx
         STWA    addrMask,d
         CALL    assertAd
         CALL    setAddr     ;Set address of trap operand
         SUBSP   13,i        ;Allocate storage for locals
         LDWA    FALSE,i     ;isOvfl <- FALSE
         STWA    isOvfl,s
         LDWA    init,i      ;state <- init
         STWA    state,s
;
do:      LDBA    charIn,d    ;Get asciiCh
         STBA    asciiCh,s
         ANDA    0x000F,i    ;Set value(asciiCh)
         STWA    valAscii,s
         LDBA    asciiCh,s   ;A<low> = asciiCh throughout the loop
         LDWX    state,s     ;switch (state)
         ASLX                ;Two bytes per address
         BR      stateJT,x
;
stateJT: .ADDRSS sInit
         .ADDRSS sSign
         .ADDRSS sDigit
;
sInit:   CPBA    '+',i       ;if (asciiCh == '+')
         BRNE    ifMinus
         LDWX    FALSE,i     ;isNeg <- FALSE
         STWX    isNeg,s
         LDWX    sign,i      ;state <- sign
         STWX    state,s
         BR      do
;
ifMinus: CPBA    '-',i       ;else if (asciiCh == '-')
         BRNE    ifDigit
         LDWX    TRUE,i      ;isNeg <- TRUE
         STWX    isNeg,s
         LDWX    sign,i      ;state <- sign
         STWX    state,s
         BR      do
;
ifDigit: CPBA    '0',i       ;else if (asciiCh is a digit)
         BRLT    ifWhite
         CPBA    '9',i
         BRGT    ifWhite
         LDWX    FALSE,i     ;isNeg <- FALSE
         STWX    isNeg,s
         LDWX    valAscii,s  ;total <- Value(asciiCh)
         STWX    total,s
         LDWX    digit,i     ;state <- digit
         STWX    state,s
         BR      do
;
ifWhite: CPBA    ' ',i       ;else if (asciiCh is not a space
         BREQ    do
         CPBA    '\n',i      ;or line feed)
         BRNE    deciErr     ;exit with DECI error
         BR      do
;
sSign:   CPBA    '0',i       ;if asciiCh (is not a digit)
         BRLT    deciErr
         CPBA    '9',i
         BRGT    deciErr     ;exit with DECI error
         LDWX    valAscii,s  ;else total <- Value(asciiCh)
         STWX    total,s
         LDWX    digit,i     ;state <- digit
         STWX    state,s
         BR      do
;
sDigit:  CPBA    '0',i       ;if (asciiCh is not a digit)
         BRLT    deciNorm
         CPBA    '9',i
         BRGT    deciNorm    ;exit normaly
         LDWX    TRUE,i      ;else X <- TRUE for later assignments
         LDWA    total,s     ;Multiply total by 10 as follows:
         ASLA                ;First, times 2
         BRV     ovfl1       ;If overflow then
         BR      L1
ovfl1:   STWX    isOvfl,s    ;isOvfl <- TRUE
L1:      STWA    temp,s      ;Save 2 * total in temp
         ASLA                ;Now, 4 * total
         BRV     ovfl2       ;If overflow then
         BR      L2
ovfl2:   STWX    isOvfl,s    ;isOvfl <- TRUE
L2:      ASLA                ;Now, 8 * total
         BRV     ovfl3       ;If overflow then
         BR      L3
ovfl3:   STWX    isOvfl,s    ;isOvfl <- TRUE
L3:      ADDA    temp,s      ;Finally, 8 * total + 2 * total
         BRV     ovfl4       ;If overflow then
         BR      L4
ovfl4:   STWX    isOvfl,s    ;isOvfl <- TRUE
L4:      ADDA    valAscii,s  ;A <- 10 * total + valAscii
         BRV     ovfl5       ;If overflow then
         BR      L5
ovfl5:   STWX    isOvfl,s    ;isOvfl <- TRUE
L5:      STWA    total,s     ;Update total
         BR      do
;
deciNorm:LDWA    isNeg,s     ;If isNeg then
         BREQ    setNZ
         LDWA    total,s     ;If total != 0x8000 then
         CPWA    0x8000,i
         BREQ    L6
         NEGA                ;Negate total
         STWA    total,s
         BR      setNZ
L6:      LDWA    FALSE,i     ;else -32768 is a special case
         STWA    isOvfl,s    ;isOvfl <- FALSE
;
setNZ:   LDBX    oldNZVC,s   ;Set NZ according to total result:
         ANDX    0x0001,i    ;First initialize NZV to 000
         LDWA    total,s     ;If total is negative then
         BRGE    checkZ
         ORX     0x0008,i    ;set N to 1
checkZ:  CPWA    0,i         ;If total is not zero then
         BRNE    setV
         ORX     0x0004,i    ;set Z to 1
setV:    LDWA    isOvfl,s    ;If not isOvfl then
         BREQ    storeFl
         ORX     0x0002,i    ;set V to 1
storeFl: STBX    oldNZVC,s   ;Store the NZVC flags
;
exitDeci:LDWA    total,s     ;Put total in memory
         STWA    opAddr,n
         ADDSP   13,i        ;Deallocate locals
         RET                 ;Return to trap handler
;
deciErr: LDBA    '\n',i
         STBA    charOut,d
         LDWA    deciMsg,i   ;Push address of message onto stack
         STWA    -2,s
         SUBSP   2,i
         CALL    prntMsg     ;and print
         STOP                ;Fatal error: program terminates
;
deciMsg: .ASCII  "ERROR: Invalid DECI input\x00"
;
;******* Opcode 0x38
;The DECO instruction.
;Output format: If the operand is negative, the algorithm prints
;a single '-' followed by the magnitude. Otherwise it prints the
;magnitude without a leading '+'. It suppresses leading zeros.
;
remain:  .EQUATE 0           ;Remainder of value to output
outYet:  .EQUATE 2           ;Has a character been output yet?
place:   .EQUATE 4           ;Place value for division
;
opcode38:LDWA    0x00FF,i    ;Assert i, d, n, s, sf, x, sx, sfx
         STWA    addrMask,d
         CALL    assertAd
         CALL    setAddr     ;Set address of trap operand
         SUBSP   6,i         ;Allocate storage for locals
         LDWA    opAddr,n    ;A <- oprnd
         CPWA    0,i         ;If oprnd is negative then
         BRGE    printMag
         LDBX    '-',i       ;Print leading '-'
         STBX    charOut,d
         NEGA                ;Make magnitude positive
printMag:STWA    remain,s    ;remain <- abs(oprnd)
         LDWA    FALSE,i     ;Initialize outYet <- FALSE
         STWA    outYet,s
         LDWA    10000,i     ;place <- 10,000
         STWA    place,s
         CALL    divide      ;Write 10,000's place
         LDWA    1000,i      ;place <- 1,000
         STWA    place,s
         CALL    divide      ;Write 1000's place
         LDWA    100,i       ;place <- 100
         STWA    place,s
         CALL    divide      ;Write 100's place
         LDWA    10,i        ;place <- 10
         STWA    place,s
         CALL    divide      ;Write 10's place
         LDWA    remain,s    ;Always write 1's place
         ORA     0x0030,i    ;Convert decimal to ASCII
         STBA    charOut,d   ;  and output it
         ADDSP   6,i         ;Dallocate storage for locals
         RET
;
;Subroutine to print the most significant decimal digit of the
;remainder. It assumes that place (place2 here) contains the
;decimal place value. It updates the remainder.
;
remain2: .EQUATE 2           ;Stack addresses while executing a
outYet2: .EQUATE 4           ;  subroutine are greater by two because
place2:  .EQUATE 6           ;  the retAddr is on the stack
;
divide:  LDWA    remain2,s   ;A <- remainder
         LDWX    0,i         ;X <- 0
divLoop: SUBA    place2,s    ;Division by repeated subtraction
         BRLT    writeNum    ;If remainder is negative then done
         ADDX    1,i         ;X <- X + 1
         STWA    remain2,s   ;Store the new remainder
         BR      divLoop
;
writeNum:CPWX    0,i         ;If X != 0 then
         BREQ    checkOut
         LDWA    TRUE,i      ;outYet <- TRUE
         STWA    outYet2,s
         BR      printDgt    ;and branch to print this digit
checkOut:LDWA    outYet2,s   ;else if a previous char was output
         BRNE    printDgt    ;then branch to print this zero
         RET                 ;else return to calling routine
;
printDgt:ORX     0x0030,i    ;Convert decimal to ASCII
         STBX    charOut,d   ;  and output it
         RET                 ;return to calling routine
;
;******* Opcode 0x40
;The HEXO instruction.
;Outputs one word as four hex characters from memory.
;
opcode40:LDWA    0x00FF,i    ;Assert i, d, n, s, sf, x, sx, sfx
         STWA    addrMask,d
         CALL    assertAd
         CALL    setAddr     ;Set address of trap operand
         LDWA    opAddr,n    ;A <- oprnd
         STWA    wordTemp,d  ;Save oprnd in wordTemp
         LDBA    wordTemp,d  ;Put high-order byte in low-order A
         ASRA                ;Shift right four bits
         ASRA
         ASRA
         ASRA
         CALL    hexOut      ;Output first hex character
         LDBA    wordTemp,d  ;Put high-order byte in low-order A
         CALL    hexOut      ;Output second hex character
         LDBA    byteTemp,d  ;Put low-order byte in low order A
         ASRA                ;Shift right four bits
         ASRA
         ASRA
         ASRA
         CALL    hexOut      ;Output third hex character
         LDBA    byteTemp,d  ;Put low-order byte in low order A
         CALL    hexOut      ;Output fourth hex character
         RET
;
;Subroutine to output in hex the least significant nybble of the
;accumulator.
;
hexOut:  ANDA    0x000F,i    ;Isolate the digit value
         CPBA    9,i         ;If it is not in 0..9 then
         BRLE    prepNum
         SUBA    9,i         ;  convert to ASCII letter
         ORA     0x0040,i    ;  and prefix ASCII code for letter
         BR      writeHex
prepNum: ORA     0x0030,i    ;else prefix ASCII code for number
writeHex:STBA    charOut,d   ;Output nybble as hex
         RET
;
;******* Opcode 0x48
;The STRO instruction.
;Outputs a null-terminated string from memory.
;
opcode48:LDWA    0x003E,i    ;Assert d, n, s, sf, x
         STWA    addrMask,d
         CALL    assertAd
         CALL    setAddr     ;Set address of trap operand
         LDWA    opAddr,d    ;Push address of string to print
         STWA    -2,s
         SUBSP   2,i
         CALL    prntMsg     ;and print
         ADDSP   2,i
         RET
;
;******* Print subroutine
;Prints a string of ASCII bytes until it encounters a null
;byte (eight zero bits). Assumes one parameter, which
;contains the address of the message.
;
msgAddr: .EQUATE 2           ;Address of message to print
;
prntMsg: LDWX    0,i         ;X <- 0
         LDWA    0,i         ;A <- 0
prntMore:LDBA    msgAddr,sfx ;Test next char
         BREQ    exitPrnt    ;If null then exit
         STBA    charOut,d   ;else print
         ADDX    1,i         ;X <- X + 1 for next character
         BR      prntMore
;
exitPrnt:RET
;
;******* Vectors for system memory map
         .ADDRSS osRAM       ;User stack pointer
         .ADDRSS wordTemp    ;System stack pointer
         .ADDRSS charIn      ;Memory-mapped input device
         .ADDRSS charOut     ;Memory-mapped output device
         .ADDRSS loader      ;Loader program counter
         .ADDRSS trap        ;Trap program counter
;
         .END

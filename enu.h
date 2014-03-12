// File: enu.h
/*
    Pep8-1 is a virtual machine for writing machine language and assembly
    language programs.
    
    Copyright (C) 2009  J. Stanley Warford, Pepperdine University

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ENU_H
#define ENU_H

namespace Enu
{
   // Instruction mnemonics
   enum EMnemonic
   {
        ADDA, ADDSP, ADDX, ANDA, ANDX, ASLA, ASLX, ASRA, ASRX,
        BR, BRC, BREQ, BRGE, BRGT, BRLE, BRLT, BRNE, BRV,
        CALL, CHARI, CHARO, CPA, CPX,
        DECI, DECO,
        LDA, LDBYTEA, LDBYTEX, LDX,
        MOVFLGA, MOVSPA,
        NEGA, NEGX, NOP, NOP0, NOP1, NOP2, NOP3, NOTA, NOTX,
        ORA, ORX,
        RET0, RET1, RET2, RET3, RET4, RET5, RET6, RET7, RETTR, ROLA, ROLX, RORA, RORX,
        STA, STBYTEA, STBYTEX, STOP, STRO, STX, SUBA, SUBSP, SUBX
        };

    // Addressing modes for instructions
    enum EAddrMode
    {
        NONE = 0,
        I = 1,
        D = 2,
        N = 4,
        S = 8,
        SF = 16,
        X = 32,
        SX = 64,
        SXF = 128,
        ALL = 255
    };

    // Format for symbols
    enum ESymbolFormat
    {
        F_NONE, F_1C, F_1D, F_2D, F_1H, F_2H
    };

    enum EExecState
    {
        EStart,
        ERun, ERunAwaitIO,
        EDebugAwaitIO, EDebugAwaitClick, EDebugRunToBP, EDebugSingleStep
    };

    enum EWaiting
    {
        ERunWaiting,
        EDebugSSWaiting,
        EDebugResumeWaiting,
    };

    enum EPane
    {
        ESource,
        EObject,
        EListing,
        EListingTrace,
        EMemoryTrace,
        EBatchIO,
        ETerminal,
    };

}

#endif // ENU_H

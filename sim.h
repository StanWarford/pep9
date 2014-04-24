// File: sim.h
/*
    Pep9 is a virtual machine for writing machine language and assembly
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

#ifndef SIM_H
#define SIM_H

#include <QVector>
#include "enu.h"

class Sim
{
public:    
    // The machine
    static QVector<int> Mem;
    static bool nBit, zBit, vBit, cBit;
    static int accumulator;
    static int indexRegister;
    static int stackPointer;
    static int programCounter;
    static int instructionSpecifier;
    static int operandSpecifier;
    static int operand;
    static int operandDisplayFieldWidth;

    static QString inputBuffer;
    static QString outputBuffer;

    static QSet<int> modifiedBytes;
    static bool trapped;
    static bool tracingTraps;

    static Enu::EExecState executionState;
    // State for keeping track of what actions are possible for user and machine

    static int toSignedDecimal(int value);
    // Pre: 0 <= value < 65536
    // Post: -32768 <= value < 32768 is returned

    static int fromSignedDecimal(int value);
    // Pre: -32768 <= value < 32768
    // Post: 0 <= value < 65536 is returned

    static int nzvcToInt();
    // Post: NZVC is returned in postions <4..7> of the one-byte int

    static int add(int lhs, int rhs);

    static int addAndSetNZVC(int lhs, int rhs);

    static void loadMem(QList<int> objectCodeList);

    static int readByte(int memAddr);
    static int readWord(int memAddr);
    static int readByteOprnd(Enu::EAddrMode addrMode);
    static int readWordOprnd(Enu::EAddrMode addrMode);

    static void writeByte(int memAddr, int value);
    // Pre: 0 <= value < 256
    // Post: Value is stored in Mem[memAddr]

    static void writeWord(int memAddr, int value);
    // Pre: 0 <= value < 65536
    // Post: The high-end byte of value is stored in Mem[memAddr]
    // and the low-end byte of value is stored in Mem[memAddr + 1]

    static void writeByteOprnd(Enu::EAddrMode addrMode, int value);
    static void writeWordOprnd(Enu::EAddrMode addrMode, int value);

    static int cellSize(Enu::ESymbolFormat symbolFormat);
    // This is used exclusively in the memoryTracePane/memoryCellGraphicsItem
    // I still disagree with where this is. It should be in the MemoryCellGraphicsItem
    // because that is what it is used for.

    static bool vonNeumannStep(QString &errorString);

};

#endif // SIM_H

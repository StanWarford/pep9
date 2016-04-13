// File: sim.cpp
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
#include "sim.h"
#include "pep.h"
#include <QMessageBox>
#include <QSet>
// #include <QDebug>

using namespace Enu;

// The machine
QVector<int> Sim::Mem(65536);
bool Sim::nBit, Sim::zBit, Sim::vBit, Sim::cBit;
int Sim::accumulator;
int Sim::indexRegister;
int Sim::stackPointer;
int Sim::programCounter;
int Sim::instructionSpecifier;
int Sim::operandSpecifier;
int Sim::operand;
int Sim::operandDisplayFieldWidth;

QString Sim::inputBuffer;
QString Sim::outputBuffer;

QSet<int> Sim::modifiedBytes;
bool Sim::trapped;
bool Sim::tracingTraps;

Enu::EExecState Sim::executionState;

int Sim::toSignedDecimal(int value)
{
    return value > 32767 ? value - 65536 : value;
}

int Sim::fromSignedDecimal(int value)
{
    return value < 0 ? value + 65536 : value;
}

int Sim::nzvcToInt()
{
    int i = 0;
    if (nBit) i |= 8;
    if (zBit) i |= 4;
    if (vBit) i |= 2;
    if (cBit) i |= 1;
    return i;
}

void Sim::loadMem(QList<int> objectCodeList) {
    for (int i = 0; objectCodeList.length() > 0; i++) {
        Mem[i] = objectCodeList.takeAt(0);
    }
}

int Sim::add(int lhs, int rhs)
{
    return (lhs + rhs) & 0xffff;
}

int Sim::addAndSetNZVC(int lhs, int rhs)
{
    int result = lhs + rhs;    
    if (result >= 65536) {
        Sim::cBit = true;
        result = result & 0xffff;
    }
    else {
        Sim::cBit = false;
    }
    Sim::nBit = result < 32768 ?  false : true;
    Sim::zBit = result == 0 ? true : false;
    Sim::vBit = (lhs < 32768 && rhs < 32768 && result >= 32768) ||
                (lhs >= 32768 && rhs >= 32768 && result < 32768);
    return result;
}

int Sim::readByte(int memAddr)
{
    return Mem[memAddr & 0xffff];
}

int Sim::readWord(int memAddr)
{
     return 256 * Mem[memAddr & 0xffff] + Mem[(memAddr + 1) & 0xffff];
}

int Sim::addrOfByteOprnd(Enu::EAddrMode addrMode)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        break;
    case Enu::D:
        return operandSpecifier;
    case Enu::N:
        return readWord(operandSpecifier);
    case Enu::S:
        return add(stackPointer, operandSpecifier);
    case Enu::SF:
        return readWord(add(stackPointer, operandSpecifier));
    case Enu::X:
        return add(operandSpecifier, indexRegister);
    case Enu::SX:
        return add(add(stackPointer, operandSpecifier), indexRegister);
    case Enu::SFX:
        return add(readWord(add(stackPointer, operandSpecifier)), indexRegister);
    case Enu::ALL:
        break;
    }
    return 0;
}

int Sim::readByteOprnd(Enu::EAddrMode addrMode)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        return operandSpecifier;
    case Enu::D:
        return readByte(operandSpecifier);
    case Enu::N:
        return readByte(readWord(operandSpecifier));
    case Enu::S:
        return readByte(add(stackPointer, operandSpecifier));
    case Enu::SF:
        return readByte(readWord(add(stackPointer, operandSpecifier)));
    case Enu::X:
        return readByte(add(operandSpecifier, indexRegister));
    case Enu::SX:
        return readByte(add(add(stackPointer, operandSpecifier), indexRegister));
    case Enu::SFX:
        return readByte(add(readWord(add(stackPointer, operandSpecifier)), indexRegister));
    case Enu::ALL:
        break;
    }
    return 0;
}

int Sim::readWordOprnd(Enu::EAddrMode addrMode)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        return operandSpecifier;
    case Enu::D:
        return readWord(operandSpecifier);
    case Enu::N:
        return readWord(readWord(operandSpecifier));
    case Enu::S:
        return readWord(add(stackPointer, operandSpecifier));
    case Enu::SF:
        return readWord(readWord(add(stackPointer, operandSpecifier)));
    case Enu::X:
        return readWord(add(operandSpecifier, indexRegister));
    case Enu::SX:
        return readWord(add(add(stackPointer, operandSpecifier), indexRegister));
    case Enu::SFX:
        return readWord(add(readWord(add(stackPointer, operandSpecifier)), indexRegister));
    case Enu::ALL:
        break;
    }
    return 0;
}

void Sim::writeByte(int memAddr, int value)
{
    if (memAddr < Pep::romStartAddress) {
        Mem[memAddr & 0xffff] = value;
        modifiedBytes.insert(memAddr & 0xffff);
    }
}

void Sim::writeWord(int memAddr, int value)
{
    if (memAddr < Pep::romStartAddress) { // There is an intentional inaccuracy here (it is possible to overwrite the first byte of ROM).
        Mem[memAddr & 0xffff] = value / 256;
        Mem[(memAddr + 1) & 0xffff] = value % 256;
        modifiedBytes.insert(memAddr & 0xffff);
        modifiedBytes.insert((memAddr + 1) & 0xffff);
    }
}

void Sim::writeByteOprnd(Enu::EAddrMode addrMode, int value)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        // illegal
        break;
    case Enu::D:
        writeByte(operandSpecifier, value);
        break;
    case Enu::N:
        writeByte(readWord(operandSpecifier), value);
        break;
    case Enu::S:
        writeByte(add(stackPointer, operandSpecifier), value);
        break;
    case Enu::SF:
        writeByte(readWord(add(stackPointer, operandSpecifier)), value);
        break;
    case Enu::X:
        writeByte(add(operandSpecifier, indexRegister), value);
        break;
    case Enu::SX:
        writeByte(add(add(stackPointer, operandSpecifier), indexRegister), value);
        break;
    case Enu::SFX:
        writeByte(add(readWord(add(stackPointer, operandSpecifier)), indexRegister), value);
        break;
    case Enu::ALL:
        break;
    }
}

void Sim::writeWordOprnd(Enu::EAddrMode addrMode, int value)
{
    switch (addrMode) {
    case Enu::NONE:
        break;
    case Enu::I:
        // illegal
        break;
    case Enu::D:
        writeWord(operandSpecifier, value);
        break;
    case Enu::N:
        writeWord(readWord(operandSpecifier), value);
        break;
    case Enu::S:
        writeWord(add(stackPointer, operandSpecifier), value);
        break;
    case Enu::SF:
        writeWord(readWord(add(stackPointer, operandSpecifier)), value);
        break;
    case Enu::X:
        writeWord(add(operandSpecifier, indexRegister), value);
        break;
    case Enu::SX:
        writeWord(add(add(stackPointer, operandSpecifier), indexRegister), value);
        break;
    case Enu::SFX:
        writeWord(add(readWord(add(stackPointer, operandSpecifier)), indexRegister), value);
        break;
    case Enu::ALL:
        break;
    }
}

int Sim::cellSize(Enu::ESymbolFormat symbolFormat)
{
    switch (symbolFormat) {
    case Enu::F_1C:
        return 1;
    case Enu::F_1D:
        return 1;
    case Enu::F_2D:
        return 2;
    case Enu::F_1H:
        return 1;
    case Enu::F_2H:
        return 2;
    default:
        // Should not occur
        return 0;
    }
}

bool Sim::vonNeumannStep(QString &errorString)
{
    modifiedBytes.clear();
    EMnemonic mnemonic;
    EAddrMode addrMode;
    int temp;
    bool bTemp;
    // Fetch
    instructionSpecifier = readByte(programCounter);
    // Increment
    programCounter = add(programCounter, 1);
    // Decode
    mnemonic = Pep::decodeMnemonic[instructionSpecifier];
    addrMode = Pep::decodeAddrMode[instructionSpecifier];
    if (!Pep::isUnaryMap[mnemonic]) {
        operandSpecifier = readWord(programCounter);
        programCounter = add(programCounter, 2);
    }
    // Execute
    //qDebug() << Pep::enumToMnemonMap[Pep::decodeMnemonic[instructionSpecifier]];
    if (!Pep::isUnaryMap[mnemonic] && !Pep::isTrapMap[mnemonic] && !(Pep::addrModesMap.value(mnemonic) & addrMode )) {
        errorString = "Invalid Addressing Mode.";
        return false;
    }

    switch (Pep::decodeMnemonic[instructionSpecifier]) {
    case ADDA:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        accumulator = addAndSetNZVC(accumulator, operand);
        return true;
    case ADDX:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        indexRegister = addAndSetNZVC(indexRegister, operand);
        return true;
    case ADDSP:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        stackPointer = add(stackPointer, operand);
        return true;
    case ANDA:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        accumulator = accumulator & operand;
        nBit = accumulator > 32768;
        zBit = accumulator == 0;
        return true;
    case ANDX:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        indexRegister = indexRegister & operand;
        nBit = indexRegister > 32768;
        zBit = indexRegister == 0;
        return true;
    case ASLA:
        vBit = (accumulator >= 0x4000 && accumulator < 0x8000) || // prefix is 01 (bin)
                    (accumulator >= 0x8000 && accumulator < 0xC000); // prefix is 10 (bin)
        accumulator *= 2;
        if (accumulator >= 65536) {
            cBit = 1;
            accumulator = accumulator & 0xffff;
        }
        else {
            cBit = 0;
        }
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case ASLX:
        vBit = (indexRegister >= 0x4000 && indexRegister < 0x8000) || // prefix is 01 (bin)
                    (indexRegister >= 0x8000 && indexRegister < 0xC000); // prefix is 10 (bin)
        indexRegister *= 2;
        if (indexRegister >= 65536) {
            cBit = 1;
            indexRegister = indexRegister & 0xffff;
        }
        else {
            cBit = 0;
        }
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case ASRA:
        cBit = (accumulator % 2) == 1;
        if (accumulator < 32768) {
            accumulator /= 2;
        }
        else {
            accumulator = accumulator / 2 + 32768;
        }
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case ASRX:
        cBit = (indexRegister % 2) == 1;
        if (indexRegister < 32768) {
            indexRegister /= 2;
        }
        else {
            indexRegister = indexRegister / 2 + 32768;
        }
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case BR:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        programCounter = operand;
        return true;
    case BRC:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (cBit) {
            programCounter = operand;
        }
        return true;
    case BREQ:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (zBit) {
            programCounter = operand;
        }
        return true;
    case BRGE:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (!nBit) {
            programCounter = operand;
        }
        return true;
    case BRGT:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (!nBit && !zBit) {
            programCounter = operand;
        }
        return true;
    case BRLE:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (nBit || zBit) {
            programCounter = operand;
        }
        return true;
    case BRLT:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (nBit) {
            programCounter = operand;
        }
        return true;
    case BRNE:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (!zBit) {
            programCounter = operand;
        }
        return true;
    case BRV:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        if (vBit) {
            programCounter = operand;
        }
        return true;
    case CALL:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        stackPointer = add(stackPointer, 65534); // SP <- SP - 2
        writeWord(stackPointer, programCounter); // Mem[SP] <- PC
        programCounter = operand; // PC <- Oprnd
        return true;
    case CPBA:
        operand = readByteOprnd(addrMode);
        operandDisplayFieldWidth = 2;
        temp = (accumulator & 0x00ff) - operand;
        Sim::nBit = temp < 0;
        Sim::zBit = temp == 0;
        Sim::vBit = false;
        Sim::cBit = false;
        return true;
    case CPBX:
        operand = readByteOprnd(addrMode);
        operandDisplayFieldWidth = 2;
        temp = (indexRegister & 0x00ff) - operand;
        Sim::nBit = temp < 0;
        Sim::zBit = temp == 0;
        Sim::vBit = false;
        Sim::cBit = false;
        return true;
    case CPWA:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        addAndSetNZVC(accumulator, (~operand + 1) & 0xffff);
        if (vBit) { // Extend compare range. nBit and zBit are not adjusted in subtract instructions.
            nBit = !nBit;
        }
        return true;
    case CPWX:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        addAndSetNZVC(indexRegister, (~operand + 1) & 0xffff);
        if (vBit) { // Extend compare range. nBit and zBit are not adjusted in subtract instructions.
            nBit = !nBit;
        }
        return true;
    case DECI: case DECO: case HEXO: case STRO:
    case NOP: case NOP0: case NOP1:
        temp = readWord(Pep::dotBurnArgument - 9);
        // 9 is the vector offset from the last byte of the OS for the System stack pointer
        writeByte(temp - 1, instructionSpecifier);
        writeWord(temp - 3, stackPointer);
        writeWord(temp - 5, programCounter);
        writeWord(temp - 7, indexRegister);
        writeWord(temp - 9, accumulator);
        writeByte(temp - 10, nzvcToInt());
        stackPointer = temp - 10;
        programCounter = readWord(Pep::dotBurnArgument - 1);
        indexRegister = 0; // To compensate for a bug in Pep/9 OS, where it should be done. (But the book is published already!)
        return true;
    case LDBA:
        if (addrMode != Enu::I && addrOfByteOprnd(addrMode) == 256 * Mem[Pep::dotBurnArgument - 7] + Mem[Pep::dotBurnArgument - 6]) {
            // Memory-mapped input
            if (Sim::inputBuffer.size() != 0) {
                QString ch = Sim::inputBuffer.left(1);
                Sim::inputBuffer.remove(0, 1);
                operand = QChar(ch[0]).toLatin1();
                operand += operand < 0 ? 256 : 0;
            } else {
                // Attempt to read past end of input.
                // Only happens with batch input.
                operand = 0;
            }
        } else {
            operand = readByteOprnd(addrMode);
        }
        operandDisplayFieldWidth = 2;
        accumulator = accumulator & 0xff00;
        accumulator |= operand & 255;
        nBit = 0;
        zBit = operand == 0;
        return true;
    case LDBX:
        if (addrMode != Enu::I && addrOfByteOprnd(addrMode) == 256 * Mem[Pep::dotBurnArgument - 7] + Mem[Pep::dotBurnArgument - 6]) {
            // Memory-mapped input
            if (Sim::inputBuffer.size() != 0) {
                QString ch = Sim::inputBuffer.left(1);
                Sim::inputBuffer.remove(0, 1);
                operand = QChar(ch[0]).toLatin1();
                operand += operand < 0 ? 256 : 0;
            } else {
                // Attempt to read past end of input.
                // Only happens with batch input.
                operand = 0;
            }
        } else {
            operand = readByteOprnd(addrMode);
        }
        operandDisplayFieldWidth = 2;
        indexRegister = indexRegister & 0xff00;
        indexRegister |= operand & 255;
        nBit = 0;
        zBit = operand == 0;
        return true;
    case LDWA:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        accumulator = operand & 0xffff;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case LDWX:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        indexRegister = operand & 0xffff;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case MOVAFLG:
        cBit = (accumulator & 0x0001) != 0;
        vBit = (accumulator & 0x0002) != 0;
        zBit = (accumulator & 0x0004) != 0;
        nBit = (accumulator & 0x0008) != 0;
        return true;
    case MOVFLGA:
        accumulator = accumulator & 0xff00;
        accumulator |= cBit ? 1 : 0;
        accumulator |= vBit ? 2 : 0;
        accumulator |= zBit ? 4 : 0;
        accumulator |= nBit ? 8 : 0;
        return true;
    case MOVSPA:
        accumulator = stackPointer;
        return true;
    case NEGA:
        accumulator = (~accumulator + 1) & 0xffff;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        vBit = accumulator == 32768;
        return true;
    case NEGX:
        indexRegister = (~indexRegister + 1) & 0xffff;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        vBit = indexRegister == 32768;
        return true;
    case NOTA:
        accumulator = ~accumulator & 0xffff;
        nBit = accumulator >= 32768;
        zBit = accumulator == 0;
        return true;
    case NOTX:
        indexRegister = ~indexRegister & 0xffff;
        nBit = indexRegister >= 32768;
        zBit = indexRegister == 0;
        return true;
    case ORA:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        accumulator = accumulator | operand;
        nBit = accumulator > 32768;
        zBit = accumulator == 0;
        return true;
    case ORX:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        indexRegister = indexRegister | operand;
        nBit = indexRegister > 32768;
        zBit = indexRegister == 0;
        return true;
    case RET:
        programCounter = readWord(stackPointer); // PC <- Mem[SP]
        stackPointer = add(stackPointer, 2); // SP <- SP + 2
        return true;
    case RETTR:
        temp = readByte(stackPointer);
        nBit = (temp & 8) != 0;
        zBit = (temp & 4) != 0;
        vBit = (temp & 2) != 0;
        cBit = (temp & 1) != 0;
        accumulator = readWord(stackPointer + 1);
        indexRegister = readWord(stackPointer + 3);
        programCounter = readWord(stackPointer + 5);
        stackPointer = readWord(stackPointer + 7);
        return true;
    case ROLA:
        bTemp = accumulator >= 32768;
        accumulator = (accumulator * 2) & 0xffff;
        accumulator |= cBit ? 1 : 0;
        cBit = bTemp;
        return true;
    case ROLX:
        bTemp = indexRegister >= 32768;
        indexRegister = (indexRegister * 2) & 0xffff;
        indexRegister |= cBit ? 1 : 0;
        cBit = bTemp;
        return true;
    case RORA:
        bTemp = accumulator % 2 == 1;
        accumulator = (accumulator / 2);
        accumulator |= cBit ? 0x8000 : 0;
        cBit = bTemp;
        return true;
    case RORX:
        bTemp = indexRegister % 2 == 1;
        indexRegister = (indexRegister / 2);
        indexRegister |= cBit ? 0x8000 : 0;
        cBit = bTemp;
        return true;
    case STBA:
        operand = accumulator & 0x00ff;
        if (addrOfByteOprnd(addrMode) == 256 * Mem[Pep::dotBurnArgument - 5] + Mem[Pep::dotBurnArgument - 4]) {
            // Memory-mapped output
            Sim::outputBuffer = QString(operand);
        } else {
            writeByteOprnd(addrMode, operand);
        }
        operandDisplayFieldWidth = 2;
        return true;
    case STBX:
        operand = indexRegister & 0x00ff;
        if (addrOfByteOprnd(addrMode) == 256 * Mem[Pep::dotBurnArgument - 5] + Mem[Pep::dotBurnArgument - 4]) {
            // Memory-mapped output
            Sim::outputBuffer = QString(operand);
        } else {
            writeByteOprnd(addrMode, operand);
        }
        operandDisplayFieldWidth = 2;
        return true;
    case STWA:
        writeWordOprnd(addrMode, accumulator);
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        return true;
    case STWX:
        writeWordOprnd(addrMode, indexRegister);
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        return true;
    case STOP:
        return true;
    case SUBA:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        accumulator = addAndSetNZVC(accumulator, (~operand + 1) & 0xffff);
        return true;
    case SUBX:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        indexRegister = addAndSetNZVC(indexRegister, (~operand + 1) & 0xffff);
        return true;
    case SUBSP:
        operand = readWordOprnd(addrMode);
        operandDisplayFieldWidth = 4;
        stackPointer = add(stackPointer, (~operand + 1) & 0xffff);
        return true;
    default:
        return false;
    }
    return false;
}

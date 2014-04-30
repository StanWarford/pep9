// File: argument.h
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

#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "asm.h"
#include "pep.h"
#include "sim.h"

// Abstract Argument class
class Argument
{
    friend class Asm;
public:
    virtual ~Argument() { }
    virtual int getArgumentValue() = 0;
    virtual QString getArgumentString() = 0;
};

// Concrete argument classes
// All methods are defined in this argument.h file.
class CharArgument: public Argument
{
    friend class Asm;
private:
    QString charValue;
public:
    CharArgument(QString cValue) { charValue = cValue; }
    int getArgumentValue() { return Asm::charStringToInt(charValue); }
    QString getArgumentString() { return charValue; }
};

class DecArgument: public Argument
{
private:
    int decValue;
public:
    DecArgument(int dValue) { decValue = dValue; }
    int getArgumentValue() { return decValue; }
    QString getArgumentString() {
        int temp = decValue >= 32768 ? decValue - 65536 : decValue;
        return QString("%1").arg(temp);
    }
};

class UnsignedDecArgument: public Argument
{
private:
    int decValue;
public:
    UnsignedDecArgument(int dValue) { decValue = dValue; }
    int getArgumentValue() { return decValue; }
    QString getArgumentString() {
        return QString("%1").arg(decValue);
    }
};

class HexArgument: public Argument
{
private:
    int hexValue;
public:
    HexArgument(int hValue) { hexValue = hValue; }
    int getArgumentValue() { return hexValue; }
    QString getArgumentString() { return "0x" + QString("%1").arg(hexValue, 4, 16, QLatin1Char('0')).toUpper(); }
};

class StringArgument: public Argument
{
private:
    QString stringValue;
public:
    StringArgument(QString sValue) { stringValue = sValue; }
    int getArgumentValue() { return Asm::string2ArgumentToInt(stringValue); }
    QString getArgumentString() { return stringValue; }
};

class SymbolRefArgument: public Argument
{
private:
    QString symbolRefValue;
public:
    SymbolRefArgument(QString sRefValue) { symbolRefValue = sRefValue; }
    int getArgumentValue() {
        if (symbolRefValue == "charIn") {
            return Pep::symbolTable.contains("charIn") ? Pep::symbolTable.value(symbolRefValue) : 256 * Sim::Mem[0xfff8] + Sim::Mem[0xfff9];
        }
        else if (symbolRefValue == "charOut") {
            return Pep::symbolTable.contains("charOut") ? Pep::symbolTable.value(symbolRefValue) : 256 * Sim::Mem[0xfffa] + Sim::Mem[0xfffb];
        }
        else {
            return Pep::symbolTable.value(symbolRefValue);
        }
    }
    QString getArgumentString() { return symbolRefValue; }
};

#endif // ARGUMENT_H

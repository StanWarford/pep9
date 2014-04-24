// File: asm.h
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

#ifndef ASM_H
#define ASM_H

#include <QRegExp>
#include "enu.h"

class Code; // Forward declaration for argument of processSourceLine.

class Asm
{
public:
    // Lexical tokens
    enum ELexicalToken
    {
        LT_ADDRESSING_MODE, LT_CHAR_CONSTANT, LT_COMMENT, LT_DEC_CONSTANT, LT_DOT_COMMAND,
        LT_EMPTY, LT_HEX_CONSTANT, LT_IDENTIFIER, LT_STRING_CONSTANT, LT_SYMBOL_DEF
    };

    enum ParseState
    {
        PS_ADDRESSING_MODE, PS_CLOSE, PS_COMMENT, PS_DOT_ADDRSS, PS_DOT_ASCII, PS_DOT_BLOCK,
        PS_DOT_BURN, PS_DOT_BYTE, PS_DOT_END, PS_DOT_EQUATE, PS_DOT_WORD, PS_FINISH,
        PS_INSTRUCTION, PS_START, PS_STRING, PS_SYMBOL_DEF
    };

    // Regular expressions for lexical analysis
    static QRegExp rxAddrMode;
    static QRegExp rxCharConst;
    static QRegExp rxComment;
    static QRegExp rxDecConst;
    static QRegExp rxDotCommand;
    static QRegExp rxHexConst;
    static QRegExp rxIdentifier;
    static QRegExp rxStringConst;

    // Regular expressions for trace tag analysis
    static QRegExp rxFormatTag;
    static QRegExp rxSymbolTag;
    static QRegExp rxArrayMultiplier;

    static bool getToken(QString &sourceLine, ELexicalToken &token, QString &tokenString);
    // Pre: sourceLine has one line of source code.
    // Post: If the next token is valid, the string of characters representing the next token are deleted from the
    // beginning of sourceLine and returned in tokenString, true is returned, and token is set to the token type.
    // Post: If false is returned, then tokenString is set to the lexical error message.

    static bool processSourceLine(QString sourceLine, int lineNum, Code *&code, QString &errorString, bool &dotEndDetected);
    // Pre: sourceLine has one line of source code.
    // Pre: lineNum is the line number of the source code.
    // Post: If the source line is valid, true is returned and code is set to the source code for the line.
    // Post: dotEndDetected is set to true if .END is processed. Otherwise it is set to false.
    // Post: Pep::byteCount is incremented by the number of bytes generated.
    // Post: If the source line is not valid, false is returned and errorString is set to the error message.

    static QList<QString> listOfReferencedSymbols;
    static QList<int> listOfReferencedSymbolLineNums;

    static bool startsWithHexPrefix(QString str);
    // Post: Returns true if str starts with the characters 0x or 0X. Otherwise returns false.

    static Enu::EAddrMode stringToAddrMode(QString str);
    // Post: Returns the addressing mode integer defined in Pep from its string representation.

    static int charStringToInt(QString str);
    // Pre: str is enclosed in single quotes.
    // Post: Returns the ASCII integer value of the character accounting for \ quoted characters.

    static int string2ArgumentToInt(QString str);
    // Pre: str is enclosed in double quotes and contains at most two possibly quoted characters.
    // Post: Returns the two-byte ASCII integer value for the string.

    static void unquotedStringToInt(QString &str, int &value);
    // Pre: str is a character or string stripped of its single or double quotes.
    // Post: The sequence of characters representing the first possibly \ quoted character
    // is stripped from the beginning of str.
    // Post: value is the ASCII integer value of the first possibly \ quoted character.

    static int byteStringLength(QString str);
    // Pre: str is a double quoted string.
    // Post: Returns the byte length of str accounting for possibly \ quoted characters.

    static Enu::ESymbolFormat formatTagType(QString formatTag);
    // Pre: formatTag is a valid format trace tag.
    // Post: Returns the enumerated trace tag format type.

    static int tagNumBytes(Enu::ESymbolFormat symbolFormat);
    // Pre: symbolFormat is a valid format trace tag type.
    // Post: Returns the corresponding integer number of bytes.

    static int formatMultiplier(QString formatTag);
    // Pre: format tag is a valid format trace tag.
    // Post: If the format tag specifies an array, returns the array multiplier.
    // Otherwise, returns 1.

};

#endif // ASM_H

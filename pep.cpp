// File: pep.cpp
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
#include <QFile>
#include <QTextStream>
#include "pep.h"

using namespace Enu;

// Fonts
const QString Pep::codeFont = getSystem() == "Windows" ? "Courier New" : (getSystem() == "Mac" ? "Courier" : "Courier 10 Pitch");
const int Pep::codeFontSize = getSystem() == "Mac" ? 12 : 9;
const int Pep::ioFontSize = getSystem() == "Mac" ? 13 : 10;
const QString Pep::labelFont = getSystem() == "Mac" ? "Lucida Grande" : "Verdana";
const int Pep::labelFontSize = getSystem() == "Mac" ? 13 : 10;

// Default redefine mnemonics
const QString Pep::defaultUnaryMnemonic0 = "NOP0";
const QString Pep::defaultUnaryMnemonic1 = "NOP1";
const QString Pep::defaultNonUnaryMnemonic0 = "NOP";
const bool Pep::defaultMnemon0i = true;
const bool Pep::defaultMnemon0d = false;
const bool Pep::defaultMnemon0n = false;
const bool Pep::defaultMnemon0s = false;
const bool Pep::defaultMnemon0sf = false;
const bool Pep::defaultMnemon0x = false;
const bool Pep::defaultMnemon0sx = false;
const bool Pep::defaultMnemon0sxf = false;
const QString Pep::defaultNonUnaryMnemonic1 = "DECI";
const bool Pep::defaultMnemon1i = false;
const bool Pep::defaultMnemon1d = true;
const bool Pep::defaultMnemon1n = true;
const bool Pep::defaultMnemon1s = true;
const bool Pep::defaultMnemon1sf = true;
const bool Pep::defaultMnemon1x = true;
const bool Pep::defaultMnemon1sx = true;
const bool Pep::defaultMnemon1sxf = true;
const QString Pep::defaultNonUnaryMnemonic2 = "DECO";
const bool Pep::defaultMnemon2i = true;
const bool Pep::defaultMnemon2d = true;
const bool Pep::defaultMnemon2n = true;
const bool Pep::defaultMnemon2s = true;
const bool Pep::defaultMnemon2sf = true;
const bool Pep::defaultMnemon2x = true;
const bool Pep::defaultMnemon2sx = true;
const bool Pep::defaultMnemon2sxf = true;
const QString Pep::defaultNonUnaryMnemonic3 = "HEXO";
const bool Pep::defaultMnemon3i = true;
const bool Pep::defaultMnemon3d = true;
const bool Pep::defaultMnemon3n = true;
const bool Pep::defaultMnemon3s = true;
const bool Pep::defaultMnemon3sf = true;
const bool Pep::defaultMnemon3x = true;
const bool Pep::defaultMnemon3sx = true;
const bool Pep::defaultMnemon3sxf = true;
const QString Pep::defaultNonUnaryMnemonic4 = "STRO";
const bool Pep::defaultMnemon4i = false;
const bool Pep::defaultMnemon4d = true;
const bool Pep::defaultMnemon4n = true;
const bool Pep::defaultMnemon4s = false;
const bool Pep::defaultMnemon4sf = true;
const bool Pep::defaultMnemon4x = false;
const bool Pep::defaultMnemon4sx = false;
const bool Pep::defaultMnemon4sxf = false;

int Pep::aaaAddressField(EAddrMode addressMode)
{
    if (addressMode == I) return 0;
    if (addressMode == D) return 1;
    if (addressMode == N) return 2;
    if (addressMode == S) return 3;
    if (addressMode == SF) return 4;
    if (addressMode == X) return 5;
    if (addressMode == SX) return 6;
    if (addressMode == SXF) return 7;
    return -1; // Should not occur;
}

int Pep::aAddressField(EAddrMode addressMode)
{
    if (addressMode == I) return 0;
    if (addressMode == X) return 1;
    return -1; // Should not occur;
}

QString Pep::intToAddrMode(EAddrMode addressMode) {
    if (addressMode == I) return "i";
    if (addressMode == D) return "d";
    if (addressMode == N) return "n";
    if (addressMode == S) return "s";
    if (addressMode == SF) return "sf";
    if (addressMode == X) return "x";
    if (addressMode == SX) return "sx";
    if (addressMode == SXF) return "sxf";
    return ""; // Should not occur
}

QString Pep::commaSpaceToAddrMode(EAddrMode addressMode) {
    if (addressMode == NONE) return "";
    if (addressMode == I) return ", i";
    if (addressMode == D) return ", d";
    if (addressMode == N) return ", n";
    if (addressMode == S) return ", s";
    if (addressMode == SF) return ", sf";
    if (addressMode == X) return ", x";
    if (addressMode == SX) return ", sx";
    if (addressMode == SXF) return ", sxf";
    return ""; // Should not occur
}

// Function to read text from a resource file
QString Pep::resToString(QString fileName) {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QString inString = "";
    while (!in.atEnd()) {
        QString line = in.readLine();
        inString.append(line + "\n");
    }
    return inString;
}

QString Pep::getSystem() {
    #ifdef Q_OS_LINUX
    return QString("Linux");
    #endif

    #ifdef Q_OS_OSX
    return QString("Mac");
    #endif

    #ifdef Q_OS_WIN
    return QString("Windows");
    #endif

    return QString("No OS");
}

// Maps between mnemonic enums and strings
QMap<Enu::EMnemonic, QString> Pep::enumToMnemonMap;
QMap<QString, Enu::EMnemonic> Pep::mnemonToEnumMap;
void Pep::initEnumMnemonMaps()
{
    enumToMnemonMap.clear(); mnemonToEnumMap.clear(); // Can be called from Redefine Mnemonics
    enumToMnemonMap.insert(ADDA, "ADDA"); mnemonToEnumMap.insert("ADDA", ADDA);
    enumToMnemonMap.insert(ADDSP, "ADDSP"); mnemonToEnumMap.insert("ADDSP", ADDSP);
    enumToMnemonMap.insert(ADDX, "ADDX"); mnemonToEnumMap.insert("ADDX", ADDX);
    enumToMnemonMap.insert(ANDA, "ANDA"); mnemonToEnumMap.insert("ANDA", ANDA);
    enumToMnemonMap.insert(ANDX, "ANDX"); mnemonToEnumMap.insert("ANDX", ANDX);
    enumToMnemonMap.insert(ASLA, "ASLA"); mnemonToEnumMap.insert("ASLA", ASLA);
    enumToMnemonMap.insert(ASLX, "ASLX"); mnemonToEnumMap.insert("ASLX", ASLX);
    enumToMnemonMap.insert(ASRA, "ASRA"); mnemonToEnumMap.insert("ASRA", ASRA);
    enumToMnemonMap.insert(ASRX, "ASRX"); mnemonToEnumMap.insert("ASRX", ASRX);
    enumToMnemonMap.insert(BR, "BR"); mnemonToEnumMap.insert("BR", BR);
    enumToMnemonMap.insert(BRC, "BRC"); mnemonToEnumMap.insert("BRC", BRC);
    enumToMnemonMap.insert(BREQ, "BREQ"); mnemonToEnumMap.insert("BREQ", BREQ);
    enumToMnemonMap.insert(BRGE, "BRGE"); mnemonToEnumMap.insert("BRGE", BRGE);
    enumToMnemonMap.insert(BRGT, "BRGT"); mnemonToEnumMap.insert("BRGT", BRGT);
    enumToMnemonMap.insert(BRLE, "BRLE"); mnemonToEnumMap.insert("BRLE", BRLE);
    enumToMnemonMap.insert(BRLT, "BRLT"); mnemonToEnumMap.insert("BRLT", BRLT);
    enumToMnemonMap.insert(BRNE, "BRNE"); mnemonToEnumMap.insert("BRNE", BRNE);
    enumToMnemonMap.insert(BRV, "BRV"); mnemonToEnumMap.insert("BRV", BRV);
    enumToMnemonMap.insert(CALL, "CALL"); mnemonToEnumMap.insert("CALL", CALL);
    // FOR DEBUGGING NOW. ELIMINATE LATER *************************************************
    enumToMnemonMap.insert(CHARI, "CHARI"); mnemonToEnumMap.insert("CHARI", CHARI);
    enumToMnemonMap.insert(CHARO, "CHARO"); mnemonToEnumMap.insert("CHARO", CHARO);

    enumToMnemonMap.insert(CPBA, "CPBA"); mnemonToEnumMap.insert("CPBA", CPBA);
    enumToMnemonMap.insert(CPBX, "CPBX"); mnemonToEnumMap.insert("CPBX", CPBX);
    enumToMnemonMap.insert(CPWA, "CPWA"); mnemonToEnumMap.insert("CPWA", CPWA);
    enumToMnemonMap.insert(CPWX, "CPWX"); mnemonToEnumMap.insert("CPWX", CPWX);
    enumToMnemonMap.insert(DECI, defaultNonUnaryMnemonic1); mnemonToEnumMap.insert(defaultNonUnaryMnemonic1, DECI);
    enumToMnemonMap.insert(DECO, defaultNonUnaryMnemonic2); mnemonToEnumMap.insert(defaultNonUnaryMnemonic2, DECO);
    enumToMnemonMap.insert(HEXO, defaultNonUnaryMnemonic3); mnemonToEnumMap.insert(defaultNonUnaryMnemonic3, HEXO);
    enumToMnemonMap.insert(LDBA, "LDBA"); mnemonToEnumMap.insert("LDBA", LDBA);
    enumToMnemonMap.insert(LDWA, "LDWA"); mnemonToEnumMap.insert("LDWA", LDWA);
    enumToMnemonMap.insert(LDBX, "LDBX"); mnemonToEnumMap.insert("LDBX", LDBX);
    enumToMnemonMap.insert(LDWX, "LDWX"); mnemonToEnumMap.insert("LDWX", LDWX);
    enumToMnemonMap.insert(MOVAFLG, "MOVAFLG"); mnemonToEnumMap.insert("MOVAFLG", MOVAFLG);
    enumToMnemonMap.insert(MOVFLGA, "MOVFLGA"); mnemonToEnumMap.insert("MOVFLGA", MOVFLGA);
    enumToMnemonMap.insert(MOVSPA, "MOVSPA"); mnemonToEnumMap.insert("MOVSPA", MOVSPA);
    enumToMnemonMap.insert(NEGA, "NEGA"); mnemonToEnumMap.insert("NEGA", NEGA);
    enumToMnemonMap.insert(NEGX, "NEGX"); mnemonToEnumMap.insert("NEGX", NEGX);
    enumToMnemonMap.insert(NOP, defaultNonUnaryMnemonic0); mnemonToEnumMap.insert(defaultNonUnaryMnemonic0, NOP);
    enumToMnemonMap.insert(NOP0, defaultUnaryMnemonic0); mnemonToEnumMap.insert(defaultUnaryMnemonic0, NOP0);
    enumToMnemonMap.insert(NOP1, defaultUnaryMnemonic1); mnemonToEnumMap.insert(defaultUnaryMnemonic1, NOP1);
    enumToMnemonMap.insert(NOTA, "NOTA"); mnemonToEnumMap.insert("NOTA", NOTA);
    enumToMnemonMap.insert(NOTX, "NOTX"); mnemonToEnumMap.insert("NOTX", NOTX);
    enumToMnemonMap.insert(ORA, "ORA"); mnemonToEnumMap.insert("ORA", ORA);
    enumToMnemonMap.insert(ORX, "ORX"); mnemonToEnumMap.insert("ORX", ORX);
    enumToMnemonMap.insert(RET, "RET"); mnemonToEnumMap.insert("RET", RET);
    enumToMnemonMap.insert(RETTR, "RETTR"); mnemonToEnumMap.insert("RETTR", RETTR);
    enumToMnemonMap.insert(ROLA, "ROLA"); mnemonToEnumMap.insert("ROLA", ROLA);
    enumToMnemonMap.insert(ROLX, "ROLX"); mnemonToEnumMap.insert("ROLX", ROLX);
    enumToMnemonMap.insert(RORA, "RORA"); mnemonToEnumMap.insert("RORA", RORA);
    enumToMnemonMap.insert(RORX, "RORX"); mnemonToEnumMap.insert("RORX", RORX);
    enumToMnemonMap.insert(STBA, "STBA"); mnemonToEnumMap.insert("STBA", STBA);
    enumToMnemonMap.insert(STBX, "STBX"); mnemonToEnumMap.insert("STBX", STBX);
    enumToMnemonMap.insert(STWA, "STWA"); mnemonToEnumMap.insert("STWA", STWA);
    enumToMnemonMap.insert(STWX, "STWX"); mnemonToEnumMap.insert("STWX", STWX);
    enumToMnemonMap.insert(STOP, "STOP"); mnemonToEnumMap.insert("STOP", STOP);
    enumToMnemonMap.insert(STRO, defaultNonUnaryMnemonic4); mnemonToEnumMap.insert(defaultNonUnaryMnemonic4, STRO);
    enumToMnemonMap.insert(SUBA, "SUBA"); mnemonToEnumMap.insert("SUBA", SUBA);
    enumToMnemonMap.insert(SUBSP, "SUBSP"); mnemonToEnumMap.insert("SUBSP", SUBSP);
    enumToMnemonMap.insert(SUBX, "SUBX"); mnemonToEnumMap.insert("SUBX", SUBX);
}

// Maps to characterize each instruction
QMap<Enu::EMnemonic, int> Pep::opCodeMap;
QMap<Enu::EMnemonic, bool> Pep::isUnaryMap;
QMap<Enu::EMnemonic, bool> Pep::addrModeRequiredMap;
QMap<Enu::EMnemonic, bool> Pep::isTrapMap;

void Pep::initMnemonicMaps()
{
    opCodeMap.insert(ADDA, 96); isUnaryMap.insert(ADDA, false); addrModeRequiredMap.insert(ADDA, true); isTrapMap.insert(ADDA, false);
    opCodeMap.insert(ADDSP, 80); isUnaryMap.insert(ADDSP, false); addrModeRequiredMap.insert(ADDSP, true); isTrapMap.insert(ADDSP, false);
    opCodeMap.insert(ADDX, 104); isUnaryMap.insert(ADDX, false); addrModeRequiredMap.insert(ADDX, true); isTrapMap.insert(ADDX, false);
    opCodeMap.insert(ANDA, 128); isUnaryMap.insert(ANDA, false); addrModeRequiredMap.insert(ANDA, true); isTrapMap.insert(ANDA, false);
    opCodeMap.insert(ANDX, 136); isUnaryMap.insert(ANDX, false); addrModeRequiredMap.insert(ANDX, true); isTrapMap.insert(ANDX, false);
    opCodeMap.insert(ASLA, 10); isUnaryMap.insert(ASLA, true); addrModeRequiredMap.insert(ASLA, true); isTrapMap.insert(ASLA, false);
    opCodeMap.insert(ASLX, 11); isUnaryMap.insert(ASLX, true); addrModeRequiredMap.insert(ASLX, true); isTrapMap.insert(ASLX, false);
    opCodeMap.insert(ASRA, 12); isUnaryMap.insert(ASRA, true); addrModeRequiredMap.insert(ASRA, true); isTrapMap.insert(ASRA, false);
    opCodeMap.insert(ASRX, 13); isUnaryMap.insert(ASRX, true); addrModeRequiredMap.insert(ASRX, true); isTrapMap.insert(ASRX, false);

    opCodeMap.insert(BR, 18); isUnaryMap.insert(BR, false); addrModeRequiredMap.insert(BR, false); isTrapMap.insert(BR, false);
    opCodeMap.insert(BRC, 34); isUnaryMap.insert(BRC, false); addrModeRequiredMap.insert(BRC, false); isTrapMap.insert(BRC, false);
    opCodeMap.insert(BREQ, 24); isUnaryMap.insert(BREQ, false); addrModeRequiredMap.insert(BREQ, false); isTrapMap.insert(BREQ, false);
    opCodeMap.insert(BRGE, 28); isUnaryMap.insert(BRGE, false); addrModeRequiredMap.insert(BRGE, false); isTrapMap.insert(BRGE, false);
    opCodeMap.insert(BRGT, 30); isUnaryMap.insert(BRGT, false); addrModeRequiredMap.insert(BRGT, false); isTrapMap.insert(BRGT, false);
    opCodeMap.insert(BRLE, 20); isUnaryMap.insert(BRLE, false); addrModeRequiredMap.insert(BRLE, false); isTrapMap.insert(BRLE, false);
    opCodeMap.insert(BRLT, 22); isUnaryMap.insert(BRLT, false); addrModeRequiredMap.insert(BRLT, false); isTrapMap.insert(BRLT, false);
    opCodeMap.insert(BRNE, 26); isUnaryMap.insert(BRNE, false); addrModeRequiredMap.insert(BRNE, false); isTrapMap.insert(BRNE, false);
    opCodeMap.insert(BRV, 32); isUnaryMap.insert(BRV, false); addrModeRequiredMap.insert(BRV, false); isTrapMap.insert(BRV, false);

    opCodeMap.insert(CALL, 36); isUnaryMap.insert(CALL, false); addrModeRequiredMap.insert(CALL, false); isTrapMap.insert(CALL, false);
    // FOR DEBUGGING. REMOVE LATER ***************************************************************
    opCodeMap.insert(CHARI, 0); isUnaryMap.insert(CHARI, false); addrModeRequiredMap.insert(CHARI, true); isTrapMap.insert(CHARI, false);
    opCodeMap.insert(CHARO, 0); isUnaryMap.insert(CHARO, false); addrModeRequiredMap.insert(CHARO, true); isTrapMap.insert(CHARO, false);
    // ******************************************************
    opCodeMap.insert(CPBA, 176); isUnaryMap.insert(CPBA, false); addrModeRequiredMap.insert(CPBA, true); isTrapMap.insert(CPBA, false);
    opCodeMap.insert(CPBX, 184); isUnaryMap.insert(CPBX, false); addrModeRequiredMap.insert(CPBX, true); isTrapMap.insert(CPBX, false);
    opCodeMap.insert(CPWA, 160); isUnaryMap.insert(CPWA, false); addrModeRequiredMap.insert(CPWA, true); isTrapMap.insert(CPWA, false);
    opCodeMap.insert(CPWX, 168); isUnaryMap.insert(CPWX, false); addrModeRequiredMap.insert(CPWX, true); isTrapMap.insert(CPWX, false);

    opCodeMap.insert(DECI, 48); isUnaryMap.insert(DECI, false); addrModeRequiredMap.insert(DECI, true); isTrapMap.insert(DECI, true);
    opCodeMap.insert(DECO, 56); isUnaryMap.insert(DECO, false); addrModeRequiredMap.insert(DECO, true); isTrapMap.insert(DECO, true);

    opCodeMap.insert(HEXO, 64); isUnaryMap.insert(HEXO, false); addrModeRequiredMap.insert(HEXO, true); isTrapMap.insert(HEXO, true);

    opCodeMap.insert(LDBA, 208); isUnaryMap.insert(LDBA, false); addrModeRequiredMap.insert(LDBA, true); isTrapMap.insert(LDBA, false);
    opCodeMap.insert(LDBX, 216); isUnaryMap.insert(LDBX, false); addrModeRequiredMap.insert(LDBX, true); isTrapMap.insert(LDBX, false);
    opCodeMap.insert(LDWA, 192); isUnaryMap.insert(LDWA, false); addrModeRequiredMap.insert(LDWA, true); isTrapMap.insert(LDWA, false);
    opCodeMap.insert(LDWX, 200); isUnaryMap.insert(LDWX, false); addrModeRequiredMap.insert(LDWX, true); isTrapMap.insert(LDWX, false);

    opCodeMap.insert(MOVAFLG, 5); isUnaryMap.insert(MOVAFLG, true); addrModeRequiredMap.insert(MOVAFLG, true); isTrapMap.insert(MOVAFLG, false);
    opCodeMap.insert(MOVFLGA, 4); isUnaryMap.insert(MOVFLGA, true); addrModeRequiredMap.insert(MOVFLGA, true); isTrapMap.insert(MOVFLGA, false);
    opCodeMap.insert(MOVSPA, 3); isUnaryMap.insert(MOVSPA, true); addrModeRequiredMap.insert(MOVSPA, true); isTrapMap.insert(MOVSPA, false);

    opCodeMap.insert(NEGA, 8); isUnaryMap.insert(NEGA, true); addrModeRequiredMap.insert(NEGA, true); isTrapMap.insert(NEGA, false);
    opCodeMap.insert(NEGX, 9); isUnaryMap.insert(NEGX, true); addrModeRequiredMap.insert(NEGX, true); isTrapMap.insert(NEGX, false);
    opCodeMap.insert(NOP, 40); isUnaryMap.insert(NOP, false); addrModeRequiredMap.insert(NOP, true); isTrapMap.insert(NOP, true);
    opCodeMap.insert(NOP0, 38); isUnaryMap.insert(NOP0, true); addrModeRequiredMap.insert(NOP0, true); isTrapMap.insert(NOP0, true);
    opCodeMap.insert(NOP1, 39); isUnaryMap.insert(NOP1, true); addrModeRequiredMap.insert(NOP1, true); isTrapMap.insert(NOP1, true);
    opCodeMap.insert(NOTA, 6); isUnaryMap.insert(NOTA, true); addrModeRequiredMap.insert(NOTA, true); isTrapMap.insert(NOTA, false);
    opCodeMap.insert(NOTX, 7); isUnaryMap.insert(NOTX, true); addrModeRequiredMap.insert(NOTX, true); isTrapMap.insert(NOTX, false);

    opCodeMap.insert(ORA, 144); isUnaryMap.insert(ORA, false); addrModeRequiredMap.insert(ORA, true); isTrapMap.insert(ORA, false);
    opCodeMap.insert(ORX, 152); isUnaryMap.insert(ORX, false); addrModeRequiredMap.insert(ORX, true); isTrapMap.insert(ORX, false);

    opCodeMap.insert(RET, 1); isUnaryMap.insert(RET, true); addrModeRequiredMap.insert(RET, true); isTrapMap.insert(RET, false);
    opCodeMap.insert(RETTR, 2); isUnaryMap.insert(RETTR, true); addrModeRequiredMap.insert(RETTR, true); isTrapMap.insert(RETTR, false);
    opCodeMap.insert(ROLA, 14); isUnaryMap.insert(ROLA, true); addrModeRequiredMap.insert(ROLA, true); isTrapMap.insert(ROLA, false);
    opCodeMap.insert(ROLX, 15); isUnaryMap.insert(ROLX, true); addrModeRequiredMap.insert(ROLX, true); isTrapMap.insert(ROLX, false);
    opCodeMap.insert(RORA, 16); isUnaryMap.insert(RORA, true); addrModeRequiredMap.insert(RORA, true); isTrapMap.insert(RORA, false);
    opCodeMap.insert(RORX, 17); isUnaryMap.insert(RORX, true); addrModeRequiredMap.insert(RORX, true); isTrapMap.insert(RORX, false);

    opCodeMap.insert(STBA, 240); isUnaryMap.insert(STBA, false); addrModeRequiredMap.insert(STBA, true); isTrapMap.insert(STBA, false);
    opCodeMap.insert(STBX, 248); isUnaryMap.insert(STBX, false); addrModeRequiredMap.insert(STBX, true); isTrapMap.insert(STBX, false);
    opCodeMap.insert(STWA, 224); isUnaryMap.insert(STWA, false); addrModeRequiredMap.insert(STWA, true); isTrapMap.insert(STWA, false);
    opCodeMap.insert(STWX, 232); isUnaryMap.insert(STWX, false); addrModeRequiredMap.insert(STWX, true); isTrapMap.insert(STWX, false);
    opCodeMap.insert(STOP, 0); isUnaryMap.insert(STOP, true); addrModeRequiredMap.insert(STOP, true); isTrapMap.insert(STOP, false);
    opCodeMap.insert(STRO, 72); isUnaryMap.insert(STRO, false); addrModeRequiredMap.insert(STRO, true); isTrapMap.insert(STRO, true);
    opCodeMap.insert(SUBA, 112); isUnaryMap.insert(SUBA, false); addrModeRequiredMap.insert(SUBA, true); isTrapMap.insert(SUBA, false);
    opCodeMap.insert(SUBSP, 88); isUnaryMap.insert(SUBSP, false); addrModeRequiredMap.insert(SUBSP, true); isTrapMap.insert(SUBSP, false);
    opCodeMap.insert(SUBX, 120); isUnaryMap.insert(SUBX, false); addrModeRequiredMap.insert(SUBX, true); isTrapMap.insert(SUBX, false);
}

// Map to specify legal addressing modes for each instruction
QMap<Enu::EMnemonic, int > Pep::addrModesMap;
void Pep::initAddrModesMap()
{
    // Nonunary instructions
    addrModesMap.insert(ADDA, ALL);
    addrModesMap.insert(ADDSP, ALL);
    addrModesMap.insert(ADDX, ALL);
    addrModesMap.insert(ANDA, ALL);
    addrModesMap.insert(ANDX, ALL);
    addrModesMap.insert(BR, I | X);
    addrModesMap.insert(BRC, I | X);
    addrModesMap.insert(BREQ, I | X);
    addrModesMap.insert(BRGE, I | X);
    addrModesMap.insert(BRGT, I | X);
    addrModesMap.insert(BRLE, I | X);
    addrModesMap.insert(BRLT, I | X);
    addrModesMap.insert(BRNE, I | X);
    addrModesMap.insert(BRV, I | X);
    addrModesMap.insert(CALL, I | X);
    addrModesMap.insert(CHARI, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(CHARO, ALL);
    addrModesMap.insert(CPA, ALL);
    addrModesMap.insert(CPX, ALL);
    addrModesMap.insert(LDA, ALL);
    addrModesMap.insert(LDBYTEA, ALL);
    addrModesMap.insert(LDBYTEX, ALL);
    addrModesMap.insert(LDX, ALL);
    addrModesMap.insert(ORA, ALL);
    addrModesMap.insert(ORX, ALL);
    addrModesMap.insert(STA, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(STBYTEA, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(STBYTEX, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(STX, D | N | S | SF | X | SX | SXF);
    addrModesMap.insert(SUBA, ALL);
    addrModesMap.insert(SUBSP, ALL);
    addrModesMap.insert(SUBX, ALL);
    // Nonunary trap instructions
    int addrMode;
    addrMode = 0;
    if (defaultMnemon0i) addrMode |= I;
    if (defaultMnemon0d) addrMode |= D;
    if (defaultMnemon0n) addrMode |= N;
    if (defaultMnemon0s) addrMode |= S;
    if (defaultMnemon0sf) addrMode |= SF;
    if (defaultMnemon0x) addrMode |= X;
    if (defaultMnemon0sx) addrMode |= SX;
    if (defaultMnemon0sxf) addrMode |= SXF;
    addrModesMap.insert(NOP, addrMode);
    addrMode = 0;
    if (defaultMnemon1i) addrMode |= I;
    if (defaultMnemon1d) addrMode |= D;
    if (defaultMnemon1n) addrMode |= N;
    if (defaultMnemon1s) addrMode |= S;
    if (defaultMnemon1sf) addrMode |= SF;
    if (defaultMnemon1x) addrMode |= X;
    if (defaultMnemon1sx) addrMode |= SX;
    if (defaultMnemon1sxf) addrMode |= SXF;
    addrModesMap.insert(DECI, addrMode);
    addrMode = 0;
    if (defaultMnemon2i) addrMode |= I;
    if (defaultMnemon2d) addrMode |= D;
    if (defaultMnemon2n) addrMode |= N;
    if (defaultMnemon2s) addrMode |= S;
    if (defaultMnemon2sf) addrMode |= SF;
    if (defaultMnemon2x) addrMode |= X;
    if (defaultMnemon2sx) addrMode |= SX;
    if (defaultMnemon2sxf) addrMode |= SXF;
    addrModesMap.insert(DECO, addrMode);
    addrMode = 0;
    if (defaultMnemon3i) addrMode |= I;
    if (defaultMnemon3d) addrMode |= D;
    if (defaultMnemon3n) addrMode |= N;
    if (defaultMnemon3s) addrMode |= S;
    if (defaultMnemon3sf) addrMode |= SF;
    if (defaultMnemon3x) addrMode |= X;
    if (defaultMnemon3sx) addrMode |= SX;
    if (defaultMnemon3sxf) addrMode |= SXF;
    addrModesMap.insert(STRO, addrMode);
}

// The symbol table
QMap<QString, int> Pep::symbolTable;
QMap<QString, bool> Pep::adjustSymbolValueForBurn;

// The trace tag tables
QMap<QString, Enu::ESymbolFormat> Pep::symbolFormat;
QMap<QString, int> Pep::symbolFormatMultiplier;
QMap<QString, QStringList> Pep::globalStructSymbols;

QMap<int, QStringList> Pep::symbolTraceList; // Key is memory address
QStringList Pep::blockSymbols;
QStringList Pep::equateSymbols;

// Map from instruction memory address to assembler listing line
QMap<int, int> *Pep::memAddrssToAssemblerListing;
QMap<int, Qt::CheckState> *Pep::listingRowChecked;

QMap<int, int> Pep::memAddrssToAssemblerListingProg;
QMap<int, Qt::CheckState> Pep::listingRowCheckedProg;

QMap<int, int> Pep::memAddrssToAssemblerListingOS;
QMap<int, Qt::CheckState> Pep::listingRowCheckedOS;

// Decoder tables
QVector<Enu::EMnemonic> Pep::decodeMnemonic(256);
QVector<Enu::EAddrMode> Pep::decodeAddrMode(256);
void Pep::initDecoderTables()
{
    decodeMnemonic[0] = STOP; decodeAddrMode[0] = NONE;
    decodeMnemonic[1] = RETTR; decodeAddrMode[1] = NONE;
    decodeMnemonic[2] = MOVSPA; decodeAddrMode[2] = NONE;
    decodeMnemonic[3] = MOVFLGA; decodeAddrMode[3] = NONE;

    decodeMnemonic[4] = BR; decodeAddrMode[4] = I;
    decodeMnemonic[5] = BR; decodeAddrMode[5] = X;
    decodeMnemonic[6] = BRLE; decodeAddrMode[6] = I;
    decodeMnemonic[7] = BRLE; decodeAddrMode[7] = X;
    decodeMnemonic[8] = BRLT; decodeAddrMode[8] = I;
    decodeMnemonic[9] = BRLT; decodeAddrMode[9] = X;
    decodeMnemonic[10] = BREQ; decodeAddrMode[10] = I;
    decodeMnemonic[11] = BREQ; decodeAddrMode[11] = X;
    decodeMnemonic[12] = BRNE; decodeAddrMode[12] = I;
    decodeMnemonic[13] = BRNE; decodeAddrMode[13] = X;
    decodeMnemonic[14] = BRGE; decodeAddrMode[14] = I;
    decodeMnemonic[15] = BRGE; decodeAddrMode[15] = X;
    decodeMnemonic[16] = BRGT; decodeAddrMode[16] = I;
    decodeMnemonic[17] = BRGT; decodeAddrMode[17] = X;
    decodeMnemonic[18] = BRV; decodeAddrMode[18] = I;
    decodeMnemonic[19] = BRV; decodeAddrMode[19] = X;
    decodeMnemonic[20] = BRC; decodeAddrMode[20] = I;
    decodeMnemonic[21] = BRC; decodeAddrMode[21] = X;
    decodeMnemonic[22] = CALL; decodeAddrMode[22] = I;
    decodeMnemonic[23] = CALL; decodeAddrMode[23] = X;

    decodeMnemonic[24] = NOTA; decodeAddrMode[24] = NONE;
    decodeMnemonic[25] = NOTX; decodeAddrMode[25] = NONE;
    decodeMnemonic[26] = NEGA; decodeAddrMode[26] = NONE;
    decodeMnemonic[27] = NEGX; decodeAddrMode[27] = NONE;
    decodeMnemonic[28] = ASLA; decodeAddrMode[28] = NONE;
    decodeMnemonic[29] = ASLX; decodeAddrMode[29] = NONE;
    decodeMnemonic[30] = ASRA; decodeAddrMode[30] = NONE;
    decodeMnemonic[31] = ASRX; decodeAddrMode[31] = NONE;
    decodeMnemonic[32] = ROLA; decodeAddrMode[32] = NONE;
    decodeMnemonic[33] = ROLX; decodeAddrMode[33] = NONE;
    decodeMnemonic[34] = RORA; decodeAddrMode[34] = NONE;
    decodeMnemonic[35] = RORX; decodeAddrMode[35] = NONE;

    // Note that the trap instructions are all unary at the machine level
    decodeMnemonic[36] = NOP0; decodeAddrMode[36] = NONE;
    decodeMnemonic[37] = NOP1; decodeAddrMode[37] = NONE;
    decodeMnemonic[38] = NOP2; decodeAddrMode[38] = NONE;
    decodeMnemonic[39] = NOP3; decodeAddrMode[39] = NONE;

    decodeMnemonic[40] = NOP; decodeAddrMode[40] = NONE;
    decodeMnemonic[41] = NOP; decodeAddrMode[41] = NONE;
    decodeMnemonic[42] = NOP; decodeAddrMode[42] = NONE;
    decodeMnemonic[43] = NOP; decodeAddrMode[43] = NONE;
    decodeMnemonic[44] = NOP; decodeAddrMode[44] = NONE;
    decodeMnemonic[45] = NOP; decodeAddrMode[45] = NONE;
    decodeMnemonic[46] = NOP; decodeAddrMode[46] = NONE;
    decodeMnemonic[47] = NOP; decodeAddrMode[47] = NONE;

    decodeMnemonic[48] = DECI; decodeAddrMode[48] = NONE;
    decodeMnemonic[49] = DECI; decodeAddrMode[49] = NONE;
    decodeMnemonic[50] = DECI; decodeAddrMode[50] = NONE;
    decodeMnemonic[51] = DECI; decodeAddrMode[51] = NONE;
    decodeMnemonic[52] = DECI; decodeAddrMode[52] = NONE;
    decodeMnemonic[53] = DECI; decodeAddrMode[53] = NONE;
    decodeMnemonic[54] = DECI; decodeAddrMode[54] = NONE;
    decodeMnemonic[55] = DECI; decodeAddrMode[55] = NONE;

    decodeMnemonic[56] = DECO; decodeAddrMode[56] = NONE; // I think this is a bug...?
    decodeMnemonic[57] = DECO; decodeAddrMode[57] = NONE;
    decodeMnemonic[58] = DECO; decodeAddrMode[58] = NONE;
    decodeMnemonic[59] = DECO; decodeAddrMode[59] = NONE;
    decodeMnemonic[60] = DECO; decodeAddrMode[60] = NONE;
    decodeMnemonic[61] = DECO; decodeAddrMode[61] = NONE;
    decodeMnemonic[62] = DECO; decodeAddrMode[62] = NONE;
    decodeMnemonic[63] = DECO; decodeAddrMode[63] = NONE;

    decodeMnemonic[64] = STRO; decodeAddrMode[64] = NONE;
    decodeMnemonic[65] = STRO; decodeAddrMode[65] = NONE;
    decodeMnemonic[66] = STRO; decodeAddrMode[66] = NONE;
    decodeMnemonic[67] = STRO; decodeAddrMode[67] = NONE;
    decodeMnemonic[68] = STRO; decodeAddrMode[68] = NONE;
    decodeMnemonic[69] = STRO; decodeAddrMode[69] = NONE;
    decodeMnemonic[70] = STRO; decodeAddrMode[70] = NONE;
    decodeMnemonic[71] = STRO; decodeAddrMode[71] = NONE;

    decodeMnemonic[72] = CHARI; decodeAddrMode[72] = I;
    decodeMnemonic[73] = CHARI; decodeAddrMode[73] = D;
    decodeMnemonic[74] = CHARI; decodeAddrMode[74] = N;
    decodeMnemonic[75] = CHARI; decodeAddrMode[75] = S;
    decodeMnemonic[76] = CHARI; decodeAddrMode[76] = SF;
    decodeMnemonic[77] = CHARI; decodeAddrMode[77] = X;
    decodeMnemonic[78] = CHARI; decodeAddrMode[78] = SX;
    decodeMnemonic[79] = CHARI; decodeAddrMode[79] = SXF;

    decodeMnemonic[80] = CHARO; decodeAddrMode[80] = I;
    decodeMnemonic[81] = CHARO; decodeAddrMode[81] = D;
    decodeMnemonic[82] = CHARO; decodeAddrMode[82] = N;
    decodeMnemonic[83] = CHARO; decodeAddrMode[83] = S;
    decodeMnemonic[84] = CHARO; decodeAddrMode[84] = SF;
    decodeMnemonic[85] = CHARO; decodeAddrMode[85] = X;
    decodeMnemonic[86] = CHARO; decodeAddrMode[86] = SX;
    decodeMnemonic[87] = CHARO; decodeAddrMode[87] = SXF;

    decodeMnemonic[88] = RET0; decodeAddrMode[88] = NONE;
    decodeMnemonic[89] = RET1; decodeAddrMode[89] = NONE;
    decodeMnemonic[90] = RET2; decodeAddrMode[90] = NONE;
    decodeMnemonic[91] = RET3; decodeAddrMode[91] = NONE;
    decodeMnemonic[92] = RET4; decodeAddrMode[92] = NONE;
    decodeMnemonic[93] = RET5; decodeAddrMode[93] = NONE;
    decodeMnemonic[94] = RET6; decodeAddrMode[94] = NONE;
    decodeMnemonic[95] = RET7; decodeAddrMode[95] = NONE;

    decodeMnemonic[96] = ADDSP; decodeAddrMode[96] = I;
    decodeMnemonic[97] = ADDSP; decodeAddrMode[97] = D;
    decodeMnemonic[98] = ADDSP; decodeAddrMode[98] = N;
    decodeMnemonic[99] = ADDSP; decodeAddrMode[99] = S;
    decodeMnemonic[100] = ADDSP; decodeAddrMode[100] = SF;
    decodeMnemonic[101] = ADDSP; decodeAddrMode[101] = X;
    decodeMnemonic[102] = ADDSP; decodeAddrMode[102] = SX;
    decodeMnemonic[103] = ADDSP; decodeAddrMode[103] = SXF;

    decodeMnemonic[104] = SUBSP; decodeAddrMode[104] = I;
    decodeMnemonic[105] = SUBSP; decodeAddrMode[105] = D;
    decodeMnemonic[106] = SUBSP; decodeAddrMode[106] = N;
    decodeMnemonic[107] = SUBSP; decodeAddrMode[107] = S;
    decodeMnemonic[108] = SUBSP; decodeAddrMode[108] = SF;
    decodeMnemonic[109] = SUBSP; decodeAddrMode[109] = X;
    decodeMnemonic[110] = SUBSP; decodeAddrMode[110] = SX;
    decodeMnemonic[111] = SUBSP; decodeAddrMode[111] = SXF;

    decodeMnemonic[112] = ADDA; decodeAddrMode[112] = I;
    decodeMnemonic[113] = ADDA; decodeAddrMode[113] = D;
    decodeMnemonic[114] = ADDA; decodeAddrMode[114] = N;
    decodeMnemonic[115] = ADDA; decodeAddrMode[115] = S;
    decodeMnemonic[116] = ADDA; decodeAddrMode[116] = SF;
    decodeMnemonic[117] = ADDA; decodeAddrMode[117] = X;
    decodeMnemonic[118] = ADDA; decodeAddrMode[118] = SX;
    decodeMnemonic[119] = ADDA; decodeAddrMode[119] = SXF;

    decodeMnemonic[120] = ADDX; decodeAddrMode[120] = I;
    decodeMnemonic[121] = ADDX; decodeAddrMode[121] = D;
    decodeMnemonic[122] = ADDX; decodeAddrMode[122] = N;
    decodeMnemonic[123] = ADDX; decodeAddrMode[123] = S;
    decodeMnemonic[124] = ADDX; decodeAddrMode[124] = SF;
    decodeMnemonic[125] = ADDX; decodeAddrMode[125] = X;
    decodeMnemonic[126] = ADDX; decodeAddrMode[126] = SX;
    decodeMnemonic[127] = ADDX; decodeAddrMode[127] = SXF;

    decodeMnemonic[128] = SUBA; decodeAddrMode[128] = I;
    decodeMnemonic[129] = SUBA; decodeAddrMode[129] = D;
    decodeMnemonic[130] = SUBA; decodeAddrMode[130] = N;
    decodeMnemonic[131] = SUBA; decodeAddrMode[131] = S;
    decodeMnemonic[132] = SUBA; decodeAddrMode[132] = SF;
    decodeMnemonic[133] = SUBA; decodeAddrMode[133] = X;
    decodeMnemonic[134] = SUBA; decodeAddrMode[134] = SX;
    decodeMnemonic[135] = SUBA; decodeAddrMode[135] = SXF;

    decodeMnemonic[136] = SUBX; decodeAddrMode[136] = I;
    decodeMnemonic[137] = SUBX; decodeAddrMode[137] = D;
    decodeMnemonic[138] = SUBX; decodeAddrMode[138] = N;
    decodeMnemonic[139] = SUBX; decodeAddrMode[139] = S;
    decodeMnemonic[140] = SUBX; decodeAddrMode[140] = SF;
    decodeMnemonic[141] = SUBX; decodeAddrMode[141] = X;
    decodeMnemonic[142] = SUBX; decodeAddrMode[142] = SX;
    decodeMnemonic[143] = SUBX; decodeAddrMode[143] = SXF;

    decodeMnemonic[144] = ANDA; decodeAddrMode[144] = I;
    decodeMnemonic[145] = ANDA; decodeAddrMode[145] = D;
    decodeMnemonic[146] = ANDA; decodeAddrMode[146] = N;
    decodeMnemonic[147] = ANDA; decodeAddrMode[147] = S;
    decodeMnemonic[148] = ANDA; decodeAddrMode[148] = SF;
    decodeMnemonic[149] = ANDA; decodeAddrMode[149] = X;
    decodeMnemonic[150] = ANDA; decodeAddrMode[150] = SX;
    decodeMnemonic[151] = ANDA; decodeAddrMode[151] = SXF;

    decodeMnemonic[152] = ANDX; decodeAddrMode[152] = I;
    decodeMnemonic[153] = ANDX; decodeAddrMode[153] = D;
    decodeMnemonic[154] = ANDX; decodeAddrMode[154] = N;
    decodeMnemonic[155] = ANDX; decodeAddrMode[155] = S;
    decodeMnemonic[156] = ANDX; decodeAddrMode[156] = SF;
    decodeMnemonic[157] = ANDX; decodeAddrMode[157] = X;
    decodeMnemonic[158] = ANDX; decodeAddrMode[158] = SX;
    decodeMnemonic[159] = ANDX; decodeAddrMode[159] = SXF;

    decodeMnemonic[160] = ORA; decodeAddrMode[160] = I;
    decodeMnemonic[161] = ORA; decodeAddrMode[161] = D;
    decodeMnemonic[162] = ORA; decodeAddrMode[162] = N;
    decodeMnemonic[163] = ORA; decodeAddrMode[163] = S;
    decodeMnemonic[164] = ORA; decodeAddrMode[164] = SF;
    decodeMnemonic[165] = ORA; decodeAddrMode[165] = X;
    decodeMnemonic[166] = ORA; decodeAddrMode[166] = SX;
    decodeMnemonic[167] = ORA; decodeAddrMode[167] = SXF;

    decodeMnemonic[168] = ORX; decodeAddrMode[168] = I;
    decodeMnemonic[169] = ORX; decodeAddrMode[169] = D;
    decodeMnemonic[170] = ORX; decodeAddrMode[170] = N;
    decodeMnemonic[171] = ORX; decodeAddrMode[171] = S;
    decodeMnemonic[172] = ORX; decodeAddrMode[172] = SF;
    decodeMnemonic[173] = ORX; decodeAddrMode[173] = X;
    decodeMnemonic[174] = ORX; decodeAddrMode[174] = SX;
    decodeMnemonic[175] = ORX; decodeAddrMode[175] = SXF;

    decodeMnemonic[176] = CPA; decodeAddrMode[176] = I;
    decodeMnemonic[177] = CPA; decodeAddrMode[177] = D;
    decodeMnemonic[178] = CPA; decodeAddrMode[178] = N;
    decodeMnemonic[179] = CPA; decodeAddrMode[179] = S;
    decodeMnemonic[180] = CPA; decodeAddrMode[180] = SF;
    decodeMnemonic[181] = CPA; decodeAddrMode[181] = X;
    decodeMnemonic[182] = CPA; decodeAddrMode[182] = SX;
    decodeMnemonic[183] = CPA; decodeAddrMode[183] = SXF;

    decodeMnemonic[184] = CPX; decodeAddrMode[184] = I;
    decodeMnemonic[185] = CPX; decodeAddrMode[185] = D;
    decodeMnemonic[186] = CPX; decodeAddrMode[186] = N;
    decodeMnemonic[187] = CPX; decodeAddrMode[187] = S;
    decodeMnemonic[188] = CPX; decodeAddrMode[188] = SF;
    decodeMnemonic[189] = CPX; decodeAddrMode[189] = X;
    decodeMnemonic[190] = CPX; decodeAddrMode[190] = SX;
    decodeMnemonic[191] = CPX; decodeAddrMode[191] = SXF;

    decodeMnemonic[192] = LDA; decodeAddrMode[192] = I;
    decodeMnemonic[193] = LDA; decodeAddrMode[193] = D;
    decodeMnemonic[194] = LDA; decodeAddrMode[194] = N;
    decodeMnemonic[195] = LDA; decodeAddrMode[195] = S;
    decodeMnemonic[196] = LDA; decodeAddrMode[196] = SF;
    decodeMnemonic[197] = LDA; decodeAddrMode[197] = X;
    decodeMnemonic[198] = LDA; decodeAddrMode[198] = SX;
    decodeMnemonic[199] = LDA; decodeAddrMode[199] = SXF;

    decodeMnemonic[200] = LDX; decodeAddrMode[200] = I;
    decodeMnemonic[201] = LDX; decodeAddrMode[201] = D;
    decodeMnemonic[202] = LDX; decodeAddrMode[202] = N;
    decodeMnemonic[203] = LDX; decodeAddrMode[203] = S;
    decodeMnemonic[204] = LDX; decodeAddrMode[204] = SF;
    decodeMnemonic[205] = LDX; decodeAddrMode[205] = X;
    decodeMnemonic[206] = LDX; decodeAddrMode[206] = SX;
    decodeMnemonic[207] = LDX; decodeAddrMode[207] = SXF;

    decodeMnemonic[208] = LDBYTEA; decodeAddrMode[208] = I;
    decodeMnemonic[209] = LDBYTEA; decodeAddrMode[209] = D;
    decodeMnemonic[210] = LDBYTEA; decodeAddrMode[210] = N;
    decodeMnemonic[211] = LDBYTEA; decodeAddrMode[211] = S;
    decodeMnemonic[212] = LDBYTEA; decodeAddrMode[212] = SF;
    decodeMnemonic[213] = LDBYTEA; decodeAddrMode[213] = X;
    decodeMnemonic[214] = LDBYTEA; decodeAddrMode[214] = SX;
    decodeMnemonic[215] = LDBYTEA; decodeAddrMode[215] = SXF;

    decodeMnemonic[216] = LDBYTEX; decodeAddrMode[216] = I;
    decodeMnemonic[217] = LDBYTEX; decodeAddrMode[217] = D;
    decodeMnemonic[218] = LDBYTEX; decodeAddrMode[218] = N;
    decodeMnemonic[219] = LDBYTEX; decodeAddrMode[219] = S;
    decodeMnemonic[220] = LDBYTEX; decodeAddrMode[220] = SF;
    decodeMnemonic[221] = LDBYTEX; decodeAddrMode[221] = X;
    decodeMnemonic[222] = LDBYTEX; decodeAddrMode[222] = SX;
    decodeMnemonic[223] = LDBYTEX; decodeAddrMode[223] = SXF;

    decodeMnemonic[224] = STA; decodeAddrMode[224] = I;
    decodeMnemonic[225] = STA; decodeAddrMode[225] = D;
    decodeMnemonic[226] = STA; decodeAddrMode[226] = N;
    decodeMnemonic[227] = STA; decodeAddrMode[227] = S;
    decodeMnemonic[228] = STA; decodeAddrMode[228] = SF;
    decodeMnemonic[229] = STA; decodeAddrMode[229] = X;
    decodeMnemonic[230] = STA; decodeAddrMode[230] = SX;
    decodeMnemonic[231] = STA; decodeAddrMode[231] = SXF;

    decodeMnemonic[232] = STX; decodeAddrMode[232] = I;
    decodeMnemonic[233] = STX; decodeAddrMode[233] = D;
    decodeMnemonic[234] = STX; decodeAddrMode[234] = N;
    decodeMnemonic[235] = STX; decodeAddrMode[235] = S;
    decodeMnemonic[236] = STX; decodeAddrMode[236] = SF;
    decodeMnemonic[237] = STX; decodeAddrMode[237] = X;
    decodeMnemonic[238] = STX; decodeAddrMode[238] = SX;
    decodeMnemonic[239] = STX; decodeAddrMode[239] = SXF;

    decodeMnemonic[240] = STBYTEA; decodeAddrMode[240] = I;
    decodeMnemonic[241] = STBYTEA; decodeAddrMode[241] = D;
    decodeMnemonic[242] = STBYTEA; decodeAddrMode[242] = N;
    decodeMnemonic[243] = STBYTEA; decodeAddrMode[243] = S;
    decodeMnemonic[244] = STBYTEA; decodeAddrMode[244] = SF;
    decodeMnemonic[245] = STBYTEA; decodeAddrMode[245] = X;
    decodeMnemonic[246] = STBYTEA; decodeAddrMode[246] = SX;
    decodeMnemonic[247] = STBYTEA; decodeAddrMode[247] = SXF;

    decodeMnemonic[248] = STBYTEX; decodeAddrMode[248] = I;
    decodeMnemonic[249] = STBYTEX; decodeAddrMode[249] = D;
    decodeMnemonic[250] = STBYTEX; decodeAddrMode[250] = N;
    decodeMnemonic[251] = STBYTEX; decodeAddrMode[251] = S;
    decodeMnemonic[252] = STBYTEX; decodeAddrMode[252] = SF;
    decodeMnemonic[253] = STBYTEX; decodeAddrMode[253] = X;
    decodeMnemonic[254] = STBYTEX; decodeAddrMode[254] = SX;
    decodeMnemonic[255] = STBYTEX; decodeAddrMode[255] = SXF;
}

// .BURN and the ROM state
int Pep::byteCount;
int Pep::burnCount;
int Pep::dotBurnArgument;
int Pep::romStartAddress;

// Memory trace state
bool Pep::traceTagWarning;

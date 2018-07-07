// File: redefinemnemonicsdialog.cpp
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
#include <QRegExpValidator>
#include "redefinemnemonicsdialog.h"
#include "ui_redefinemnemonicsdialog.h"

using namespace Enu;

RedefineMnemonicsDialog::RedefineMnemonicsDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::RedefineMnemonicsDialog)
{
    ui->setupUi(this);
    restoreDefaults();

    ui->unaryOpCodeLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->unaryOpCode1Label->setFont(QFont(Pep::codeFont));
    ui->unaryOpCode2Label->setFont(QFont(Pep::codeFont));

    ui->unaryMnemonicLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->unaryMnemonic0LineEdit->setFont(QFont(Pep::codeFont));
    ui->unaryMnemonic1LineEdit->setFont(QFont(Pep::codeFont));

    ui->nonunaryMnemonicLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->iLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->dLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->nLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->sLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->sfLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->xLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->sxLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));
    ui->sfxLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));

    ui->nonunaryOpCodeLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize, QFont::Bold));

    ui->nonunaryOpCode1Label->setFont(QFont(Pep::codeFont));
    ui->nonunaryOpCode2Label->setFont(QFont(Pep::codeFont));
    ui->nonunaryOpCode3Label->setFont(QFont(Pep::codeFont));
    ui->nonunaryOpCode4Label->setFont(QFont(Pep::codeFont));
    ui->nonunaryOpCode5Label->setFont(QFont(Pep::codeFont));

    ui->nonUnaryMnemonic0LineEdit->setFont(QFont(Pep::codeFont));
    ui->nonUnaryMnemonic1LineEdit->setFont(QFont(Pep::codeFont));
    ui->nonUnaryMnemonic2LineEdit->setFont(QFont(Pep::codeFont));
    ui->nonUnaryMnemonic3LineEdit->setFont(QFont(Pep::codeFont));
    ui->nonUnaryMnemonic4LineEdit->setFont(QFont(Pep::codeFont));

    connect(ui->defaultMnemonicsButton, SIGNAL(clicked()), this, SLOT(restoreDefaults()));

    QRegExp rx("^[A-Za-z][A-Za-z0-9]{0,7}");
    QValidator *validator = new QRegExpValidator(rx, this);

    ui->nonUnaryMnemonic0LineEdit->setValidator(validator);
    ui->nonUnaryMnemonic1LineEdit->setValidator(validator);
    ui->nonUnaryMnemonic2LineEdit->setValidator(validator);
    ui->nonUnaryMnemonic3LineEdit->setValidator(validator);
    ui->nonUnaryMnemonic4LineEdit->setValidator(validator);

    ui->unaryMnemonic0LineEdit->setValidator(validator);
    ui->unaryMnemonic1LineEdit->setValidator(validator);

    connect(ui->nonUnaryMnemonic0LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic0(QString)));
    connect(ui->nonUnaryMnemonic1LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic1(QString)));
    connect(ui->nonUnaryMnemonic2LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic2(QString)));
    connect(ui->nonUnaryMnemonic3LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic3(QString)));
    connect(ui->nonUnaryMnemonic4LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineNonUnaryMnemonic3(QString)));

    connect(ui->unaryMnemonic0LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineUnaryMnemonic0(QString)));
    connect(ui->unaryMnemonic1LineEdit, SIGNAL(textEdited(QString)), this, SLOT(redefineUnaryMnemonic1(QString)));

    connect(ui->mnemon0iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon0sfxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(ui->mnemon1iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon1sfxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(ui->mnemon2iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon2sfxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(ui->mnemon3iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon3sfxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));

    connect(ui->mnemon4iCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4dCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4nCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4sCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4sfCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4xCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4sxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
    connect(ui->mnemon4sfxCheckBox, SIGNAL(clicked()), this, SLOT(setNonUnaryAllowedModes()));
}

RedefineMnemonicsDialog::~RedefineMnemonicsDialog()
{
    delete ui;
}

void RedefineMnemonicsDialog::restoreDefaults()
{
    ui->unaryMnemonic0LineEdit->setText(Pep::defaultUnaryMnemonic0);
    ui->unaryMnemonic1LineEdit->setText(Pep::defaultUnaryMnemonic1);
    ui->nonUnaryMnemonic0LineEdit->setText(Pep::defaultNonUnaryMnemonic0);
    ui->mnemon0iCheckBox->setChecked(Pep::defaultMnemon0i);
    ui->mnemon0dCheckBox->setChecked(Pep::defaultMnemon0d);
    ui->mnemon0nCheckBox->setChecked(Pep::defaultMnemon0n);
    ui->mnemon0sCheckBox->setChecked(Pep::defaultMnemon0s);
    ui->mnemon0sfCheckBox->setChecked(Pep::defaultMnemon0sf);
    ui->mnemon0xCheckBox->setChecked(Pep::defaultMnemon0x);
    ui->mnemon0sxCheckBox->setChecked(Pep::defaultMnemon0sx);
    ui->mnemon0sfxCheckBox->setChecked(Pep::defaultMnemon0sfx);
    ui->nonUnaryMnemonic1LineEdit->setText(Pep::defaultNonUnaryMnemonic1);
    ui->mnemon1iCheckBox->setChecked(Pep::defaultMnemon1i);
    ui->mnemon1dCheckBox->setChecked(Pep::defaultMnemon1d);
    ui->mnemon1nCheckBox->setChecked(Pep::defaultMnemon1n);
    ui->mnemon1sCheckBox->setChecked(Pep::defaultMnemon1s);
    ui->mnemon1sfCheckBox->setChecked(Pep::defaultMnemon1sf);
    ui->mnemon1xCheckBox->setChecked(Pep::defaultMnemon1x);
    ui->mnemon1sxCheckBox->setChecked(Pep::defaultMnemon1sx);
    ui->mnemon1sfxCheckBox->setChecked(Pep::defaultMnemon1sfx);
    ui->nonUnaryMnemonic2LineEdit->setText(Pep::defaultNonUnaryMnemonic2);
    ui->mnemon2iCheckBox->setChecked(Pep::defaultMnemon2i);
    ui->mnemon2dCheckBox->setChecked(Pep::defaultMnemon2d);
    ui->mnemon2nCheckBox->setChecked(Pep::defaultMnemon2n);
    ui->mnemon2sCheckBox->setChecked(Pep::defaultMnemon2s);
    ui->mnemon2sfCheckBox->setChecked(Pep::defaultMnemon2sf);
    ui->mnemon2xCheckBox->setChecked(Pep::defaultMnemon2x);
    ui->mnemon2sxCheckBox->setChecked(Pep::defaultMnemon2sx);
    ui->mnemon2sfxCheckBox->setChecked(Pep::defaultMnemon2sfx);
    ui->nonUnaryMnemonic3LineEdit->setText(Pep::defaultNonUnaryMnemonic3);
    ui->mnemon3iCheckBox->setChecked(Pep::defaultMnemon3i);
    ui->mnemon3dCheckBox->setChecked(Pep::defaultMnemon3d);
    ui->mnemon3nCheckBox->setChecked(Pep::defaultMnemon3n);
    ui->mnemon3sCheckBox->setChecked(Pep::defaultMnemon3s);
    ui->mnemon3sfCheckBox->setChecked(Pep::defaultMnemon3sf);
    ui->mnemon3xCheckBox->setChecked(Pep::defaultMnemon3x);
    ui->mnemon3sxCheckBox->setChecked(Pep::defaultMnemon3sx);
    ui->mnemon3sfxCheckBox->setChecked(Pep::defaultMnemon3sfx);
    ui->nonUnaryMnemonic4LineEdit->setText(Pep::defaultNonUnaryMnemonic4);
    ui->mnemon4iCheckBox->setChecked(Pep::defaultMnemon4i);
    ui->mnemon4dCheckBox->setChecked(Pep::defaultMnemon4d);
    ui->mnemon4nCheckBox->setChecked(Pep::defaultMnemon4n);
    ui->mnemon4sCheckBox->setChecked(Pep::defaultMnemon4s);
    ui->mnemon4sfCheckBox->setChecked(Pep::defaultMnemon4sf);
    ui->mnemon4xCheckBox->setChecked(Pep::defaultMnemon4x);
    ui->mnemon4sxCheckBox->setChecked(Pep::defaultMnemon4sx);
    ui->mnemon4sfxCheckBox->setChecked(Pep::defaultMnemon4sfx);

    int addrMode = 0;
    if (Pep::defaultMnemon0i) addrMode |= I;
    if (Pep::defaultMnemon0d) addrMode |= D;
    if (Pep::defaultMnemon0n) addrMode |= N;
    if (Pep::defaultMnemon0s) addrMode |= S;
    if (Pep::defaultMnemon0sf) addrMode |= SF;
    if (Pep::defaultMnemon0x) addrMode |= X;
    if (Pep::defaultMnemon0sx) addrMode |= SX;
    if (Pep::defaultMnemon0sfx) addrMode |= SFX;
    Pep::addrModesMap.insert(NOP, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon1i) addrMode |= I;
    if (Pep::defaultMnemon1d) addrMode |= D;
    if (Pep::defaultMnemon1n) addrMode |= N;
    if (Pep::defaultMnemon1s) addrMode |= S;
    if (Pep::defaultMnemon1sf) addrMode |= SF;
    if (Pep::defaultMnemon1x) addrMode |= X;
    if (Pep::defaultMnemon1sx) addrMode |= SX;
    if (Pep::defaultMnemon1sfx) addrMode |= SFX;
    Pep::addrModesMap.insert(DECI, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon2i) addrMode |= I;
    if (Pep::defaultMnemon2d) addrMode |= D;
    if (Pep::defaultMnemon2n) addrMode |= N;
    if (Pep::defaultMnemon2s) addrMode |= S;
    if (Pep::defaultMnemon2sf) addrMode |= SF;
    if (Pep::defaultMnemon2x) addrMode |= X;
    if (Pep::defaultMnemon2sx) addrMode |= SX;
    if (Pep::defaultMnemon2sfx) addrMode |= SFX;
    Pep::addrModesMap.insert(DECO, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon3i) addrMode |= I;
    if (Pep::defaultMnemon3d) addrMode |= D;
    if (Pep::defaultMnemon3n) addrMode |= N;
    if (Pep::defaultMnemon3s) addrMode |= S;
    if (Pep::defaultMnemon3sf) addrMode |= SF;
    if (Pep::defaultMnemon3x) addrMode |= X;
    if (Pep::defaultMnemon3sx) addrMode |= SX;
    if (Pep::defaultMnemon3sfx) addrMode |= SFX;
    Pep::addrModesMap.insert(HEXO, addrMode);
    addrMode = 0;
    if (Pep::defaultMnemon4i) addrMode |= I;
    if (Pep::defaultMnemon4d) addrMode |= D;
    if (Pep::defaultMnemon4n) addrMode |= N;
    if (Pep::defaultMnemon4s) addrMode |= S;
    if (Pep::defaultMnemon4sf) addrMode |= SF;
    if (Pep::defaultMnemon4x) addrMode |= X;
    if (Pep::defaultMnemon4sx) addrMode |= SX;
    if (Pep::defaultMnemon4sfx) addrMode |= SFX;
    Pep::addrModesMap.insert(STRO, addrMode);

    Pep::initEnumMnemonMaps();
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic0(QString string)
{    
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 00101.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 00101.");
    }
    else {
        ui->nonUnaryMnemonic0LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP));
        Pep::enumToMnemonMap.insert(Enu::NOP, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic1(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 00110.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 00110.");
    }
    else {
        ui->nonUnaryMnemonic1LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::DECI));
        Pep::enumToMnemonMap.insert(Enu::DECI, string);
        Pep::mnemonToEnumMap.insert(string, Enu::DECI);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic2(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 00111.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 00111.");
    }
    else {
        ui->nonUnaryMnemonic2LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::DECO));
        Pep::enumToMnemonMap.insert(Enu::DECO, string);
        Pep::mnemonToEnumMap.insert(string, Enu::DECO);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic3(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 01000.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 01000.");
    }
    else {
        ui->nonUnaryMnemonic3LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::HEXO));
        Pep::enumToMnemonMap.insert(Enu::STRO, string);
        Pep::mnemonToEnumMap.insert(string, Enu::HEXO);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineNonUnaryMnemonic4(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 01001.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 01001.");
    }
    else {
        ui->nonUnaryMnemonic3LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::STRO));
        Pep::enumToMnemonMap.insert(Enu::STRO, string);
        Pep::mnemonToEnumMap.insert(string, Enu::STRO);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineUnaryMnemonic0(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 0010 0010.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 0010 0010.");
    }
    else {
        ui->unaryMnemonic0LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP0));
        Pep::enumToMnemonMap.insert(Enu::NOP0, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP0);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::redefineUnaryMnemonic1(QString string)
{
    string = string.toUpper();
    if (string.isEmpty()) {
        ui->warningLabel->setText("Empty mnemonic not stored for 0010 0101.");
    }
    else if (Pep::mnemonToEnumMap.contains(string)) {
        ui->warningLabel->setText("Duplicate not stored for 0010 0101.");
    }
    else {
        ui->unaryMnemonic1LineEdit->setText(string);
        Pep::mnemonToEnumMap.remove(Pep::enumToMnemonMap.value(Enu::NOP1));
        Pep::enumToMnemonMap.insert(Enu::NOP1, string);
        Pep::mnemonToEnumMap.insert(string, Enu::NOP1);
        ui->warningLabel->clear();
    }
}

void RedefineMnemonicsDialog::setNonUnaryAllowedModes()
{
    int addrMode = 0;
    if (ui->mnemon0iCheckBox->isChecked()) addrMode |= I;
    if (ui->mnemon0dCheckBox->isChecked()) addrMode |= D;
    if (ui->mnemon0nCheckBox->isChecked()) addrMode |= N;
    if (ui->mnemon0sCheckBox->isChecked()) addrMode |= S;
    if (ui->mnemon0sfCheckBox->isChecked()) addrMode |= SF;
    if (ui->mnemon0xCheckBox->isChecked()) addrMode |= X;
    if (ui->mnemon0sxCheckBox->isChecked()) addrMode |= SX;
    if (ui->mnemon0sfxCheckBox->isChecked()) addrMode |= SFX;
    Pep::addrModesMap.insert(NOP, addrMode);
    addrMode = 0;
    if (ui->mnemon1iCheckBox->isChecked()) addrMode |= I;
    if (ui->mnemon1dCheckBox->isChecked()) addrMode |= D;
    if (ui->mnemon1nCheckBox->isChecked()) addrMode |= N;
    if (ui->mnemon1sCheckBox->isChecked()) addrMode |= S;
    if (ui->mnemon1sfCheckBox->isChecked()) addrMode |= SF;
    if (ui->mnemon1xCheckBox->isChecked()) addrMode |= X;
    if (ui->mnemon1sxCheckBox->isChecked()) addrMode |= SX;
    if (ui->mnemon1sfxCheckBox->isChecked()) addrMode |= SFX;
    Pep::addrModesMap.insert(DECI, addrMode);
    addrMode = 0;
    if (ui->mnemon2iCheckBox->isChecked()) addrMode |= I;
    if (ui->mnemon2dCheckBox->isChecked()) addrMode |= D;
    if (ui->mnemon2nCheckBox->isChecked()) addrMode |= N;
    if (ui->mnemon2sCheckBox->isChecked()) addrMode |= S;
    if (ui->mnemon2sfCheckBox->isChecked()) addrMode |= SF;
    if (ui->mnemon2xCheckBox->isChecked()) addrMode |= X;
    if (ui->mnemon2sxCheckBox->isChecked()) addrMode |= SX;
    if (ui->mnemon2sfxCheckBox->isChecked()) addrMode |= SFX;
    Pep::addrModesMap.insert(DECO, addrMode);
    addrMode = 0;
    if (ui->mnemon3iCheckBox->isChecked()) addrMode |= I;
    if (ui->mnemon3dCheckBox->isChecked()) addrMode |= D;
    if (ui->mnemon3nCheckBox->isChecked()) addrMode |= N;
    if (ui->mnemon3sCheckBox->isChecked()) addrMode |= S;
    if (ui->mnemon3sfCheckBox->isChecked()) addrMode |= SF;
    if (ui->mnemon3xCheckBox->isChecked()) addrMode |= X;
    if (ui->mnemon3sxCheckBox->isChecked()) addrMode |= SX;
    if (ui->mnemon3sfxCheckBox->isChecked()) addrMode |= SFX;
    Pep::addrModesMap.insert(HEXO, addrMode);
    addrMode = 0;
    if (ui->mnemon4iCheckBox->isChecked()) addrMode |= I;
    if (ui->mnemon4dCheckBox->isChecked()) addrMode |= D;
    if (ui->mnemon4nCheckBox->isChecked()) addrMode |= N;
    if (ui->mnemon4sCheckBox->isChecked()) addrMode |= S;
    if (ui->mnemon4sfCheckBox->isChecked()) addrMode |= SF;
    if (ui->mnemon4xCheckBox->isChecked()) addrMode |= X;
    if (ui->mnemon4sxCheckBox->isChecked()) addrMode |= SX;
    if (ui->mnemon4sfxCheckBox->isChecked()) addrMode |= SFX;
    Pep::addrModesMap.insert(STRO, addrMode);
}

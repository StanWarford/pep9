// File: byteconverterhex.cpp
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
#include "byteconverterhex.h"
#include "ui_byteconverterhex.h"
#include <QTextCursor>

ByteConverterHex::ByteConverterHex(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ByteConverterHex)
{
    ui->setupUi(this);
    // Regular expression to validate 0x00 - 0xff
    QRegExp hexRx("0x([0-9]|[a-f]|[A-F])([0-9]|[a-f]|[A-F])");
    hexValidator = new QRegExpValidator(hexRx, this);
    ui->lineEdit->setValidator(hexValidator);
    // Forward the textEdited() signal from ui->lineEdit up to the main window
    connect(ui->lineEdit, SIGNAL(textEdited(const QString &)), this, SIGNAL(textEdited(const QString &)));
    connect(ui->lineEdit, SIGNAL(cursorPositionChanged(int,int)), this, SLOT(moveCursorAwayFromPrefix(int, int)));

}

ByteConverterHex::~ByteConverterHex()
{
    delete ui;
}

void ByteConverterHex::setValue(int value)
{
    if (value == -1) {
        ui->lineEdit->setText("0x");
    }
    else {
        ui->lineEdit->setText(QString("0x%1").arg(value, 2, 16, QLatin1Char('0')));
    }
}

void ByteConverterHex::moveCursorAwayFromPrefix(int old, int current)
{
    if (current < 2) {
        ui->lineEdit->setCursorPosition(old);
    }
}

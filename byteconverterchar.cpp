// File: byteconverterchar.cpp
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
#include "byteconverterchar.h"
#include "ui_byteconverterchar.h"

ByteConverterChar::ByteConverterChar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ByteConverterChar)
{
    ui->setupUi(this);
    // Regular expression to validate a single character
    QRegExp charRx("(.){0,1}");
    charValidator = new QRegExpValidator(charRx, this);
    ui->lineEdit->setValidator(charValidator);
    // Forward the textEdited() signal from ui->lineEdit up to the main window
    QObject::connect(ui->lineEdit, SIGNAL(textEdited(const QString &)), this,
                     SIGNAL(textEdited(const QString &)));
}

ByteConverterChar::~ByteConverterChar()
{
    delete ui;
}

void ByteConverterChar::setValue(int value)
{
    ui->lineEdit->setText(QString(QChar(value)));
}

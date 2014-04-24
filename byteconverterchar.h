// File: byteconverterchar.h
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

#ifndef BYTECONVERTERCHAR_H
#define BYTECONVERTERCHAR_H

#include <QWidget>
#include <QRegExpValidator>

namespace Ui {
    class ByteConverterChar;
}

class ByteConverterChar : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ByteConverterChar)
public:
    explicit ByteConverterChar(QWidget *parent = 0);
    virtual ~ByteConverterChar();
    void setValue(int);

private:
    Ui::ByteConverterChar *ui;
    QRegExpValidator* charValidator;

signals:
    void textEdited(const QString &);
};

#endif // BYTECONVERTERCHAR_H

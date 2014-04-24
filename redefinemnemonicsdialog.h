// File: redefinemnemonicsdialog.h
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

#ifndef REDEFINEMNEMONICSDIALOG_H
#define REDEFINEMNEMONICSDIALOG_H

#include <QDialog>
#include "pep.h"

namespace Ui {
    class RedefineMnemonicsDialog;
}

class RedefineMnemonicsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(RedefineMnemonicsDialog)
public:
    explicit RedefineMnemonicsDialog(QWidget *parent = 0);
    virtual ~RedefineMnemonicsDialog();

private:
    Ui::RedefineMnemonicsDialog *ui;

private:

private slots:
    void restoreDefaults();

    void redefineNonUnaryMnemonic0(QString);
    void redefineNonUnaryMnemonic1(QString);
    void redefineNonUnaryMnemonic2(QString);
    void redefineNonUnaryMnemonic3(QString);
    void redefineNonUnaryMnemonic4(QString);

    void redefineUnaryMnemonic0(QString);
    void redefineUnaryMnemonic1(QString);

    void setNonUnaryAllowedModes();
};

#endif // REDEFINEMNEMONICSDIALOG_H

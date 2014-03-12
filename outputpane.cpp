// File: outputpane.cpp
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
#include <QFontDialog>
#include <QScrollBar>
#include "outputpane.h"
#include "ui_outputpane.h"
#include "pep.h"

OutputPane::OutputPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputPane)
{
    ui->setupUi(this);

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->plainTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
}

OutputPane::~OutputPane()
{
    delete ui;
}

void OutputPane::appendOutput(QString str)
{
    ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText().append(str));
    ui->plainTextEdit->verticalScrollBar()->setValue(ui->plainTextEdit->verticalScrollBar()->maximum());
}

void OutputPane::clearOutput()
{
    ui->plainTextEdit->clear();
}

void OutputPane::highlightOnFocus()
{
    if (ui->plainTextEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool OutputPane::hasFocus()
{
    return ui->plainTextEdit->hasFocus();
}

void OutputPane::copy()
{
    ui->plainTextEdit->copy();
}

void OutputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->plainTextEdit->font()), this, "Set Output Font");
    if (ok) {
        ui->plainTextEdit->setFont(font);
    }
}

void OutputPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->plainTextEdit->setFocus();
}

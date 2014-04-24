// File: assemblerlistingpane.cpp
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

#include <QScrollBar>
#include <QFontDialog>
#include "assemblerlistingpane.h"
#include "ui_assemblerlistingpane.h"
#include "pep.h"

#include <QMouseEvent>

AssemblerListingPane::AssemblerListingPane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::AssemblerListingPane)
{
    ui->setupUi(this);

    pepHighlighter = new PepHighlighter(ui->textEdit->document());

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->textEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

AssemblerListingPane::~AssemblerListingPane()
{
    delete ui;
}

void AssemblerListingPane::setAssemblerListing(QStringList assemblerListingList) {
    clearAssemblerListing();
    ui->textEdit->append("-------------------------------------------------------------------------------");
    ui->textEdit->append("      Object");
    ui->textEdit->append("Addr  code   Symbol   Mnemon  Operand     Comment");
    ui->textEdit->append("-------------------------------------------------------------------------------");
    ui->textEdit->append(assemblerListingList.join("\n"));
    ui->textEdit->append("-------------------------------------------------------------------------------");
    if (Pep::symbolTable.size() > 0) {
        ui->textEdit->append("");
        ui->textEdit->append("");
        ui->textEdit->append("Symbol table");
        ui->textEdit->append("--------------------------------------");
        ui->textEdit->append("Symbol    Value        Symbol    Value");
        ui->textEdit->append("--------------------------------------");
        QMapIterator<QString, int> i(Pep::symbolTable);
        QString symbolTableLine = "";
        QString hexString;
        while (i.hasNext()) {
            i.next();
            hexString = QString("%1").arg(i.value(), 4, 16, QLatin1Char('0')).toUpper();
            if (symbolTableLine.length() == 0) {
                symbolTableLine = QString("%1%2").arg(i.key(), -10).arg(hexString, -13);
            }
            else {
                symbolTableLine.append(QString("%1%2").arg(i.key(), -10).arg(hexString, -4));
                ui->textEdit->append(symbolTableLine);
                symbolTableLine = "";
            }
        }
        if (symbolTableLine.length() > 0) {
            ui->textEdit->append(symbolTableLine);
        }
        ui->textEdit->append("--------------------------------------");
    }
    ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->minimum());
}

void AssemblerListingPane::clearAssemblerListing()
{
    ui->textEdit->clear();
}

bool AssemblerListingPane::isModified()
{
    return ui->textEdit->document()->isModified();
}

QString AssemblerListingPane::toPlainText()
{
    return ui->textEdit->toPlainText();
}

void AssemblerListingPane::setCurrentFile(QString string)
{
    if (!string.isEmpty()) {
        ui->label->setText("Assembler Listing - " + string);
    }
    else {
        ui->label->setText("Assembler Listing - untitled.pepl");
    }
}

void AssemblerListingPane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool AssemblerListingPane::hasFocus()
{
    return ui->textEdit->hasFocus();
}

void AssemblerListingPane::copy()
{
    ui->textEdit->copy();
}

void AssemblerListingPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->textEdit->font()), this, "Set Assembler Listing Font");
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void AssemblerListingPane::setFocus()
{
    ui->textEdit->setFocus();
}

bool AssemblerListingPane::isEmpty()
{
    return ui->textEdit->toPlainText() == "";
}

void AssemblerListingPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}

void AssemblerListingPane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::EListing);
}

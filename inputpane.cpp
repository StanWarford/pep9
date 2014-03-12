// File: inputpane.cpp
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
#include <QKeyEvent>
#include "inputpane.h"
#include "ui_inputpane.h"
#include "pep.h"

InputPane::InputPane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::InputPane)
{
    ui->setupUi(this);

    connect(ui->plainTextEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->plainTextEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->plainTextEdit->setFont(QFont(Pep::codeFont, Pep::ioFontSize));
}

InputPane::~InputPane()
{
    delete ui;
}

void InputPane::highlightOnFocus()
{
    if (ui->plainTextEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

QString InputPane::toPlainText()
{
    return ui->plainTextEdit->toPlainText();
}

void InputPane::setText(QString input)
{
    ui->plainTextEdit->setPlainText(input);
}

bool InputPane::hasFocus()
{
    return ui->plainTextEdit->hasFocus();
}

void InputPane::undo()
{
    ui->plainTextEdit->undo();
}

void InputPane::redo()
{
    ui->plainTextEdit->redo();
}

bool InputPane::isUndoable()
{
    return ui->plainTextEdit->document()->isUndoAvailable();
}

bool InputPane::isRedoable()
{
    return ui->plainTextEdit->document()->isRedoAvailable();
}

void InputPane::cut()
{
    ui->plainTextEdit->cut();
}

void InputPane::copy()
{
    ui->plainTextEdit->copy();
}

void InputPane::paste()
{
    ui->plainTextEdit->paste();
}

void InputPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->plainTextEdit->font()), this, "Set Input Font");
    if (ok) {
        ui->plainTextEdit->setFont(font);
    }
}

void InputPane::setReadOnly(bool b)
{
    ui->plainTextEdit->setReadOnly(b);
}

void InputPane::tab()
{
    if (!ui->plainTextEdit->isReadOnly()) {
        QTextCursor cursor = ui->plainTextEdit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);

        int curLinePos = ui->plainTextEdit->textCursor().position() - cursor.position();

        QString string;
        int spaces;
        spaces = 4 - (curLinePos % 4);

        for (int i = 0; i < spaces; i++) {
            string.append(" ");
        }

        ui->plainTextEdit->insertPlainText(string);
    }
}

void InputPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->plainTextEdit->setFocus();
}

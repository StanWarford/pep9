// File: objectcodepane.cpp
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
#include <QFontDialog>
#include "objectcodepane.h"
#include "ui_objectcodepane.h"
#include "pep.h"

ObjectCodePane::ObjectCodePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectCodePane)
{
    ui->setupUi(this);

    connect(ui->textEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(setLabelToModified(bool)));

    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->textEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

ObjectCodePane::~ObjectCodePane()
{
    delete ui;
}

void ObjectCodePane::setObjectCode(QList<int> objectCode)
{
    QString objectCodeString = "";
    for (int i = 0; i < objectCode.length(); i++) {
        objectCodeString.append(QString("%1").arg(objectCode[i], 2, 16, QLatin1Char('0')).toUpper());
        objectCodeString.append((i % 16) == 15 ? '\n' : ' ');
    }
    objectCodeString.append("zz");
    ui->textEdit->clear();
    ui->textEdit->setText(objectCodeString);
}

void ObjectCodePane::setObjectCodePaneText(QString string)
{
        ui->textEdit->setText(string);
}

bool ObjectCodePane::getObjectCode(QList<int> &objectCodeList)
{
    QString objectString = ui->textEdit->toPlainText();
    while (objectString.length() > 0) {
        if (objectString.at(1) == QChar('z')) {
            return true;
        }
        if (objectString.length() < 3) {
            return false;
        }
        QString s = objectString.left(2); // Get the two-char hex number
        objectString.remove(0, 3); // Removes the number and trailing whitespace
        bool ok;
        objectCodeList.append(s.toInt(&ok, 16));
        if (!ok) {
            return false;
        }
    }
    return false;
}

void ObjectCodePane::clearObjectCode()
{
    ui->textEdit->clear();
}

bool ObjectCodePane::isModified()
{
    return ui->textEdit->document()->isModified();
}

void ObjectCodePane::setModifiedFalse()
{
    ui->textEdit->document()->setModified(false);
}

QString ObjectCodePane::toPlainText()
{
    return ui->textEdit->toPlainText();
}

void ObjectCodePane::setCurrentFile(QString string)
{
    if (!string.isEmpty()) {
        ui->label->setText("Object Code - " + string);
    }
    else {
        ui->label->setText("Object Code - untitled.pepo");
    }
}

void ObjectCodePane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool ObjectCodePane::hasFocus()
{
    return ui->textEdit->hasFocus();
}

void ObjectCodePane::undo()
{
    ui->textEdit->undo();
}

void ObjectCodePane::redo()
{
    ui->textEdit->redo();
}

bool ObjectCodePane::isUndoable()
{
    return ui->textEdit->document()->isUndoAvailable();
}

bool ObjectCodePane::isRedoable()
{
    return ui->textEdit->document()->isRedoAvailable();
}

void ObjectCodePane::cut()
{
    ui->textEdit->cut();
}

void ObjectCodePane::copy()
{
    ui->textEdit->copy();
}

void ObjectCodePane::paste()
{
    ui->textEdit->paste();
}

void ObjectCodePane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->textEdit->font()), this, "Set Object Code Font");
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

void ObjectCodePane::setReadOnly(bool b)
{
    ui->textEdit->setReadOnly(b);
}

void ObjectCodePane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}

void ObjectCodePane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::EObject);
}

void ObjectCodePane::setLabelToModified(bool modified)
{
    QString temp = ui->label->text();
    if (modified) {
        ui->label->setText(temp.append(temp.endsWith(QChar('*')) ? "" : "*"));
    }
    else if (temp.endsWith(QChar('*'))) {
        temp.chop(1);
        ui->label->setText(temp);
    }
}


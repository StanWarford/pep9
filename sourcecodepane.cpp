// File: sourcecodepane.cpp
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

#include <QList>
#include <QStringList>
#include <QTextCursor>
#include <QPalette>
#include <QSyntaxHighlighter>
#include <QFontDialog>
#include <QKeyEvent>
#include "sourcecodepane.h"
#include "ui_sourcecodepane.h"
#include "code.h"
#include "sim.h"
#include "pep.h"

// #include <QDebug>

SourceCodePane::SourceCodePane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SourceCodePane)
{
    ui->setupUi(this);

    connect(ui->textEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(setLabelToModified(bool)));

    pepHighlighter = new PepHighlighter(ui->textEdit->document());

    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->textEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

SourceCodePane::~SourceCodePane()
{
    delete ui;
}

bool SourceCodePane::assemble()
{
    QString sourceLine;
    QString errorString;
    QStringList sourceCodeList;
    Code *code;
    int lineNum = 0;
    bool dotEndDetected = false;

    removeErrorMessages();
    Asm::listOfReferencedSymbols.clear();
    Asm::listOfReferencedSymbolLineNums.clear();
    Pep::memAddrssToAssemblerListing->clear();
    Pep::symbolTable.clear();
    Pep::adjustSymbolValueForBurn.clear();
    Pep::symbolFormat.clear();
    Pep::symbolFormatMultiplier.clear();;
    Pep::symbolTraceList.clear(); // Does this clear the lists within the map?
    Pep::globalStructSymbols.clear();
    Pep::blockSymbols.clear();
    Pep::equateSymbols.clear();
    while (!codeList.isEmpty()) {
        delete codeList.takeFirst();
    }
    QString sourceCode = ui->textEdit->toPlainText();
    sourceCodeList = sourceCode.split('\n');
    Pep::byteCount = 0;
    Pep::burnCount = 0;
    while (lineNum < sourceCodeList.size() && !dotEndDetected) {
        sourceLine = sourceCodeList[lineNum];
        if (!Asm::processSourceLine(sourceLine, lineNum, code, errorString, dotEndDetected)) {
            appendMessageInSourceCodePaneAt(lineNum, errorString);
            return false;
        }
        codeList.append(code);
        lineNum++;
    }
    if (!dotEndDetected) {
        errorString = ";ERROR: Missing .END sentinel.";
        appendMessageInSourceCodePaneAt(0, errorString);
        return false;
    }
    if (Pep::byteCount > 65535) {
        errorString = ";ERROR: Object code size too large to fit into memory.";
        appendMessageInSourceCodePaneAt(0, errorString);
        return false;
    }
    for (int i = 0; i < Asm::listOfReferencedSymbols.length(); i++) {
        if (!Pep::symbolTable.contains(Asm::listOfReferencedSymbols[i])
                && !(Asm::listOfReferencedSymbols[i] == "charIn")
                && !(Asm::listOfReferencedSymbols[i] == "charOut")) {
            errorString = ";ERROR: Symbol " + Asm::listOfReferencedSymbols[i] + " is used but not defined.";
            appendMessageInSourceCodePaneAt(Asm::listOfReferencedSymbolLineNums[i], errorString);
            return false;
        }
    }
    Pep::traceTagWarning = false;
    for (int i = 0; i < codeList.size(); i++) {
        if (!codeList[i]->processFormatTraceTags(lineNum, errorString)) {
            appendMessageInSourceCodePaneAt(lineNum, errorString);
            Pep::traceTagWarning = true;
        }
    }
    if (!Pep::traceTagWarning && !(Pep::blockSymbols.isEmpty() && Pep::equateSymbols.isEmpty())) {
        for (int i = 0; i < codeList.size(); i++) {
            if (!codeList[i]->processSymbolTraceTags(lineNum, errorString)) {
                appendMessageInSourceCodePaneAt(lineNum, errorString);
                Pep::traceTagWarning = true;
            }
        }
    }
    return true;
}

QList<int> SourceCodePane::getObjectCode()
{
    objectCode.clear();
    for (int i = 0; i < codeList.size(); ++i) {
        codeList.at(i)->appendObjectCode(objectCode);
    }
    return objectCode;
}

QStringList SourceCodePane::getAssemblerListingList()
{
    assemblerListingList.clear();
    listingTraceList.clear();
    hasCheckBox.clear();
    for (int i = 0; i < codeList.length(); i++) {
        codeList[i]->appendSourceLine(assemblerListingList, listingTraceList, hasCheckBox);
    }
    return assemblerListingList;
}

QStringList SourceCodePane::getListingTraceList()
{
    return listingTraceList;
}

QList<bool> SourceCodePane::getHasCheckBox()
{
    return hasCheckBox;
}

void SourceCodePane::adjustCodeList(int addressDelta)
{
    for (int i = 0; i < codeList.length(); i++) {
        codeList[i]->adjustMemAddress(addressDelta);
    }
}

void SourceCodePane::installOS()
{
    for (int i = 0; i < 65536; i++) {
        Sim::Mem[i] = 0;
    }
    int j = Pep::romStartAddress;
    for (int i = 0; i < objectCode.size(); i++) {
        Sim::Mem[j++] = objectCode[i];
    }
}

bool SourceCodePane::installDefaultOs()
{
    QString sourceLine;
    QString errorString;
    QStringList sourceCodeList;
    Code *code;
    int lineNum = 0;
    bool dotEndDetected = false;

    Asm::listOfReferencedSymbols.clear();
    Pep::memAddrssToAssemblerListing->clear();
    Pep::symbolTable.clear();
    Pep::adjustSymbolValueForBurn.clear();
    while (!codeList.isEmpty()) {
        delete codeList.takeFirst();
    }
    QString sourceCode = Pep::resToString(":/help/figures/pep9os.pep");
    sourceCodeList = sourceCode.split('\n');
    Pep::byteCount = 0;
    Pep::burnCount = 0;
    while (lineNum < sourceCodeList.size() && !dotEndDetected) {
        sourceLine = sourceCodeList[lineNum];
        if (!Asm::processSourceLine(sourceLine, lineNum, code, errorString, dotEndDetected)) {
            return false;
        }
        codeList.append(code);
        lineNum++;
    }
    if (!dotEndDetected) {
        return false;
    }
    if (Pep::byteCount > 65535) {
        return false;
    }
    for (int i = 0; i < Asm::listOfReferencedSymbols.length(); i++) {
        if (!Pep::symbolTable.contains(Asm::listOfReferencedSymbols[i])) {
            return false;
        }
    }

    if (Pep::burnCount != 1) {
        return false;
    }

    // Adjust for .BURN
    int addressDelta = Pep::dotBurnArgument - Pep::byteCount + 1;
    QMutableMapIterator <QString, int> i(Pep::symbolTable);
    while (i.hasNext()) {
        i.next();
        if (Pep::adjustSymbolValueForBurn.value(i.key())) {
            i.setValue(i.value() + addressDelta);
        }
    }

    adjustCodeList(addressDelta);
    Pep::romStartAddress += addressDelta;
    getObjectCode();
    installOS();

    return true;
}

void SourceCodePane::removeErrorMessages()
{
    QTextCursor cursor(ui->textEdit->document()->find(";ERROR:"));
    while (!cursor.isNull()) {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor = ui->textEdit->document()->find(";ERROR:", cursor);
    }
    cursor = ui->textEdit->document()->find(";WARNING:");
    while (!cursor.isNull()) {
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor = ui->textEdit->document()->find(";WARNING:", cursor);
    }
}

void SourceCodePane::appendMessageInSourceCodePaneAt(int lineNumber, QString message)
{
    QTextCursor cursor(ui->textEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < lineNumber; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }
    cursor.movePosition(QTextCursor::EndOfLine);
    cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
    if (cursor.selectedText() == " ") {
        cursor.setPosition(cursor.anchor());
    }
    else {
        cursor.setPosition(cursor.anchor());
        cursor.insertText(" ");
    }
    cursor.insertText(message);
}

void SourceCodePane::setSourceCodePaneText(QString string)
{
    ui->textEdit->setText(string);
}

void SourceCodePane::clearSourceCode()
{
    ui->textEdit->clear();
    codeList.clear(); // This may cause issues with "format from listing" - but this needs to be cleared regardless.
}

bool SourceCodePane::isModified()
{
    return ui->textEdit->document()->isModified();
}

void SourceCodePane::setModifiedFalse()
{
    ui->textEdit->document()->setModified(false);
}

QString SourceCodePane::toPlainText()
{
    return ui->textEdit->toPlainText();
}

void SourceCodePane::setCurrentFile(QString string)
{
    ui->label->setText("Source Code - " + string);
}

void SourceCodePane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool SourceCodePane::hasFocus()
{
    return ui->textEdit->hasFocus();
}

void SourceCodePane::undo()
{
    ui->textEdit->undo();
}

void SourceCodePane::redo()
{
    ui->textEdit->redo();
}

bool SourceCodePane::isUndoable()
{
    return ui->textEdit->document()->isUndoAvailable();
}

bool SourceCodePane::isRedoable()
{
    return ui->textEdit->document()->isRedoAvailable();
}

void SourceCodePane::cut()
{
    ui->textEdit->cut();
}

void SourceCodePane::copy()
{
    ui->textEdit->copy();
}

void SourceCodePane::paste()
{
    ui->textEdit->paste();
}

void SourceCodePane::setReadOnly(bool b)
{
    ui->textEdit->setReadOnly(b);
}

void SourceCodePane::tab()
{
    if (!ui->textEdit->isReadOnly()) {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::StartOfLine);
        QString string;
        int curLinePos = ui->textEdit->textCursor().position() - cursor.position();
        int spaces;
        if (curLinePos < 9) {
            spaces = 9 - curLinePos;
        }
        else if (curLinePos < 17) {
            spaces = 17 - curLinePos;
        }
        else if (curLinePos < 29) {
            spaces = 29 - curLinePos;
        }
        else if (curLinePos == 29) {
            spaces = 5;
        }
        else {
            spaces = 4 - ((curLinePos - 30) % 4);
        }

        for (int i = 0; i < spaces; i++) {
            string.append(" ");
        }

        ui->textEdit->insertPlainText(string);
    }
}

void SourceCodePane::writeSettings(QSettings &settings)
{
    settings.beginGroup("SourceCodePane");
    settings.endGroup();
}

void SourceCodePane::readSettings(QSettings &settings)
{
    settings.beginGroup("SourceCodePane");
    settings.endGroup();
}

void SourceCodePane::onFontChanged(QFont font)
{
    ui->textEdit->setFont(font);
}

// TODO: Make syntax highlighting persistent between runs, like fonts are.
void SourceCodePane::onColorChanged() {
    pepHighlighter = new PepHighlighter(ui->textEdit->document());
    pepHighlighter->rehighlight();
}

void SourceCodePane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}

void SourceCodePane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::ESource);
}

void SourceCodePane::setLabelToModified(bool modified)
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


// File: memorydumppane.cpp
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
#include <QTextCharFormat>
#include <QAbstractTextDocumentLayout>
#include "memorydumppane.h"
#include "ui_memorydumppane.h"
#include "sim.h"
#include "pep.h"
#include "enu.h"

#include <QDebug>

MemoryDumpPane::MemoryDumpPane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemoryDumpPane)
{
    ui->setupUi(this);

    if (Pep::getSystem() != "Mac") {
        ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
        ui->textEdit->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    }

    connect(ui->pcPushButton, SIGNAL(clicked()), this, SLOT(scrollToPC()));
    connect(ui->spPushButton, SIGNAL(clicked()), this, SLOT(scrollToSP()));
    connect(ui->scrollToLineEdit, SIGNAL(textChanged(QString)), this, SLOT(scrollToAddress(QString)));
}

MemoryDumpPane::~MemoryDumpPane()
{
    delete ui;
}

void MemoryDumpPane::refreshMemory()
{
    QStringList memoryDump;
    QString memoryDumpLine;
    QChar ch;
    for (int i = 0; i < 65536; i += 8) {
        memoryDumpLine = "";
        memoryDumpLine.append(QString("%1 | ").arg(i, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[i + j], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[i + j]);
            if (ch.isPrint()) {
                memoryDumpLine.append(ch);
            } else {
                memoryDumpLine.append(".");
            }
        }
        memoryDump.append(memoryDumpLine);
    }
    ui->textEdit->setText(memoryDump.join("\n"));
}

void MemoryDumpPane::refreshMemoryLines(int firstByte, int lastByte)
{
    int vertScrollBarPosition = ui->textEdit->verticalScrollBar()->value();
    int horizScrollBarPosition = ui->textEdit->horizontalScrollBar()->value();

    int firstLine = firstByte / 8;
    int lastLine = lastByte / 8;

    QTextCursor cursor(ui->textEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < firstLine; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }

    QString memoryDumpLine;
    QChar ch;
    int byteNum;
    for (int i = firstLine; i <= lastLine; i++) {
        memoryDumpLine = "";
        byteNum = i * 8;
        memoryDumpLine.append(QString("%1 | ").arg(byteNum, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[byteNum++], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        byteNum = i * 8;
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[byteNum++]);
            if (ch.isPrint()) {
                memoryDumpLine.append(ch);
            } else {
                memoryDumpLine.append(".");
            }
        }
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->insertPlainText(memoryDumpLine);
        cursor.movePosition(QTextCursor::NextBlock);
    }

    ui->textEdit->verticalScrollBar()->setValue(vertScrollBarPosition);
    ui->textEdit->horizontalScrollBar()->setValue(horizScrollBarPosition);
}

void MemoryDumpPane::highlightMemory(bool b)
{
    while (!highlightedData.isEmpty()) {
        highlightByte(highlightedData.takeFirst(), Qt::black, Qt::white);
    }

    if (b) {
        highlightByte(Sim::stackPointer, Qt::white, Qt::darkMagenta);
        highlightedData.append(Sim::stackPointer);
        
        if (!Pep::isUnaryMap.value(Pep::decodeMnemonic.value(Sim::readByte(Sim::programCounter)))) {
            QTextCursor cursor(ui->textEdit->document());
            QTextCharFormat format;
            format.setBackground(Qt::blue);
            format.setForeground(Qt::white);
            cursor.setPosition(0);
            for (int i = 0; i < Sim::programCounter / 8; i++) {
                cursor.movePosition(QTextCursor::NextBlock);
            }
            for (int i = 0; i < 7 + 3 * (Sim::programCounter % 8); i++) {
                cursor.movePosition(QTextCursor::NextCharacter);
            }
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
            cursor.mergeCharFormat(format);
            highlightedData.append(Sim::programCounter);
            if (Sim::programCounter / 8 == (Sim::programCounter + 1) / 8) {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextCharacter);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            else {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextBlock);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, 7);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            highlightedData.append(Sim::add(Sim::programCounter, 1));
            if ((Sim::programCounter + 1) / 8 == (Sim::programCounter + 2) / 8) {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextCharacter);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            else {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::NextBlock);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, 7);
                cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
                cursor.mergeCharFormat(format);
            }
            highlightedData.append(Sim::add(Sim::programCounter, 2));
        }
        else { // unary.
            highlightByte(Sim::programCounter, Qt::white, Qt::blue);
            highlightedData.append(Sim::programCounter);
        }

        bytesWrittenLastStep = bytesWrittenLastStep.toSet().toList();
        qSort(bytesWrittenLastStep);
        while (!bytesWrittenLastStep.isEmpty()) {
            // This is to prevent bytes modified by the OS from being highlighted when we are not tracing traps:
            if (bytesWrittenLastStep.at(0) < Sim::readWord(Pep::dotBurnArgument - 0x7) || Sim::trapped) {
                highlightByte(bytesWrittenLastStep.at(0), Qt::white, Qt::red);
                highlightedData.append(bytesWrittenLastStep.takeFirst());
            }
            else {
                return;
            }
        }
    }
}

void MemoryDumpPane::cacheModifiedBytes()
{
    modifiedBytes.unite(Sim::modifiedBytes);
    if (Sim::tracingTraps) {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
    else if (Sim::trapped) {
        delayLastStepClear = true;
        bytesWrittenLastStep.append(Sim::modifiedBytes.toList());
    }
    else if (delayLastStepClear) {
        delayLastStepClear = false;
    }
    else {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
}

void MemoryDumpPane::updateMemory()
{
    int vertScrollBarPosition = ui->textEdit->verticalScrollBar()->value();
    int horizScrollBarPosition = ui->textEdit->horizontalScrollBar()->value();

    QList<int> list;
    QSet<int> linesToBeUpdated;
    QString memoryDumpLine;
    QChar ch;
    int byteNum;
    int lineNum;

    modifiedBytes.unite(Sim::modifiedBytes);
    list = modifiedBytes.toList();
    while(!list.isEmpty()) {
        linesToBeUpdated.insert(list.takeFirst() / 8);
    }
    list = linesToBeUpdated.toList();
    qSort(list.begin(), list.end());
    QTextCursor cursor(ui->textEdit->document());
    cursor.setPosition(0);
    lineNum = 0;
    while (!list.isEmpty()) {
        while (lineNum < list.first()) {
            cursor.movePosition(QTextCursor::NextBlock);
            lineNum++;
        }

        memoryDumpLine = "";
        byteNum = lineNum * 8;
        memoryDumpLine.append(QString("%1 | ").arg(byteNum, 4, 16, QLatin1Char('0')).toUpper());
        for (int j = 0; j < 8; j++) {
            memoryDumpLine.append(QString("%1 ").arg(Sim::Mem[byteNum++], 2, 16, QLatin1Char('0')).toUpper());
        }
        memoryDumpLine.append("|");
        byteNum = lineNum * 8;
        for (int j = 0; j < 8; j++) {
            ch = QChar(Sim::Mem[byteNum++]);
            if (ch.isPrint()) {
                memoryDumpLine.append(ch);
            } else {
                memoryDumpLine.append(".");
            }
        }
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        ui->textEdit->setTextCursor(cursor);
        ui->textEdit->insertPlainText(memoryDumpLine);
        cursor.movePosition(QTextCursor::NextBlock);
        lineNum++;
        list.removeFirst();
    }
    modifiedBytes.clear();

    ui->textEdit->verticalScrollBar()->setValue(vertScrollBarPosition);
    ui->textEdit->horizontalScrollBar()->setValue(horizScrollBarPosition);

}

void MemoryDumpPane::scrollToTop()
{
    ui->textEdit->verticalScrollBar()->setValue(0);
    ui->textEdit->horizontalScrollBar()->setValue(0);
}

void MemoryDumpPane::highlightOnFocus()
{
    if (ui->textEdit->hasFocus() || ui->scrollToLineEdit->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool MemoryDumpPane::hasFocus()
{
    return ui->textEdit->hasFocus() || ui->scrollToLineEdit->hasFocus();
}

void MemoryDumpPane::copy()
{
    ui->textEdit->copy();
}

void MemoryDumpPane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->textEdit->font()), this, "Set Memory Dump Font");
    if (ok) {
        ui->textEdit->setFont(font);
    }
}

int MemoryDumpPane::memoryDumpWidth()
{
    return ui->textEdit->document()->documentLayout()->documentSize().toSize().width() +
            ui->textEdit->verticalScrollBar()->width() + 6;
}

void MemoryDumpPane::highlightByte(int memAddr, QColor foreground, QColor background)
{
    QTextCursor cursor(ui->textEdit->document());
    cursor.setPosition(0);
    for (int i = 0; i < memAddr / 8; i++) {
        cursor.movePosition(QTextCursor::NextBlock);
    }
    for (int i = 0; i < 7 + 3 * (memAddr % 8); i++) {
        cursor.movePosition(QTextCursor::NextCharacter);
    }
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
    QTextCharFormat format;
    format.setBackground(background);
    format.setForeground(foreground);
    cursor.mergeCharFormat(format);
}

void MemoryDumpPane::mouseReleaseEvent(QMouseEvent *)
{
    ui->textEdit->setFocus();
}

void MemoryDumpPane::scrollToByte(int byte)
{
    int min = ui->textEdit->verticalScrollBar()->minimum();
    int max = ui->textEdit->verticalScrollBar()->maximum();
    ui->textEdit->verticalScrollBar()->setValue(min + static_cast<int>(8 * (byte / 4096 - 8) + ((byte - byte % 8) / 65536.0) * (max - min)));
}

void MemoryDumpPane::scrollToPC()
{
    ui->scrollToLineEdit->setText(QString("0x") + QString("%1").arg(Sim::programCounter, 4, 16, QLatin1Char('0')).toUpper());
}

void MemoryDumpPane::scrollToSP()
{
    ui->scrollToLineEdit->setText(QString("0x") + QString("%1").arg(Sim::stackPointer, 4, 16, QLatin1Char('0')).toUpper());
}

void MemoryDumpPane::scrollToAddress(QString string)
{
    bool ok;
    int byte;
    if (string.startsWith("0x", Qt::CaseInsensitive)) {
        byte = string.toInt(&ok, 16);
        if (ok) {
            if (byte > 65535) {
                ui->scrollToLineEdit->setText("0xFFFF");
            } else {
                scrollToByte(byte);
            }
        }
        else {
            ui->scrollToLineEdit->setText("0x");
        }
    }
    else {
        ui->scrollToLineEdit->setText("0x");
    }

//    else {
//        byte = string.toInt(&ok, 10);
//        if (ok) {
//            if (byte < 0) {
//                ui->scrollToLineEdit->setText("0");
//            }
//            else if (byte > 65535) {
//                ui->scrollToLineEdit->setText("65535");
//            } else {
//                scrollToByte(byte);
//                ui->scrollToLineEdit->setText(QString("%1").arg(byte));
//            }
//        }
//        else {
//            ui->scrollToLineEdit->setText("");
//        }
//    }
}

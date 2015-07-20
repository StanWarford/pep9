// File: memorytracepane.cpp
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
#include <QRgb>
#include "memorytracepane.h"
#include "ui_memorytracepane.h"
#include "pep.h"
#include "sim.h"
#include "asm.h"

#include <QMessageBox>
#include <QDebug>

MemoryTracePane::MemoryTracePane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::MemoryTracePane)
{
    ui->setupUi(this);

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->graphicsView->setFont(QFont(Pep::codeFont, Pep::codeFontSize));

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(zoomFactorChanged(int)));

    scene = new QGraphicsScene(this);
}

MemoryTracePane::~MemoryTracePane()
{
    delete ui;
}

void MemoryTracePane::setMemoryTrace()
{
    globalVars.clear();
    runtimeStack.clear();
    heap.clear();
    isRuntimeStackItemAddedStack.clear();
    isHeapItemAddedStack.clear();
    isStackFrameAddedStack.clear();
    isHeapFrameAddedStack.clear();
    stackHeightToStackFrameMap.clear();
    modifiedBytes.clear();
    bytesWrittenLastStep.clear();
    addressToGlobalItemMap.clear();
    addressToStackItemMap.clear();
    addressToHeapItemMap.clear();
    numCellsInStackFrame.clear();
    graphicItemsInStackFrame.clear();
    heapFrameItemStack.clear();
    newestHeapItemsList.clear();
    scene->clear();

    if (Pep::traceTagWarning) {
        hide();
        return;
    }

    stackLocation = QPointF(200, 0);
    globalLocation = QPointF(0, 0);
    heapLocation = QPointF(400, 0 - MemoryCellGraphicsItem::boxHeight);
    QString blockSymbol;
    int multiplier;

    // Globals:
    for (int i = 0; i < Pep::blockSymbols.size(); i++) {
        blockSymbol = Pep::blockSymbols.at(i);
        multiplier = Pep::symbolFormatMultiplier.value(blockSymbol);
        int address = Pep::symbolTable.value(blockSymbol);
        if (Pep::globalStructSymbols.contains(blockSymbol)) {
            int offset = 0;
            int bytesPerCell;
            QString structField = "";
            for (int j = 0; j < Pep::globalStructSymbols.value(blockSymbol).size(); j++) {
                structField = Pep::globalStructSymbols.value(blockSymbol).at(j);
                bytesPerCell = Sim::cellSize(Pep::symbolFormat.value(structField));
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address + offset,
                                                                          QString("%1.%2").arg(blockSymbol).arg(structField),
                                                                          Pep::symbolFormat.value(structField),
                                                                          static_cast<int>(globalLocation.x()),
                                                                          static_cast<int>(globalLocation.y()));
                item->updateValue();
                globalLocation = QPointF(globalLocation.x(), globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
                globalVars.push(item);
                addressToGlobalItemMap.insert(address + offset, item);
                scene->addItem(item);
                offset += bytesPerCell;
            }
        }
        else {
            if (multiplier == 1) {
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address,
                                                                          blockSymbol,
                                                                          Pep::symbolFormat.value(blockSymbol),
                                                                          static_cast<int>(globalLocation.x()),
                                                                          static_cast<int>(globalLocation.y()));
                item->updateValue();
                globalLocation = QPointF(globalLocation.x(), globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
                globalVars.push(item);
                addressToGlobalItemMap.insert(address, item);
                scene->addItem(item);
            }
            else { // Array
                int offset = 0;
                int bytesPerCell = Sim::cellSize(Pep::symbolFormat.value(blockSymbol));
                for (int j = 0; j < multiplier; j++) {
                    MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(address + offset,
                                                                              blockSymbol + QString("[%1]").arg(j),
                                                                              Pep::symbolFormat.value(blockSymbol),
                                                                              static_cast<int>(globalLocation.x()),
                                                                              static_cast<int>(globalLocation.y()));
                    item->updateValue();
                    globalLocation = QPointF(globalLocation.x(), globalLocation.y() + MemoryCellGraphicsItem::boxHeight);
                    globalVars.push(item);
                    addressToGlobalItemMap.insert(address + offset, item);
                    scene->addItem(item);
                    offset += bytesPerCell;
                }
            }
        }
    }

    // Stack frame:
    stackLocation.setY(globalLocation.y());
    scene->addLine(stackLocation.x() - MemoryCellGraphicsItem::boxWidth * 0.2, stackLocation.y(),
                   stackLocation.x() + MemoryCellGraphicsItem::boxWidth * 1.2, stackLocation.y(),
                   QPen(QBrush(Qt::SolidPattern), 2, Qt::SolidLine));
    int dist = static_cast<int>(MemoryCellGraphicsItem::boxWidth * 1.2 - MemoryCellGraphicsItem::boxWidth * 1.4);
    for (int i = static_cast<int>(MemoryCellGraphicsItem::boxWidth * 1.2); i > dist; i = i - 10) {
        scene->addLine(stackLocation.x() + i - 10, stackLocation.y() + 10,
                       stackLocation.x() + i, stackLocation.y(),
                       QPen(QBrush(Qt::SolidPattern), 1, Qt::SolidLine));
    }
    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);

    heapLocation.setY(globalLocation.y() - MemoryCellGraphicsItem::boxHeight);
    
    scene->setSceneRect(scene->itemsBoundingRect());
    ui->graphicsView->setScene(scene);

    ui->warningLabel->clear();

    stackFrameFSM.reset();
}

void MemoryTracePane::updateMemoryTrace()
{
    // Color all of the cells normally (globals)
    for (int i = 0; i < globalVars.size(); i++) {
        globalVars.at(i)->boxBgColor = Qt::white;
        globalVars.at(i)->boxTextColor = Qt::black;
    }
    // Color all of the cells normally (stack)
    for (int i = 0; i < runtimeStack.size(); i++) {
        runtimeStack.at(i)->boxBgColor = Qt::white;
        runtimeStack.at(i)->boxTextColor = Qt::black;
    }
    // Color all of the cells normally (heap)
    for (int i = 0; i < heap.size(); i++) {
        heap.at(i)->boxBgColor = Qt::white;
        heap.at(i)->boxTextColor = Qt::black;
    }
    // Color the newest 'malloc' items on the heap light green
    for (int i = 0; i < newestHeapItemsList.size(); i++) {
        newestHeapItemsList.at(i)->boxBgColor = QColor(72, 255, 72, 192);
    }
    newestHeapItemsList.clear();

    // Add cached stack items to the scene
    for (int i = 0; i < runtimeStack.size(); i++) {
        if (!isRuntimeStackItemAddedStack.at(i)) {
            scene->addItem(runtimeStack.at(i));
            isRuntimeStackItemAddedStack[i] = true;
        }
    }
    // Add cached stack FRAME items to the scene
    for (int i = 0; i < isStackFrameAddedStack.size(); i++) {
        if (!isStackFrameAddedStack.at(i)) {
            scene->addItem(graphicItemsInStackFrame.at(i));
            isStackFrameAddedStack[i] = true;
        }
    }

    // Add cached heap items to the scene
    for (int i = 0; i < isHeapItemAddedStack.size(); i++) {
        if (!isHeapItemAddedStack.at(i)) {
            scene->addItem(heap.at(i));
            isHeapItemAddedStack[i] = true;
        }
    }
    for (int i = 0; i < isHeapFrameAddedStack.size(); i++) {
        if (!isHeapFrameAddedStack.at(i)) {
            scene->addItem(heapFrameItemStack.at(i));
            isHeapFrameAddedStack[i] = true;
        }
    }

    // Color global/stack/heap items red if they were modified last step
    QList<int> modifiedBytesToBeUpdated = modifiedBytes.toList();
    for (int i = 0; i < bytesWrittenLastStep.size(); i++) {
        if (addressToGlobalItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToGlobalItemMap.value(bytesWrittenLastStep.at(i))->boxBgColor = Qt::red;
            addressToGlobalItemMap.value(bytesWrittenLastStep.at(i))->boxTextColor = Qt::white;
        }
        if (addressToStackItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToStackItemMap.value(bytesWrittenLastStep.at(i))->boxBgColor = Qt::red;
            addressToStackItemMap.value(bytesWrittenLastStep.at(i))->boxTextColor = Qt::white;
        }
        if (addressToHeapItemMap.contains(bytesWrittenLastStep.at(i))) {
            addressToHeapItemMap.value(bytesWrittenLastStep.at(i))->boxBgColor = Qt::red;
            addressToHeapItemMap.value(bytesWrittenLastStep.at(i))->boxTextColor = Qt::white;
        }
    }
    // Update modified cells
    for (int i = 0; i < modifiedBytesToBeUpdated.size(); i++) {
        if (addressToGlobalItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToGlobalItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
        if (addressToStackItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToStackItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
        if (addressToHeapItemMap.contains(modifiedBytesToBeUpdated.at(i))) {
            addressToHeapItemMap.value(modifiedBytesToBeUpdated.at(i))->updateValue();
        }
    }

    scene->setSceneRect(scene->itemsBoundingRect());
    // This is time-consuming, but worthwhile to ensure scrollbars aren't going off into oblivion after items are removed.
    // From the documentation for the 'itemsBoundingRect()' function:
    //  Calculates and returns the bounding rect of all items on the scene.
    //  This function works by iterating over all items, and because if this, it can be slow for large scenes.
    // Our it is unlikely that this scene become very large (as in the 30,000 chips example), so I would expect this to remain
    // a reasonable function call. It is also called in the 'setMemoryTrace' function, but the scene will have very few items at that point.

    scene->invalidate(); // redraw the scene!
    // this is fast, so we do this for the whole scene instead of just certain boxes

    // Scroll to the top item if we have a scrollbar:
    if (!runtimeStack.isEmpty() && ui->graphicsView->viewport()->height() < scene->height()) {
        ui->graphicsView->centerOn(runtimeStack.top());
    }

    // Clear modified bytes so for the next update:
    bytesWrittenLastStep.clear();
    modifiedBytes.clear();
}

void MemoryTracePane::cacheChanges()
{
    modifiedBytes.unite(Sim::modifiedBytes);
    if (Sim::tracingTraps) {
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
    else if (Sim::trapped) {
        // We delay for a single vonNeumann step so that we preserve the modified bytes until we leave the trap - this allows for
        // recoloring of cells modified by a trap instruction.
        delayLastStepClear = true;
        bytesWrittenLastStep.append(Sim::modifiedBytes.toList());
    }
    else if (delayLastStepClear) {
        // Phew! We can now update (in updateMemoryTrace). If we don't, no harm done - they didn't want to see what happened in the trap
        delayLastStepClear = false;
    }
    else {
        // Clear the bytes written the step before last, and get the new list from the previous step. This is used in our update for coloring.
        bytesWrittenLastStep.clear();
        bytesWrittenLastStep = Sim::modifiedBytes.toList();
    }
}

void MemoryTracePane::cacheStackChanges()
{
    if (Sim::trapped) {
        return;
    }

    // Look ahead for the symbol trace list (needs to be done here because of the possibility of call (can't look behind on a call)
    // so we just do it for them all)
    switch (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]) {
    case Enu::SUBSP:
    case Enu::CALL:
    case Enu::RET:
    case Enu::ADDSP:
        if (Pep::symbolTraceList.contains(Sim::programCounter)) {
            lookAheadSymbolList = Pep::symbolTraceList.value(Sim::programCounter);
        }
        break;
    default:
        break;
    }
    // End look ahead

    int multiplier = 0;
    int bytesPerCell = 0;
    int offset = 0;
    int numCellsToAdd = 0;
    int frameSizeToAdd = 0;
    QString stackSymbol;

    switch (Pep::decodeMnemonic[Sim::instructionSpecifier]) {
    case Enu::CALL:
        {
            MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer, "retAddr", Enu::F_2H,
                                                                      static_cast<int>(stackLocation.x()), static_cast<int>(stackLocation.y()));
            item->updateValue();
            stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);

            isRuntimeStackItemAddedStack.push(false);
            runtimeStack.push(item);
            addressToStackItemMap.insert(Sim::stackPointer, item);
            frameSizeToAdd = stackFrameFSM.makeTransition(1);
        }
        break;
    case Enu::SUBSP:
        {
            for (int i = 0; i < lookAheadSymbolList.size(); i++) {
                stackSymbol = lookAheadSymbolList.at(i);
                multiplier = Pep::symbolFormatMultiplier.value(stackSymbol);
                if (multiplier == 1) {
                    offset += Sim::cellSize(Pep::symbolFormat.value(stackSymbol));
                    MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier,
                                                                              stackSymbol,
                                                                              Pep::symbolFormat.value(stackSymbol),
                                                                              static_cast<int>(stackLocation.x()),
                                                                              static_cast<int>(stackLocation.y()));
                    item->updateValue();
                    stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                    isRuntimeStackItemAddedStack.push(false);
                    runtimeStack.push(item);
                    addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
                    numCellsToAdd++;
                }
                else { // This is an array!
                    bytesPerCell = Sim::cellSize(Pep::symbolFormat.value(stackSymbol));
                    for (int j = multiplier - 1; j >= 0; j--) {
                        offset += bytesPerCell;
                        MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::stackPointer - offset + Sim::operandSpecifier,
                                                                                  stackSymbol + QString("[%1]").arg(j),
                                                                                  Pep::symbolFormat.value(stackSymbol),
                                                                                  static_cast<int>(stackLocation.x()),
                                                                                  static_cast<int>(stackLocation.y()));
                        item->updateValue();
                        stackLocation.setY(stackLocation.y() - MemoryCellGraphicsItem::boxHeight);
                        isRuntimeStackItemAddedStack.push(false);
                        runtimeStack.push(item);
                        addressToStackItemMap.insert(Sim::stackPointer - offset + Sim::operandSpecifier, item);
                        numCellsToAdd++;
                    }
                }
            }
            // qDebug() << "numCellsToAdd before makeTransition in ADDSP: " << numCellsToAdd;
            frameSizeToAdd = stackFrameFSM.makeTransition(numCellsToAdd);
        }
        break;
    case Enu::RET:
        popBytes(2);
        frameSizeToAdd = stackFrameFSM.makeTransition(0); // makeTransition(0) -> 0 bytes to add to the stack frame FSM.
        break;
    case Enu::ADDSP:
        popBytes(Sim::operandSpecifier);
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    default:
        frameSizeToAdd = stackFrameFSM.makeTransition(0);
        break;
    }

    if (frameSizeToAdd != 0) {
        addStackFrame(frameSizeToAdd);
        // This map is used to correlate the top of the stack frame with the frame itself,
        // useful for determining when the frame should dissapear.
        // IE: The top byte of the frame gets removed, so does the frame
        stackHeightToStackFrameMap.insert(runtimeStack.size() - 1, graphicItemsInStackFrame.top());
    }
}

void MemoryTracePane::cacheHeapChanges()
{
    if (Sim::trapped) {
        return;
    }
    if (ui->warningLabel->text() != "") {
        ui->warningLabel->clear();
    }

    if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::CALL && Pep::symbolTable.value("malloc") == Sim::operandSpecifier) {
        qDebug() << "In cacheHeapChanges()";
        newestHeapItemsList.clear();
        int numCellsToAdd = 0;
        int offset = 0;
        int multiplier;
        QString heapSymbol;
        int heapPointer;
        if (Pep::symbolTable.contains("hpPtr")) {
            heapPointer = Pep::symbolTable.value("hpPtr");
        }
        else {
            // We have no idea where the heap pointer is. Error!
            ui->warningLabel->setText("Warning: hpPtr not found, unable to trace <code>CALL \'malloc\'</code>.");
            return;
        }
        int listNumBytes = 0;
        // Check and make sure the accumulator matches the number of bytes we're mallocing:
        // We'll start by adding up the number of bytes...
        qDebug() << "lookAheadSymbolList" << lookAheadSymbolList;
        for (int i = 0; i < lookAheadSymbolList.size(); i++) {
            qDebug() << "In loop. i == " << i;
            heapSymbol = lookAheadSymbolList.at(i);
            if (Pep::equateSymbols.contains(heapSymbol) || Pep::blockSymbols.contains(heapSymbol)) {
                // listNumBytes += number of bytes for that tag * the multiplier (IE, 2d4a is a 4 cell
                // array of 2 byte decimals, where 2 is the multiplier and 4 is the number of cells.
                // Note: the multiplier should always be 1 for malloc'd cells, but that's checked below, where we'll give a more specific error.
                listNumBytes += Asm::tagNumBytes(Pep::symbolFormat.value(heapSymbol)) * Pep::symbolFormatMultiplier.value(heapSymbol);
            }
        }
        if (listNumBytes != Sim::accumulator) {
            ui->warningLabel->setText("Warning: The accumulator doesn't match the number of bytes in the trace tags");
            return;
        }
        for (int i = 0; i < lookAheadSymbolList.size(); i++) {
            heapSymbol = lookAheadSymbolList.at(i);
            qDebug() << "heapSymbol = " << heapSymbol;
            if (Pep::equateSymbols.contains(heapSymbol) || Pep::blockSymbols.contains(heapSymbol)) {
                multiplier = Pep::symbolFormatMultiplier.value(heapSymbol);
            }
            else {
                ui->warningLabel->setText("Warning: Symbol \"" + heapSymbol + "\" not found in .equates, unknown size.");
                return;
            }
            if (multiplier == 1) { // We can't support arrays on the stack with our current addressing modes.
                // Very good! Have a cookie. Then, work! *cracks whip* (All our prereqs have been met to make an item)
                moveHeapUpOneCell();
                MemoryCellGraphicsItem *item = new MemoryCellGraphicsItem(Sim::readWord(heapPointer) + offset,
                                                                          heapSymbol,
                                                                          Pep::symbolFormat.value(heapSymbol),
                                                                          static_cast<int>(heapLocation.x()),
                                                                          static_cast<int>(heapLocation.y()));
                item->updateValue();
                isHeapItemAddedStack.push(false);
                heap.push(item);
                addressToHeapItemMap.insert(Sim::readWord(heapPointer) + offset, item);
                newestHeapItemsList.append(item);
                offset += Sim::cellSize(Pep::symbolFormat.value(heapSymbol));
                numCellsToAdd++;
            }
        }
        if (numCellsToAdd != 0) {
            addHeapFrame(numCellsToAdd);
        }
    }
}

void MemoryTracePane::highlightOnFocus()
{
    if (ui->graphicsView->hasFocus() || ui->spinBox->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool MemoryTracePane::hasFocus()
{
    return ui->graphicsView->hasFocus() || ui->spinBox->hasFocus();
}

void MemoryTracePane::setFont()
{
    // We might just do away with this in this pane.
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->graphicsView->font()), this, "Set Object Code Font");
    if (ok) {
        ui->graphicsView->setFont(font);
    }
}

void MemoryTracePane::setFocus()
{
    ui->graphicsView->setFocus();
}

void MemoryTracePane::addStackFrame(int numCells)
{
    QPen pen(Qt::black);
    pen.setWidth(4);
    QGraphicsRectItem *item = new QGraphicsRectItem(stackLocation.x() - 2, stackLocation.y() + MemoryCellGraphicsItem::boxHeight, 
                      static_cast<qreal>(MemoryCellGraphicsItem::boxWidth + 4),
                      static_cast<qreal>(MemoryCellGraphicsItem::boxHeight * numCells), 0);
    item->setPen(pen);
    graphicItemsInStackFrame.push(item);
    isStackFrameAddedStack.push(false);
    item->setZValue(1.0); // This moves the stack frame to the front
    numCellsInStackFrame.push(numCells);
}

void MemoryTracePane::addHeapFrame(int numCells)
{
    QPen pen(Qt::black);
    pen.setWidth(4);
    QGraphicsRectItem *item = new QGraphicsRectItem(heapLocation.x() - 2,
                                                    heapLocation.y() - MemoryCellGraphicsItem::boxHeight * (numCells - 1),
                      static_cast<qreal>(MemoryCellGraphicsItem::boxWidth + 4),
                      static_cast<qreal>(MemoryCellGraphicsItem::boxHeight * numCells), 0);
    item->setPen(pen);
    heapFrameItemStack.push(item);
    isHeapFrameAddedStack.push(false);
    item->setZValue(1.0); // This moves the heap frame to the front
}

void MemoryTracePane::moveHeapUpOneCell()
{
    for (int i = 0; i < heap.size(); i++) {
        heap.at(i)->moveBy(0, 0 - MemoryCellGraphicsItem::boxHeight);
    }
    for (int i = 0; i < heapFrameItemStack.size(); i++) {
        heapFrameItemStack.at(i)->moveBy(0, 0 - MemoryCellGraphicsItem::boxHeight);
    }
}

void MemoryTracePane::popBytes(int bytesToPop)
{
    while (bytesToPop > 0 && !runtimeStack.isEmpty()) {
        if (stackHeightToStackFrameMap.contains(runtimeStack.size() - 1)) {
            if (stackHeightToStackFrameMap.value(runtimeStack.size() - 1)->scene() == scene) {
                scene->removeItem(stackHeightToStackFrameMap.value(runtimeStack.size() - 1));
            }
            delete stackHeightToStackFrameMap.value(runtimeStack.size() - 1);
            graphicItemsInStackFrame.pop();
            stackHeightToStackFrameMap.remove(runtimeStack.size() - 1);
            isStackFrameAddedStack.pop();
            numCellsInStackFrame.pop();
        }
        
        if (runtimeStack.top()->scene() == scene) {
            scene->removeItem(runtimeStack.top());
        }
        addressToStackItemMap.remove(runtimeStack.top()->getAddress());
        bytesToPop -= runtimeStack.top()->getNumBytes();
        delete runtimeStack.top();
        runtimeStack.pop();
        isRuntimeStackItemAddedStack.pop();
        stackLocation.setY(stackLocation.y() + MemoryCellGraphicsItem::boxHeight);
    }
}

void MemoryTracePane::mouseReleaseEvent(QMouseEvent *)
{
    ui->graphicsView->setFocus();
}

void MemoryTracePane::zoomFactorChanged(int factor)
{
    QMatrix matrix;
    matrix.scale(factor * .01, factor * .01);
    ui->graphicsView->setMatrix(matrix);
}

void MemoryTracePane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::EMemoryTrace);
}

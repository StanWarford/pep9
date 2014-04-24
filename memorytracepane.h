// File: memorytracepane.h
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

#ifndef MEMORYTRACEPANE_H
#define MEMORYTRACEPANE_H

#include <QWidget>
#include <QGraphicsScene>
#include <QStack>
#include <QSet>
#include "memorycellgraphicsitem.h"
#include "enu.h"
#include "stackframefsm.h"

namespace Ui {
    class MemoryTracePane;
}

class MemoryTracePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(MemoryTracePane)
public:
    explicit MemoryTracePane(QWidget *parent = 0);
    virtual ~MemoryTracePane();

    void setMemoryTrace();
    // The memory trace is populated (on assembly).

    void updateMemoryTrace();
    // Post: The memory trace is updated

    void cacheChanges();
    // Post: Modfied bytes are cached for updating the sim view

    void cacheStackChanges();
    // Post: Stack changes are cached for the next time the simulation view is updated

    void cacheHeapChanges();
    // Post: Heap changes are cached for the next time the simulation view is updated

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

    void setFocus();
    // Post: the graphics item has focus
    
private:
    Ui::MemoryTracePane *ui;

    QGraphicsScene *scene;
    QStack<MemoryCellGraphicsItem *> globalVars;
    // Stack of the global variables
    QStack<MemoryCellGraphicsItem *> runtimeStack;    
    // Stack of the stack items
    QStack<MemoryCellGraphicsItem *> heap;
    // Stack of heap items
    QStack<bool> isRuntimeStackItemAddedStack;
    // Used to keep track if the item has been added to the scene yet for the runtime stack
    QStack<bool> isHeapItemAddedStack;
    // Used to keep track if the item has been added to the scene yet for the heap
    QStringList lookAheadSymbolList;
    // This is used to give us what we're pushing onto the stack before we get there.
    // It must be a look-ahead list because of branching and the inability to look behind

    // Stack frame
    QMap<int, QGraphicsRectItem *> stackHeightToStackFrameMap;
    QStack<int> numCellsInStackFrame;
    // This is a stack of ints that each represent how many cells each stack frame encompass
    QStack<bool> isStackFrameAddedStack;
    // Stack used to determine if a stack frame has been added to the scene yet
    QStack<QGraphicsRectItem *> graphicItemsInStackFrame;
    // Stack of *items used to access the stack frames
    QStack<bool> isHeapFrameAddedStack;
    // Stack used to determine if a heap frame has been added to the scene yet
    QStack<QGraphicsRectItem *> heapFrameItemStack;
    // Stack of *items for the heap graphic frames

    QPointF globalLocation;
    // This is the location where the next global item will be added
    QPointF stackLocation;
    // This is the location where the next stack item will be added
    QPointF heapLocation;
    // This is the location where the next heap item will be added

    QMap<int, MemoryCellGraphicsItem *> addressToGlobalItemMap;
    // This map is used to identify if an address is in the globals
    QMap<int, MemoryCellGraphicsItem *> addressToStackItemMap;
    // This map is used to identify if an address is part of the stack
    QMap<int, MemoryCellGraphicsItem *> addressToHeapItemMap;
    // Used to identify if an address is part of the heap
    QSet<int> modifiedBytes;
    // This set is used to cache modified bytes since the last update
    QList<int> bytesWrittenLastStep;
    // This list is used to keep track of the bytes changed last step for highlighting purposes
    bool delayLastStepClear;
    // This is used to delay the clear of the bytesWrittenLastStep list for purposes of highlighting after a trap

    QList<MemoryCellGraphicsItem *> newestHeapItemsList;
    // This is used to color the most recently new'd heap items light green

    StackFrameFSM stackFrameFSM;

    // Called by the cacheStack/HeapChanges functions to add frames to the respective places.
    // They're in their own functions because a fair bit happens there.
    void addStackFrame(int numCells);
    void addHeapFrame(int numCells);

    void moveHeapUpOneCell();
    // Moves the heap frame up n cells to accomodate for new items being added.

    void popBytes(int bytesToPop);
    // This pops bytesToPop bytes off of the runtimeStack

    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private slots:
    void zoomFactorChanged(int factor);

signals:
    void labelDoubleClicked(Enu::EPane pane);

};

#endif // MEMORYTRACEPANE_H

// File: objectcodepane.h
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

#ifndef OBJECTCODEPANE_H
#define OBJECTCODEPANE_H

#include <QWidget>
#include "enu.h"

namespace Ui {
    class ObjectCodePane;
}

class ObjectCodePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ObjectCodePane)
public:
    explicit ObjectCodePane(QWidget *parent = 0);
    virtual ~ObjectCodePane();

    void setObjectCode(QList<int> objectCode);
    // Post Sets text in source code pane.

    void setObjectCodePaneText(QString string);
    // Post: Sets text in source code pane to string.

    bool getObjectCode(QList<int> &objectCodeList);
    // Pre: The object code pane contains object code
    // Post: If the object code is syntactically correct, true is returned, and
    // &objectCodeList contains the object code, one byte per integer.
    // Otherwise, false is returned.

    void clearObjectCode();
    // Post: Clears the source code pane

    bool isModified();
    // Post: Returns true if the source code pane has been modified

    void setModifiedFalse();
    // Post: Sets isModified of the source code pane to false

    QString toPlainText();
    // Post: Contents of the source code pane are returned

    void setCurrentFile(QString string);
    // Post: Title bar of the window is set to "Object Code - 'string'"

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void undo();
    // Post: the last action in the text edit is undone

    void redo();
    // Post: the last undo in the text edit is redone

    bool isUndoable();
    // Returns the undoability of the text edit

    bool isRedoable();
    // Returns the redoability of the text edit

    void cut();
    // Post: selected text in the text edit is cut to the clipboard

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void paste();
    // Post: selected text in the clipboard is pasted to the text edit

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

    void setReadOnly(bool b);
    // Post: the text edit's read only attribute is set to b

private:
    Ui::ObjectCodePane *ui;

    void mouseReleaseEvent(QMouseEvent *);

    void mouseDoubleClickEvent(QMouseEvent *);

private slots:
    void setLabelToModified(bool modified);

signals:
    void undoAvailable(bool);
    void redoAvailable(bool);

    void labelDoubleClicked(Enu::EPane pane);
};

#endif // OBJECTCODEPANE_H

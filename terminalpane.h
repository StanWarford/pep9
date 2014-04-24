// File: terminalpane.h
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

#ifndef TERMINALPANE_H
#define TERMINALPANE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
    class TerminalPane;
}

class TerminalPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(TerminalPane)
public:
    explicit TerminalPane(QWidget *parent = 0);
    virtual ~TerminalPane();

    void appendOutput(QString str);
    // Post: str is appended to the text edit

    void waitingForInput();
    // Post: Sets the writability of the text edit to true, and prevents previously entered text from being modified

    void clearTerminal();
    // Post: Clears the terminal

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void copy();
    // Post: selected text in the text edit is copied to the clipboard

    void setFont();
    // Post: the font used by the text edit is set to a font chosen in a font dialog

private:
    Ui::TerminalPane *ui;

    bool waiting;

    QString strokeString;
    QString retString;

    void displayTerminal();
    
    bool eventFilter(QObject *, QEvent *event);

private slots:
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void undoAvailable(bool);
    void redoAvailable(bool);
    void inputReceived();

};

#endif // TERMINALPANE_H

// File: listingtracepane.h
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

#ifndef LISTINGTRACEPANE_H
#define LISTINGTRACEPANE_H

#include <QWidget>
#include <QTableWidgetItem>
#include "enu.h"

namespace Ui {
    class ListingTracePane;
}

class ListingTracePane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ListingTracePane)
public:
    explicit ListingTracePane(QWidget *parent = 0);
    virtual ~ListingTracePane();

    void setListingTrace(QStringList listingTraceList, QList<bool> hasCheckBox);
    // Post: Populate listingTraceTable from assemblerListingList and hasCheckBox

    void clearListingTrace();

    void updateListingTrace();
    // Post: the listing trace pane selection is updated

    void setDebuggingState(bool b);
    // Post: resume and single step buttons clickability is set to b
    // Also clears the selection

    void showAssemblerListing();
    // Post: The tableWidget containing the assembler listing is shown
    // and the OS tableWidget is hidden

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    bool hasFocus();
    // Post: returns if the pane has focus

    void setFocus();
    // Post: gives the text edit focus
    
//    void resizeDocWidth();
    // Post: the document widths of the trace panes are set
    // This is commented, but preserved in case we want to bring back the resizing of the document width to the width of the window.

public slots:
    void onFontChanged(QFont font);

private:
    Ui::ListingTracePane *ui;

    void mouseReleaseEvent(QMouseEvent *);
    
    QList<QTableWidgetItem *> highlightedItemList;

//    int programDocWidth;
//    int osDocWidth;
    // These are commented, but preserved in case we want to bring back the resizing of the document width to the width of the window.
    void mouseDoubleClickEvent(QMouseEvent *);

private slots:
    void updateIsCheckedTable(QTableWidgetItem *item);

signals:
    void labelDoubleClicked(Enu::EPane pane);

};

#endif // LISTINGTRACEPANE_H

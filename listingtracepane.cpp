// File: listingtracepane.cpp
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
#include <QScrollBar>
#include "listingtracepane.h"
#include "ui_listingtracepane.h"
#include "sim.h"
#include "pep.h"

// #include <QDebug>

ListingTracePane::ListingTracePane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListingTracePane)
{
    ui->setupUi(this);

    ui->listingPepOsTraceTableWidget->hide();

    connect(ui->listingTraceTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(updateIsCheckedTable(QTableWidgetItem*)));
    connect(ui->listingPepOsTraceTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(updateIsCheckedTable(QTableWidgetItem*)));

//    programDocWidth = 0;
//    osDocWidth = 0;

    ui->label->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
    ui->listingPepOsTraceTableWidget->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
    ui->listingTraceTableWidget->setFont(QFont(Pep::codeFont, Pep::codeFontSize));
}

ListingTracePane::~ListingTracePane()
{
    delete ui;
}

void ListingTracePane::setListingTrace(QStringList listingTraceList, QList<bool> hasCheckBox)
{
    // tableWidget depends on whether we are assembling the OS or a program
    QTableWidget *tableWidget;
    if (Pep::memAddrssToAssemblerListing == &Pep::memAddrssToAssemblerListingProg) {
        tableWidget = ui->listingTraceTableWidget;
    }
    else {
        tableWidget = ui->listingPepOsTraceTableWidget;
    }
    QTableWidgetItem *item;
    int numRows = listingTraceList.size();
    tableWidget->setRowCount(numRows);
    for (int i = 0; i < numRows; i++) {
        item = new QTableWidgetItem(listingTraceList[i]);
        tableWidget->setItem(i, 1, item);
    }
    for (int i = 0; i < numRows; i++) {
        item = new QTableWidgetItem();
        if (hasCheckBox[i]) {
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
        }
        else {
            item->setFlags(Qt::NoItemFlags);
        }
        tableWidget->setItem(i, 0, item);
    }
    tableWidget->resizeColumnsToContents();
    ui->listingTraceTableWidget->width();
    tableWidget->resizeRowsToContents();
//    if (Pep::memAddrssToAssemblerListing == &Pep::memAddrssToAssemblerListingProg) {
//        programDocWidth = tableWidget->columnWidth(1);
//    }
//    else {
//        osDocWidth = tableWidget->columnWidth(1);
//    }
//    resizeDocWidth();
    tableWidget->horizontalScrollBar()->setValue(tableWidget->horizontalScrollBar()->minimum());
}

void ListingTracePane::clearListingTrace()
{
    for (int i = 0; i < ui->listingTraceTableWidget->rowCount(); i++) {
        delete ui->listingTraceTableWidget->itemAt(i, 0);
        delete ui->listingTraceTableWidget->itemAt(i, 1);
    }
    ui->listingTraceTableWidget->setRowCount(0);
}

void ListingTracePane::updateListingTrace()
{
    // tableWidget depends on whether we are in the OS or a program
    QTableWidget *tableWidget;
    if (Sim::trapped) {
        tableWidget = ui->listingPepOsTraceTableWidget;
        ui->listingPepOsTraceTableWidget->show();
        ui->listingTraceTableWidget->hide();
    }
    else {
        tableWidget = ui->listingTraceTableWidget;
        ui->listingPepOsTraceTableWidget->hide();
        ui->listingTraceTableWidget->show();
    }

    for (int i = highlightedItemList.size() - 1; i >= 0; i--) {
        highlightedItemList.at(i)->setBackgroundColor(Qt::white);
        highlightedItemList.at(i)->setTextColor(Qt::black);
        highlightedItemList.removeLast();
    }
    if (Pep::memAddrssToAssemblerListing->contains(Sim::programCounter)) {
        QTableWidgetItem *highlightedItem = tableWidget->item(Pep::memAddrssToAssemblerListing->value(Sim::programCounter), 1);
        highlightedItem->setBackgroundColor(QColor(56, 117, 215));
        highlightedItem->setTextColor(Qt::white);
        highlightedItemList.append(highlightedItem);

        tableWidget->scrollToItem(highlightedItem);
    }
    tableWidget->horizontalScrollBar()->setValue(tableWidget->horizontalScrollBar()->minimum());
}

void ListingTracePane::setDebuggingState(bool b)
{
    QTableWidget *tableWidget;
    if (Sim::trapped) {
        tableWidget = ui->listingPepOsTraceTableWidget;
        ui->listingPepOsTraceTableWidget->show();
        ui->listingTraceTableWidget->hide();
    }
    else {
        tableWidget = ui->listingTraceTableWidget;
        ui->listingPepOsTraceTableWidget->hide();
        ui->listingTraceTableWidget->show();
    }

    for (int i = 0; i < tableWidget->rowCount(); i++) {
        tableWidget->item(i, 1)->setBackgroundColor(Qt::white);
        tableWidget->item(i, 1)->setTextColor(Qt::black);
    }
    highlightedItemList.clear();
    
    if (b && Pep::memAddrssToAssemblerListing->contains(Sim::programCounter)) {
        QTableWidgetItem *highlightedItem = tableWidget->item(Pep::memAddrssToAssemblerListing->value(Sim::programCounter), 1);
        highlightedItem->setBackgroundColor(QColor(56, 117, 215));
        highlightedItem->setTextColor(Qt::white);
        highlightedItemList.append(highlightedItem);

        tableWidget->scrollToItem(highlightedItem);
    }
    tableWidget->horizontalScrollBar()->setValue(tableWidget->horizontalScrollBar()->minimum());
//    resizeDocWidth();
}

void ListingTracePane::showAssemblerListing()
{
    ui->listingPepOsTraceTableWidget->hide();
    ui->listingTraceTableWidget->show();
}

void ListingTracePane::highlightOnFocus()
{
    if (ui->listingTraceTableWidget->hasFocus() || ui->listingPepOsTraceTableWidget->hasFocus()) {
        ui->label->setAutoFillBackground(true);
    }
    else {
        ui->label->setAutoFillBackground(false);
    }
}

bool ListingTracePane::hasFocus()
{
    return ui->listingTraceTableWidget->hasFocus() || ui->listingPepOsTraceTableWidget->hasFocus();
}

void ListingTracePane::setFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, QFont(ui->listingTraceTableWidget->font()), this, "Set Listing Trace Font");
    if (ok) {
        ui->listingTraceTableWidget->setFont(font);
        ui->listingPepOsTraceTableWidget->setFont(font);
        ui->listingTraceTableWidget->resizeColumnsToContents();
        ui->listingPepOsTraceTableWidget->resizeColumnsToContents();
    }
}

void ListingTracePane::setFocus()
{
    ui->listingTraceTableWidget->isHidden() ? ui->listingPepOsTraceTableWidget->setFocus() : ui->listingTraceTableWidget->setFocus();
}

//void ListingTracePane::resizeDocWidth()
//{
//    int column0 = ui->listingTraceTableWidget->columnWidth(0);
//    if (ui->listingTraceTableWidget->width() > programDocWidth + column0 + 5) { // + 5 for breathing room
//        ui->listingTraceTableWidget->setColumnWidth(1, ui->listingTraceTableWidget->viewport()->width() - column0);
//    }
//    else {
//        ui->listingTraceTableWidget->setColumnWidth(1, programDocWidth);
//    }
//    if (ui->listingPepOsTraceTableWidget->width() > osDocWidth + column0 + 5) { // + 5 for breathing room
//        ui->listingPepOsTraceTableWidget->setColumnWidth(1, ui->listingPepOsTraceTableWidget->viewport()->width() - column0);
//    }
//    else {
//        ui->listingPepOsTraceTableWidget->setColumnWidth(1, osDocWidth);
//    }
//}

void ListingTracePane::mouseReleaseEvent(QMouseEvent *)
{
    QTableWidget *tableWidget;
    if (!ui->listingTraceTableWidget->isHidden()) {
        tableWidget = ui->listingTraceTableWidget;
    }
    else {
        tableWidget = ui->listingPepOsTraceTableWidget;
    }
    tableWidget->setFocus();
}

void ListingTracePane::updateIsCheckedTable(QTableWidgetItem *item)
{
    Pep::listingRowChecked->insert(item->row(), item->checkState());
}

void ListingTracePane::mouseDoubleClickEvent(QMouseEvent *)
{
    emit labelDoubleClicked(Enu::EListingTrace);
}

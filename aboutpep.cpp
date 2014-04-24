// File: aboutpep.cpp
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
#include "aboutpep.h"
#include "ui_aboutpep.h"
#include "pep.h"

AboutPep::AboutPep(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutPep)
{
    ui->setupUi(this);
    if (Pep::getSystem() != "Mac") {
        ui->aboutLabel->hide();
    }
    else {
        ui->aboutLabelWin->hide();
    }

}

AboutPep::~AboutPep()
{
    delete ui;
}

// StackFrameFSM.h: interface for the StackFrameFSM class
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

#ifndef STACK_FRAME_FSM_H 
#define STACK_FRAME_FSM_H 

#include <QObject>
#include "enu.h"

class StackFrameFSM: public QObject  
{
public:
	StackFrameFSM();

	void reset(); // set everything to 0 and start state
        int makeTransition(int numCellsToAdd);
	// if the frame is ready to add, returns the number of cells
	// otherwise returns 0

private:
        enum { EStart, ESubSP, ECall } stackState;

	int numCellsFromSubSP; // number of cells from the first SUBSP (i.e. parameters and retVal)
	int numCellsFromCall;
};

#endif // STACK_FRAME_FSM_H

// StackFrameFSM.cpp: implementation of the StackFrameFSM class.
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

#include "stackframefsm.h"
#include "sim.h"
#include "pep.h"

// #include <QDebug>

StackFrameFSM::StackFrameFSM()
{

}

void StackFrameFSM::reset()
{
    stackState = EStart;
    numCellsFromSubSP = 0;
    numCellsFromCall = 0;
}

// the states: EStart, ESubSP, ECall

int StackFrameFSM::makeTransition(int numCellsToAdd)
{
    Enu::EMnemonic mnemon = Pep::decodeMnemonic[Sim::instructionSpecifier];

    switch(stackState)
    {
    case(EStart):
        if (mnemon == Enu::SUBSP) {
            numCellsFromSubSP = numCellsToAdd;
            stackState = ESubSP;
        }
        else if (mnemon == Enu::CALL) {
            numCellsFromSubSP = 0;
            numCellsFromCall = 1; // = numCellsToAdd; // ECall = 1
            stackState = ECall;
        }
        break;
    case(ESubSP):
        if (mnemon == Enu::CALL) {
            numCellsFromCall = 1; // = numCellsToAdd; // ECall = 1
            stackState = ECall;
        }
        else { // not ECall
            stackState = EStart;
            return numCellsFromSubSP; // lone subsp
        }
        break;
    case(ECall):
        if (mnemon == Enu::SUBSP) { // function with 1 or more locals and 0 or more parameters
            stackState = EStart;
            return (numCellsFromSubSP + numCellsFromCall + numCellsToAdd);
        }
        else { // not ESubSP
            stackState = EStart; // no locals
            return (numCellsFromSubSP + numCellsFromCall);
        }
        break;
    default:;
    }
    return 0; // don't add a frame yet
}

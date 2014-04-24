// File: cpupane.h
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

#ifndef CPUPANE_H
#define CPUPANE_H

#include <QWidget>
#include "enu.h"

namespace Ui {
    class CpuPane;
}

class CpuPane : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(CpuPane)
public:
    explicit CpuPane(QWidget *parent = 0);
    virtual ~CpuPane();

    void updateCpu();
    // Post: Updates CPU pane labels

    void clearCpu();
    // Post: The CPU pane labels are blanked and the CPU registers are cleared

    void runClicked();
    // Post: All of the debug checkboxes are unclicked

    void setDebugState(bool b);
    // Post: if b is true, checkboxes are set to disabled, and vice versa

    void traceTraps(bool b);
    // Post: Trace traps checkbox is checked if b, unchecked else

    void setButtonsEnabled(bool b);
    // Post: if b is true, buttons are enabled, and vice versa

    void runWithBatch();
    // Runs the simulator through with batch input

    void runWithTerminal();
    // Runs the simulator through with terminal i/o

    void resumeWithBatch();
    // Resumes the simulator with batch input

    void resumeWithTerminal();
    // Resumes the simulator with terminal input

    void singleStepWithBatch();
    // Single steps the simulator with batch input

    void singleStepWithTerminal();
    // Single steps the simulator with terminal input

    void trapLookahead();
    // Looks ahead to the next instruction to determine if we are trapping

    void interruptExecution();
    // Post: interruptExecutionFlag is set to true

    void highlightOnFocus();
    // Post: Highlights the label based on the label window color saved in the UI file

    Enu::EWaiting waitingState();
    // Post: Returns the waiting state of the simulation (for terminal io)

    bool singleStepHasFocus();
    // Post: returns if the single step is enabled

    bool hasFocus();
    // Post: Returns if the single step button has focus

    bool isSimulating();
    // Returns if the CPU is currently simulating - this can happen when the enter key is
    // held down to single step quickly, causing multiple enter events to fire per single step
    // This is used to avoid that.

    void giveSingleStepFocus();
    // Gives the single step button focus - this is used when we press 'enter' in other panes
    // and we want to give the cpu pane focus for ease of navigation

private:
    Ui::CpuPane *ui;

    bool interruptExecutionFlag; // Used to interrupt execution by the user

    Enu::EWaiting waiting; // Used to store terminal IO waiting for input state

    void mousePressEvent(QMouseEvent *);

    bool isCurrentlySimulating;

private slots:
    void singleStepButton();

signals:
    void resumeButtonClicked();
    void singleStepButtonClicked();
    void updateSimulationView();
    void executionComplete();
    void appendOutput(QString);
    void vonNeumannStepped();
    void waitingForInput();
};

#endif // CPUPANE_H

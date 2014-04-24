// File: cpupane.cpp
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
#include <QMessageBox>
#include <QKeyEvent>
// #include <QSound>
#include "cpupane.h"
#include "ui_cpupane.h"
#include "sim.h"
#include "pep.h"
#include <QtGlobal>

CpuPane::CpuPane(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CpuPane)
{
    ui->setupUi(this);

    connect(ui->singleStepPushButton, SIGNAL(clicked()), this, SLOT(singleStepButton()));
    connect(ui->resumePushButton, SIGNAL(clicked()), this, SIGNAL(resumeButtonClicked()));

    interruptExecutionFlag = false;
    clearCpu();

    if (Pep::getSystem() != "Mac") {
        ui->cpuLabel->setFont(QFont(Pep::labelFont, Pep::labelFontSize));
        ui->traceTrapsCheckBox->setFont(QFont(Pep::labelFont));

        ui->accLabel->setFont(QFont(Pep::labelFont));
        ui->xLabel->setFont(QFont(Pep::labelFont));
        ui->spLabel->setFont(QFont(Pep::labelFont));
        ui->pcLabel->setFont(QFont(Pep::labelFont));
        ui->instrSpecLabel->setFont(QFont(Pep::labelFont));
        ui->oprndSpecLabel->setFont(QFont(Pep::labelFont));
        ui->oprndLabel->setFont(QFont(Pep::labelFont));
        ui->singleStepPushButton->setFont(QFont(Pep::labelFont));
        ui->resumePushButton->setFont(QFont(Pep::labelFont));

        ui->pepNLabel->setFont(QFont(Pep::labelFont));
        ui->pepZLabel->setFont(QFont(Pep::labelFont));
        ui->pepVLabel->setFont(QFont(Pep::labelFont));
        ui->pepCLabel->setFont(QFont(Pep::labelFont));

        ui->nLabel->setFont(QFont(Pep::labelFont));
        ui->zLabel->setFont(QFont(Pep::labelFont));
        ui->vLabel->setFont(QFont(Pep::labelFont));
        ui->cLabel->setFont(QFont(Pep::labelFont));

        ui->accHexLabel->setFont(QFont(Pep::labelFont));
        ui->accDecLabel->setFont(QFont(Pep::labelFont));

        ui->xHexLabel->setFont(QFont(Pep::labelFont));
        ui->xDecLabel->setFont(QFont(Pep::labelFont));

        ui->spHexLabel->setFont(QFont(Pep::labelFont));
        ui->spDecLabel->setFont(QFont(Pep::labelFont));

        ui->pcHexLabel->setFont(QFont(Pep::labelFont));
        ui->pcDecLabel->setFont(QFont(Pep::labelFont));

        ui->instrSpecBinLabel->setFont(QFont(Pep::labelFont));
        ui->instrSpecMnemonLabel->setFont(QFont(Pep::labelFont));

        ui->oprndSpecHexLabel->setFont(QFont(Pep::labelFont));
        ui->oprndSpecDecLabel->setFont(QFont(Pep::labelFont));
        ui->oprndHexLabel->setFont(QFont(Pep::labelFont));
        ui->oprndDecLabel->setFont(QFont(Pep::labelFont));
    }
    isCurrentlySimulating = false;
}

CpuPane::~CpuPane()
{
    delete ui;
}

void CpuPane::updateCpu() {
    Enu::EAddrMode addrMode = Pep::decodeAddrMode[Sim::instructionSpecifier];

    ui->nLabel->setText(Sim::nBit ? "1" : "0");
    ui->zLabel->setText(Sim::zBit ? "1" : "0");
    ui->vLabel->setText(Sim::vBit ? "1" : "0");
    ui->cLabel->setText(Sim::cBit ? "1" : "0");

    ui->accHexLabel->setText(QString("0x") + QString("%1").arg(Sim::accumulator, 4, 16, QLatin1Char('0')).toUpper());
    ui->accDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::accumulator)));

    ui->xHexLabel->setText(QString("0x") + QString("%1").arg(Sim::indexRegister, 4, 16, QLatin1Char('0')).toUpper());
    ui->xDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::indexRegister)));

    ui->spHexLabel->setText(QString("0x") + QString("%1").arg(Sim::stackPointer, 4, 16, QLatin1Char('0')).toUpper());
    ui->spDecLabel->setText(QString("%1").arg(Sim::stackPointer));

    ui->pcHexLabel->setText(QString("0x") + QString("%1").arg(Sim::programCounter, 4, 16, QLatin1Char('0')).toUpper());
    ui->pcDecLabel->setText(QString("%1").arg(Sim::programCounter));

    ui->instrSpecBinLabel->setText(QString("%1").arg(Sim::instructionSpecifier, 8, 2, QLatin1Char('0')).toUpper());
    ui->instrSpecMnemonLabel->setText(" " + Pep::enumToMnemonMap.value(Pep::decodeMnemonic[Sim::instructionSpecifier])
                                           + Pep::commaSpaceToAddrMode(addrMode));

    if (Pep::decodeAddrMode.value(Sim::instructionSpecifier) == Enu::NONE) {
        ui->oprndSpecHexLabel->setText("");
        ui->oprndSpecDecLabel->setText("");
        ui->oprndHexLabel->setText("");
        ui->oprndDecLabel->setText("");
    }
    else {
        ui->oprndSpecHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operandSpecifier, 4, 16, QLatin1Char('0')).toUpper());
        ui->oprndSpecDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::operandSpecifier)));
        ui->oprndHexLabel->setText(QString("0x") + QString("%1").arg(Sim::operand, Sim::operandDisplayFieldWidth, 16, QLatin1Char('0')).toUpper());
        ui->oprndDecLabel->setText(QString("%1").arg(Sim::toSignedDecimal(Sim::operand)));
    }
}

void CpuPane::clearCpu()
{
    ui->nLabel->setText("");
    ui->zLabel->setText("");
    ui->vLabel->setText("");
    ui->cLabel->setText("");

    ui->accHexLabel->setText("");
    ui->accDecLabel->setText("");

    ui->xHexLabel->setText("");
    ui->xDecLabel->setText("");

    ui->spHexLabel->setText("");
    ui->spDecLabel->setText("");

    ui->pcHexLabel->setText("");
    ui->pcDecLabel->setText("");

    ui->instrSpecBinLabel->setText("");
    ui->instrSpecMnemonLabel->setText("");

    ui->oprndSpecHexLabel->setText("");
    ui->oprndSpecDecLabel->setText("");
    ui->oprndHexLabel->setText("");
    ui->oprndDecLabel->setText("");

    Sim::nBit = false;
    Sim::zBit = false;
    Sim::vBit = false;
    Sim::cBit = false;

    Sim::accumulator = 0;
    Sim::indexRegister = 0;
    Sim::stackPointer = 0; // Sim::readWord(Pep::dotBurnArgument - 7);
    Sim::programCounter = 0;
}

void CpuPane::runClicked() {
    ui->traceTrapsCheckBox->setChecked(false);
}

void CpuPane::setDebugState(bool b)
{
    ui->traceTrapsCheckBox->setDisabled(b);
    Sim::tracingTraps = ui->traceTrapsCheckBox->isChecked();
}

void CpuPane::traceTraps(bool b)
{
    ui->traceTrapsCheckBox->setChecked(b);
}

void CpuPane::setButtonsEnabled(bool b) {
    ui->resumePushButton->setDisabled(!b);
    ui->singleStepPushButton->setDisabled(!b);
    if (b) {
        ui->singleStepPushButton->setFocus();
    }
}

void CpuPane::runWithBatch()
{
    isCurrentlySimulating = true;
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
        }
        else {
            QMessageBox::warning(0, "Pep/9", errorString);
            updateCpu();
            emit executionComplete();
            isCurrentlySimulating = false;
            return;
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
            updateCpu();
            emit executionComplete();
            isCurrentlySimulating = false;
            return;
        }
        if (interruptExecutionFlag) {
            updateCpu();
            isCurrentlySimulating = false;
            return;
        }
    }
}

void CpuPane::runWithTerminal()
{
    isCurrentlySimulating = true;
    waiting = Enu::ERunWaiting;
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
            // we are waiting for input
            updateCpu();
            emit waitingForInput();
            isCurrentlySimulating = false;
            return;
        }
        else {
            if (Sim::vonNeumannStep(errorString)) {
                emit vonNeumannStepped();
                if (Sim::outputBuffer.length() == 1) {
                    emit appendOutput(Sim::outputBuffer);
                    Sim::outputBuffer = "";
                }
            }
            else {
                QMessageBox::warning(0, "Pep/9", errorString);
                updateCpu();
                emit executionComplete();
                isCurrentlySimulating = false;
                return;
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                updateCpu();
                emit executionComplete();
                isCurrentlySimulating = false;
                return;
            }
            if (interruptExecutionFlag) {
                updateCpu();
                emit updateSimulationView();
                isCurrentlySimulating = false;
                return;
            }
        }
    }
}

void CpuPane::resumeWithBatch()
{
    isCurrentlySimulating = true;
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        if (ui->traceTrapsCheckBox->isChecked()) {
            trapLookahead();
        }
        else if (Pep::isTrapMap[Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]]) {
            Sim::trapped = true;
        }
        else if (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::RETTR) {
            Sim::trapped = false;
        }
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                emit updateSimulationView();
                emit executionComplete();
                isCurrentlySimulating = false;
                return;
            }
            if (Pep::memAddrssToAssemblerListing->contains(Sim::programCounter) &&
                Pep::listingRowChecked->value(Pep::memAddrssToAssemblerListing->value(Sim::programCounter)) == Qt::Checked) {
                updateCpu();
                emit updateSimulationView();
                return;
            }
        }
        else {
            QMessageBox::warning(0, "Pep/9", errorString);
            updateCpu();
            emit updateSimulationView();
            isCurrentlySimulating = false;
            emit executionComplete();
        }
        if (interruptExecutionFlag) {
            emit updateSimulationView();
            isCurrentlySimulating = false;
            return;
        }
    }
}

void CpuPane::resumeWithTerminal()
{
    isCurrentlySimulating = true;
    waiting = Enu::EDebugResumeWaiting;
    interruptExecutionFlag = false;
    QString errorString;
    while (true) {
        qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
        trapLookahead();
        if (Sim::trapped && !ui->traceTrapsCheckBox->isChecked()) {
            updateCpu();
            do {
                trapLookahead();
                qApp->processEvents(); // To make sure that the event filter gets to handle keypresses during the run
                if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
                    // we are waiting for input
                    ui->singleStepPushButton->setDisabled(true);
                    ui->resumePushButton->setDisabled(true);
                    emit waitingForInput();
                    isCurrentlySimulating = false;
                    return;
                }
                else {
                    if (Sim::vonNeumannStep(errorString)) {
                        emit vonNeumannStepped();
                        if (Sim::outputBuffer.length() == 1) {
                            emit appendOutput(Sim::outputBuffer);
                            Sim::outputBuffer = "";
                        }
                        if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                            emit updateSimulationView();
                            emit executionComplete();
                        }
                    }
                    else {
                        QMessageBox::warning(0, "Pep/9", errorString);
                        emit updateSimulationView();
                        emit executionComplete();
                        isCurrentlySimulating = false;
                    }
                }
                if (interruptExecutionFlag) {
                    updateCpu();
                    emit updateSimulationView();
                    isCurrentlySimulating = false;
                    return;
                }
            } while (Sim::trapped);
        }
        else if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
            // we are waiting for input
            ui->singleStepPushButton->setDisabled(true);
            ui->resumePushButton->setDisabled(true);
            emit updateSimulationView();
            updateCpu();
            emit waitingForInput();
            isCurrentlySimulating = false;
            return;
        }
        else {
            if (Sim::vonNeumannStep(errorString)) {
                emit vonNeumannStepped();
                if (Sim::outputBuffer.length() == 1) {
                    emit appendOutput(Sim::outputBuffer);
                    Sim::outputBuffer = "";
                }
                if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                    emit updateSimulationView(); // Finish updating the memory before we're done executing
                    emit executionComplete();
                    isCurrentlySimulating = false;
                    return;
                }
                if (Pep::memAddrssToAssemblerListing->contains(Sim::programCounter) &&
                    Pep::listingRowChecked->value(Pep::memAddrssToAssemblerListing->value(Sim::programCounter)) == Qt::Checked) {
                    updateCpu();
                    emit updateSimulationView();
                    isCurrentlySimulating = false;
                    return;
                }
            }
            else {
                QMessageBox::warning(0, "Pep/9", errorString);
                updateCpu();
                emit updateSimulationView();
                emit executionComplete();
                isCurrentlySimulating = false;
#warning "should we return here?"
            }
        }
        if (interruptExecutionFlag) {
            isCurrentlySimulating = false;
            return;
        }
    }
}

void CpuPane::singleStepWithBatch()
{
    isCurrentlySimulating = true;
    interruptExecutionFlag = false;
    QString errorString;
    trapLookahead();
    if (Sim::trapped && !ui->traceTrapsCheckBox->isChecked()) {
        QString errorString;
        do {
            trapLookahead();
            qApp->processEvents();
            if (Sim::vonNeumannStep(errorString)) {
                emit vonNeumannStepped();
                if (Sim::outputBuffer.length() == 1) {
                    emit appendOutput(Sim::outputBuffer);
                    Sim::outputBuffer = "";
                }
                if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                    emit updateSimulationView();
                    emit executionComplete();
                    isCurrentlySimulating = false;
                }
            }
            else {
                QMessageBox::warning(0, "Pep/9", errorString);
                emit updateSimulationView();
                emit executionComplete();
                isCurrentlySimulating = false;
            }
            if (interruptExecutionFlag) {
                updateCpu();
                emit updateSimulationView();
                isCurrentlySimulating = false;
                return;
            }
        } while (Sim::trapped);
        emit updateSimulationView();
        updateCpu();
    }
    else if (Sim::vonNeumannStep(errorString)) {
        emit vonNeumannStepped();
        emit updateSimulationView();
        if (Sim::outputBuffer.length() == 1) {
            emit appendOutput(Sim::outputBuffer);
            Sim::outputBuffer = "";
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
            updateCpu();
        }
        else {
            isCurrentlySimulating = false;
            emit executionComplete();
        }
    }
    else {
        QMessageBox::warning(0, "Pep/9", errorString);
        emit executionComplete();
    }
    isCurrentlySimulating = false;
}

void CpuPane::singleStepWithTerminal()
{
    isCurrentlySimulating = true;
    interruptExecutionFlag = false;
    QString errorString;
    waiting = Enu::EDebugSSWaiting;
    trapLookahead();
    if (Sim::trapped && !ui->traceTrapsCheckBox->isChecked()) {
        updateCpu();
        do {
            trapLookahead();
            qApp->processEvents();
            if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
                // we are waiting for input
                ui->singleStepPushButton->setDisabled(true);
                ui->resumePushButton->setDisabled(true);
                emit waitingForInput();
                isCurrentlySimulating = false;
                return;
            }
            else {
                if (Sim::vonNeumannStep(errorString)) {
                    emit vonNeumannStepped();
                    if (Sim::outputBuffer.length() == 1) {
                        emit appendOutput(Sim::outputBuffer);
                        Sim::outputBuffer = "";
                    }
                    if (Pep::decodeMnemonic[Sim::instructionSpecifier] == Enu::STOP) {
                        emit updateSimulationView();
                        emit executionComplete();
                        isCurrentlySimulating = false;
                    }
                }
                else {
                    QMessageBox::warning(0, "Pep/9", errorString);
                    emit updateSimulationView();
                    emit executionComplete();
                    isCurrentlySimulating = false;
                }
            }
            if (interruptExecutionFlag) {
                updateCpu();
                emit updateSimulationView();
                isCurrentlySimulating = false;
                return;
            }
        } while (Sim::trapped);
        emit updateSimulationView();
    }
    else if ((Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::CHARI) && Sim::inputBuffer.isEmpty()) {
        ui->singleStepPushButton->setDisabled(true);
        ui->resumePushButton->setDisabled(true);
        emit waitingForInput();
        isCurrentlySimulating = false;
    }
    else {
        if (Sim::vonNeumannStep(errorString)) {
            emit vonNeumannStepped();
            emit updateSimulationView();
            if (Sim::outputBuffer.length() == 1) {
                emit appendOutput(Sim::outputBuffer);
                Sim::outputBuffer = "";
            }
            if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
                updateCpu();
            }
            else {
                emit executionComplete();
            }
        }
        else {
            QMessageBox::warning(0, "Pep/9", errorString);
            emit executionComplete();
            return;
        }
        if (Pep::decodeMnemonic[Sim::instructionSpecifier] != Enu::STOP) {
            updateCpu();
        }
        else {
            emit executionComplete();
        }
    }
    isCurrentlySimulating = false;
}

void CpuPane::trapLookahead()
{
    if (Pep::isTrapMap[Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)]]) {
        Sim::trapped = true;
        Pep::memAddrssToAssemblerListing = &Pep::memAddrssToAssemblerListingOS;
        Pep::listingRowChecked = &Pep::listingRowCheckedOS;
    }
    else if (Pep::decodeMnemonic[Sim::readByte(Sim::programCounter)] == Enu::RETTR) {
        Sim::trapped = false;
        Pep::memAddrssToAssemblerListing = &Pep::memAddrssToAssemblerListingProg;
        Pep::listingRowChecked = &Pep::listingRowCheckedProg;
    }
}

void CpuPane::interruptExecution()
{
    interruptExecutionFlag = true;
}

void CpuPane::highlightOnFocus()
{
    if (ui->singleStepPushButton->hasFocus()) {
        ui->cpuLabel->setAutoFillBackground(true);
    }
    else {
        ui->cpuLabel->setAutoFillBackground(false);
    }
}

Enu::EWaiting CpuPane::waitingState()
{
    return waiting;
}

bool CpuPane::singleStepHasFocus()
{
    return ui->singleStepPushButton->isEnabled() && ui->singleStepPushButton->hasFocus();
}

bool CpuPane::hasFocus()
{
    return ui->singleStepPushButton->hasFocus();
}

bool CpuPane::isSimulating()
{
    return isCurrentlySimulating;
}

void CpuPane::giveSingleStepFocus()
{
    // protect against this being used incorrectly:
    if (ui->singleStepPushButton->isEnabled()) {
        ui->singleStepPushButton->setFocus();
    }
}

void CpuPane::mousePressEvent(QMouseEvent *)
{
    if (ui->singleStepPushButton->isEnabled()) {
        ui->singleStepPushButton->setFocus();
    }
}

void CpuPane::singleStepButton()
{
    ui->singleStepPushButton->setFocus();
    emit singleStepButtonClicked();
}

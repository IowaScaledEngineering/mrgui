/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     window.cpp
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2014 Michael Petersen & Nathan Holmes

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/

#include <stdint.h>

#include <QtWidgets>
#include <QDebug>

#include "window.h"
#include "avrinfo.h"
#include "intelhexmem.h"

Window::Window(const char *device)
{
#if defined(__APPLE__)
	strncpy(avrdudePath, "./bin/mac/avrdude", sizeof(avrdudePath));
#else
	strncpy(avrdudePath, "./bin/linux/avrdude", sizeof(avrdudePath));
#endif

	strncpy(avrdudeConfPath, "./bin/avrdude.conf", sizeof(avrdudeConfPath));

	avrDevice = device;
	eeprom = (uint8_t*)malloc(getAVRInfo(avrDevice)->eeprom_size);
	// Preset EEPROM
	for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
		eeprom[i] = 0xFF;
	
	// eepromTable must be declared before setting any defaults so updateEepromTable() can update it
	eepromTable = new QTextEdit();
	eepromTable->setReadOnly(true);
	eepromTable->setLineWrapMode(QTextEdit::NoWrap);
	QFont eepromTableFont;
	eepromTableFont.setFamily("Courier");
	eepromTableFont.setStyleHint(QFont::Monospace);
	eepromTableFont.setFixedPitch(true);
	eepromTableFont.setPointSize(10);
	eepromTable->setFont(eepromTableFont);

	QWidget *eepromControls = new QWidget();
	QHBoxLayout *eepromControlsLayout = new QHBoxLayout;
	QLabel *eepromAddrLabel = new QLabel(tr("Address:"));
	eepromAddr = new HexSpinBox;
	eepromAddr->setRange(0,(getAVRInfo(avrDevice)->eeprom_size)-1);
	eepromAddr->setPrefix("0x");
	eepromControlsLayout->addWidget(eepromAddrLabel, 0);
	eepromControlsLayout->addWidget(eepromAddr, 0);
	eepromControlsLayout->addSpacing(10);
	QLabel *eepromDataLabel = new QLabel(tr("Data:"));
	eepromData = new HexSpinBox;
	eepromData->setRange(0,255);
	eepromData->setPrefix("0x");
	eepromDataBinary = new QLabel(tr(""));
	eepromDataBinary->setFont(eepromTableFont);
	eepromReadButton = new QPushButton(tr("&Read EEPROM"));
	eepromReadButton->setFocusPolicy(Qt::NoFocus);
	eepromControlsLayout->addWidget(eepromDataLabel, 0);
	eepromControlsLayout->addWidget(eepromData, 0);
	eepromControlsLayout->addSpacing(10);
	eepromControlsLayout->addWidget(eepromDataBinary, 0);
	eepromControlsLayout->addStretch(1);  // Add sacrificial stretch space to middle
	eepromControlsLayout->addWidget(eepromReadButton);
	eepromControls->setLayout(eepromControlsLayout);
	connect(eepromAddr, SIGNAL(valueChanged(int)), this, SLOT(updateEepromTable()));
	connect(eepromData, SIGNAL(valueChanged(int)), this, SLOT(updateByte()));
	connect(eepromReadButton, SIGNAL(clicked()), this, SLOT(read()));

	// From https://bugreports.qt-project.org/browse/QTBUG-15809
	updateEepromTable();  // Force update to get some data in the table
	eepromTable->document()->adjustSize();
	eepromTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	int eepromDialogSize;
	eepromDialogSize = eepromTable->minimumSizeHint().width(); // should get correct value, but really - not.
	eepromDialogSize -= eepromTable->verticalScrollBar()->sizeHint().width(); //subtracting incorrect value.
	eepromDialogSize += eepromTable->document()->size().width();
	eepromTable->setMinimumWidth(eepromDialogSize);



	// Create buttons
	writeButton = new QPushButton(tr("&Write EEPROM"));
	writeButton->setFocusPolicy(Qt::NoFocus);
	readButton = new QPushButton(tr("&Read EEPROM"));
	readButton->setFocusPolicy(Qt::NoFocus);
	connect(readButton, SIGNAL(clicked()), this, SLOT(read()));
	connect(writeButton, SIGNAL(clicked()), this, SLOT(write()));

	QWidget *nodeWidgets = new QWidget();
	QHBoxLayout *nodeLayout = new QHBoxLayout;
	QLabel *nodeAddrLabel = new QLabel(tr("Node Address:"));
	nodeAddr = new HexSpinBox;
	nodeAddr->setRange(0,255);
	nodeAddr->setPrefix("0x");
	nodeLayout->addWidget(nodeAddrLabel, 0);
	nodeLayout->addWidget(nodeAddr, 0);
	nodeLayout->addSpacing(10);
	QLabel *transmitIntervalLabel = new QLabel(tr("Transmit Interval:"));
	transmitInterval = new QDoubleSpinBox;
	transmitInterval->setRange(0.0, 6553.5);
	transmitInterval->setSingleStep(1.0);
	transmitInterval->setDecimals(1);
	transmitInterval->setSuffix("s");
	nodeLayout->addWidget(transmitIntervalLabel, 0);
	nodeLayout->addWidget(transmitInterval, 0);
	nodeLayout->addStretch(1);  // Add sacrificial stretch space to middle
	nodeLayout->addWidget(readButton);
	nodeLayout->addWidget(writeButton);
	nodeWidgets->setLayout(nodeLayout);

	// Connect signals
	connect(nodeAddr, SIGNAL(valueChanged(int)), this, SLOT(nodeAddrUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(nodeAddrSet()));
	connect(transmitInterval, SIGNAL(valueChanged(double)), this, SLOT(transmitIntervalUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(transmitIntervalSet()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(updateEepromTable()));
	
	// Set defaults
	nodeAddr->setValue(0x20);
	nodeAddrUpdated();  // Force update for initial value, even if value not changed
	transmitInterval->setValue(5.0);
	transmitIntervalUpdated();  // Force update for initial value, even if value not changed

	tabWidget = new QTabWidget();



	eepromDialog = new QDialog();
	QVBoxLayout *eepromLayout = new QVBoxLayout;
	eepromLayout->addWidget(eepromTable);
	eepromLayout->addWidget(eepromControls);
	eepromDialog->setLayout(eepromLayout);
	eepromDialog->setWindowTitle("MRGui EEPROM Viewer");
	
	
	
	consoleCloseButton = new QPushButton(tr("Close"));
	consoleText = new QTextEdit();
	consoleText->setReadOnly(true);
	consoleText->setLineWrapMode(QTextEdit::NoWrap);
	consoleText->setMinimumWidth(500);
	consoleText->setMinimumHeight(400);
	consoleText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	QPalette p = consoleText->palette();
	p.setColor(QPalette::Base, QColor(0, 0, 0));
	p.setColor(QPalette::Text, QColor(0, 255, 0));
	consoleText->setPalette(p);
	QFont consoleFont;
	consoleFont.setFamily("Courier");
	consoleFont.setStyleHint(QFont::Monospace);
	consoleFont.setFixedPitch(true);
	consoleFont.setPointSize(8);
	consoleText->setFont(consoleFont);

	consoleDialog = new QDialog();
	QVBoxLayout *consoleLayout = new QVBoxLayout;
	consoleLayout->addWidget(consoleText);
	consoleLayout->addWidget(consoleCloseButton);
	consoleLayout->setAlignment(consoleCloseButton, Qt::AlignCenter);
	consoleDialog->setLayout(consoleLayout);
	consoleDialog->setWindowTitle("Console");
	consoleDialog->setModal(true);
	connect(consoleCloseButton, SIGNAL(clicked()), consoleDialog, SLOT(accept()));
	connect(consoleDialog, SIGNAL(finished(int)), this, SLOT(cleanupConsole()));
	
	
	
	avrdudeProcess = new QProcess(this);
	connect(avrdudeProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));
	connect(avrdudeProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(avrdudeDone(void)));



	QAction *openAction = new QAction(tr("&Open..."), this);
	QAction *avrdudeAction = new QAction(tr("Set &Avrdude Path..."), this);
	connect(avrdudeAction, SIGNAL(triggered()), this, SLOT(getAvrdudePath()));
	QAction *exitAction = new QAction(tr("E&xit"), this);
	connect(exitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
	QAction *readAction = new QAction(tr("&Read EEPROM"), this);
	connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
	QAction *writeAction = new QAction(tr("&Write EEPROM"), this);
	connect(writeAction, SIGNAL(triggered()), this, SLOT(write()));
	QAction *updateAction = new QAction(tr("&Update Firmware..."), this);
	connect(updateAction, SIGNAL(triggered()), this, SLOT(updateFirmware()));
	QAction *eepromAction = new QAction(tr("&EEPROM Editor..."), this);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addSeparator();
	fileMenu->addAction(avrdudeAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	QMenu *programMenu = menuBar()->addMenu(tr("&Program"));
	programMenu->addAction(readAction);
	programMenu->addAction(writeAction);
	programMenu->addAction(updateAction);
	programMenu->addSeparator();

	programmerGroup = new QActionGroup(this);
	for(uint8_t i=0; i<(sizeof(proginfo)/sizeof(proginfo[0])); i++)
	{
		programmerAction[i] = new QAction(tr(proginfo[i].nice_name), this);
		programmerAction[i]->setCheckable(true);
		programmerGroup->addAction(programmerAction[i]);
		programMenu->addAction(programmerAction[i]);
	}
	programmerAction[0]->setChecked(true);

	QMenu *advancedMenu = menuBar()->addMenu(tr("&Advanced"));
	advancedMenu->addAction(eepromAction);
	connect(eepromAction, SIGNAL(triggered()), eepromDialog, SLOT(show()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(tabWidget);
	layout->addWidget(nodeWidgets);



    QWidget *widget = new QWidget;
    setCentralWidget(widget);
	widget->setLayout(layout);

	setWindowTitle(tr("MRGui Programmer"));
}

const AVRInfo* Window::getAVRInfo(const char* part_name)
{
	for(uint32_t i=0; i<sizeof(avrinfo)/sizeof(AVRInfo); i++)
	{
		if (0 == strcmp(part_name, avrinfo[i].part_name))
			return(&avrinfo[i]);
	}
	return(NULL);
}

void Window::readStderr(void)
{
	consoleText->insertPlainText(avrdudeProcess->readAllStandardError());
	consoleText->verticalScrollBar()->setValue(consoleText->verticalScrollBar()->maximum());
}

void Window::avrdudeDone(void)
{
	FILE* ihexInfile;
	IntelHexMemory eepromMem(getAVRInfo(avrDevice)->eeprom_size);

	consoleCloseButton->setEnabled(true);
	
	switch(avrdudeAction)
	{
		case READ_EEPROM:
			ihexInfile = fopen("mrgui.eeprom", "r");
			if(ihexInfile != NULL)
			{
				eepromMem.read_ihex(ihexInfile);
				for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
				{
					eeprom[i] = eepromMem.read_uint8(i);
				}
				fclose(ihexInfile);	
				emit eepromUpdated();
			}
			remove("mrgui.eeprom");
			break;

		case WRITE_EEPROM:
			remove("mrgui.eeprom");
			break;

		case UPDATE_FIRMWARE:
			break;
	}
}

void Window::getAvrdudePath(void)
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select Avrdude Path"));
	if(!path.isNull())
		strncpy(avrdudePath, path.toLocal8Bit().data(), sizeof(avrdudePath));
}

void Window::cleanupConsole(void)
{
	if(QProcess::NotRunning != avrdudeProcess->state())
	{
		avrdudeProcess->close();
	}
}

uint8_t Window::findProgrammerIndex(void)
{
	uint8_t programmerIndex;
	for(programmerIndex=0; programmerIndex<(sizeof(proginfo)/sizeof(proginfo[0])); programmerIndex++)
	{
		if(programmerGroup->checkedAction() == programmerAction[programmerIndex])
			break;
	}
	return(programmerIndex);
}

void Window::write(void)
{
	IntelHexMemory eepromMem(getAVRInfo(avrDevice)->eeprom_size);

	for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
	{
		eepromMem.write_uint8(i, eeprom[i]);
	}

	FILE* ihexOutfile = fopen("mrgui.eeprom", "w");
	eepromMem.write_ihex(ihexOutfile);
	fclose(ihexOutfile);

	consoleCloseButton->setEnabled(false);
	consoleText->clear();
	consoleDialog->show();
	QString cmdline = QString("%1 -C %2 -c %3 -p %4 -B1 -U eeprom:w:mrgui.eeprom:i").arg(avrdudePath, avrdudeConfPath, proginfo[findProgrammerIndex()].avrdude_name, getAVRInfo(avrDevice)->part_name);
	consoleText->append(cmdline.append("\n\n- - - - - - -\n\n"));
	avrdudeAction = WRITE_EEPROM;
	avrdudeProcess->start(cmdline);
}

void Window::read(void)
{
	consoleCloseButton->setEnabled(false);
	consoleText->clear();
	consoleDialog->show();
	QString cmdline = QString("%1 -C %2 -c %3 -p %4 -B1 -U eeprom:r:mrgui.eeprom:i").arg(avrdudePath, avrdudeConfPath, proginfo[findProgrammerIndex()].avrdude_name, getAVRInfo(avrDevice)->part_name);
	consoleText->append(cmdline.append("\n\n- - - - - - -\n\n"));
	avrdudeAction = READ_EEPROM;
	avrdudeProcess->start(cmdline);
}

void Window::updateFirmware(void)
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select Firmware File"), firmwarePath, "HEX Files (*.hex)");
	if(!path.isNull())
	{
		firmwarePath = QFileInfo(path).path();
	
		consoleCloseButton->setEnabled(false);
		consoleText->clear();
		consoleDialog->show();
		QString cmdline = QString("%1 -C %2 -c %3 -p %4 -B1 -U flash:w:%4:i").arg(avrdudePath, avrdudeConfPath, proginfo[findProgrammerIndex()].avrdude_name, getAVRInfo(avrDevice)->part_name, path);
		consoleText->append(cmdline.append("\n\n- - - - - - -\n\n"));
		avrdudeAction = UPDATE_FIRMWARE;
		avrdudeProcess->start(cmdline);
	}
}

void Window::updateByte(void)
{
	eeprom[eepromAddr->value()] = eepromData->value();
	emit eepromUpdated();
}

void Window::updateEepromTable(void)
{
	uint32_t rows = (getAVRInfo(avrDevice)->eeprom_size) / 16;
	int scrollPosition = eepromTable->verticalScrollBar()->value();
	QString eepromContents = QString();
	for(uint8_t r = 0; r < rows; r++)
	{
		QString line = QString("%1:  ").arg(r*16, 4, 16, QChar('0'));
		for(uint8_t c = 0; c < 16; c++)
		{
			if(r*16+c == eepromAddr->value())
				line.append(QString("<font style=\"color: black; background-color: #ccccff\">%1</font> ").arg(eeprom[r*16 + c], 2, 16, QChar('0')));
			else
				line.append(QString("%1 ").arg(eeprom[r*16 + c], 2, 16, QChar('0')));
			if(7 == c)
				line.append(" ");
		}
		eepromContents.append(line).append("<br>\n");
	}
	eepromTable->setText(eepromContents);
	eepromTable->verticalScrollBar()->setValue(scrollPosition);

	eepromData->setValue(eeprom[eepromAddr->value()]);
	eepromDataBinary->setText(QString("%1 %2").arg((eepromData->value() >> 4) & 0xF, 4, 2, QChar('0')).arg(eepromData->value() & 0xF, 4, 2, QChar('0')));
}

void Window::nodeAddrUpdated(void)
{
	eeprom[MRBUS_EE_DEVICE_ADDR] = nodeAddr->value();
	updateEepromTable();
}

void Window::nodeAddrSet(void)
{
	nodeAddr->setValue(eeprom[MRBUS_EE_DEVICE_ADDR]);
}

void Window::transmitIntervalUpdated(void)
{
	eeprom[MRBUS_EE_DEVICE_UPDATE_H] = (int)((transmitInterval->value() * 10) / 256) & 0xFF;
	eeprom[MRBUS_EE_DEVICE_UPDATE_L] = (int)(transmitInterval->value() * 10) & 0xFF;
	updateEepromTable();
}

void Window::transmitIntervalSet(void)
{
	transmitInterval->setValue((eeprom[MRBUS_EE_DEVICE_UPDATE_H]*256 + eeprom[MRBUS_EE_DEVICE_UPDATE_L]) / 10.0);
}

NodeDialog::NodeDialog()
{
	QPushButton *okButton = new QPushButton(tr("&OK"));
	QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	nodeList = new QListWidget();
	nodeList->addItem(tr("MRB-IIAB"));
	nodeList->addItem(tr("MRB-ACSW"));
	nodeList->addItem(tr("MRB-GIM2"));
	nodeList->sortItems();
	nodeList->insertItem(0, tr("Generic (ATMega328)"));
	nodeList->insertItem(1, tr("Generic (ATMega328P)"));
	nodeList->insertItem(2, tr("Generic (ATMega1284)"));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(nodeList, 0, 0, 1, 2);
	layout->addWidget(okButton, 1, 0);
	layout->addWidget(cancelButton, 1, 1);
	
	setLayout(layout);
	setWindowTitle(tr("Select Node"));
}



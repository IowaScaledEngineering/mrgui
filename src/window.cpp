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

#include "window.h"
#include "avrinfo.h"

Window::Window(const char *device)
{
	strncpy(avrDevice, device, sizeof(avrDevice));
	eepromSize = getAVRInfo(device)->eeprom_size;
	eeprom = (uint8_t*)malloc(eepromSize);
	// Preset EEPROM
	for(uint32_t i=0; i<eepromSize; i++)
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

	// From https://bugreports.qt-project.org/browse/QTBUG-15809
	updateEepromTable();  // Force update to get some data in the table
	eepromTable->document()->adjustSize();
	eepromTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	int eepromDialogSize;
	eepromDialogSize = eepromTable->minimumSizeHint().width(); // should get correct value, but really - not.
	eepromDialogSize -= eepromTable->verticalScrollBar()->sizeHint().width(); //subtracting incorrect value.
	eepromDialogSize += eepromTable->document()->size().width();
	eepromTable->setMinimumWidth(eepromDialogSize);

	QWidget *eepromControls = new QWidget();
	QHBoxLayout *eepromControlsLayout = new QHBoxLayout;
	QLabel *eepromAddrLabel = new QLabel(tr("Address:"));
	eepromAddr = new HexSpinBox;
	eepromAddr->setRange(0,eepromSize-1);
	eepromAddr->setPrefix("0x");
	eepromControlsLayout->addWidget(eepromAddrLabel, 0);
	eepromControlsLayout->addWidget(eepromAddr, 0);
	eepromControlsLayout->addSpacing(10);
	QLabel *eepromDataLabel = new QLabel(tr("Data:"));
	eepromData = new HexSpinBox;
	eepromData->setRange(0,255);
	eepromData->setPrefix("0x");
	eepromWriteButton = new QPushButton(tr("&Change Byte"));
	eepromWriteButton->setFocusPolicy(Qt::NoFocus);
	eepromReadButton = new QPushButton(tr("&Read EEPROM"));
	eepromReadButton->setFocusPolicy(Qt::NoFocus);
	eepromControlsLayout->addWidget(eepromDataLabel, 0);
	eepromControlsLayout->addWidget(eepromData, 0);
	eepromControlsLayout->addWidget(eepromWriteButton);
	eepromControlsLayout->addStretch(1);  // Add sacrificial stretch space to middle
	eepromControlsLayout->addWidget(eepromReadButton);
	eepromControls->setLayout(eepromControlsLayout);
	connect(eepromReadButton, SIGNAL(clicked()), this, SLOT(read()));
	connect(eepromWriteButton, SIGNAL(clicked()), this, SLOT(updateByte()));



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
	nodeAddr->setRange(1,254);
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
	
	// Set defaults
	nodeAddr->setValue(0x01);
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



	QAction *openAction = new QAction(tr("&Open..."), this);
	QAction *exitAction = new QAction(tr("E&xit"), this);
	connect(exitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
	QAction *readAction = new QAction(tr("&Read EEPROM"), this);
	connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
	QAction *writeAction = new QAction(tr("&Write EEPROM"), this);
	connect(writeAction, SIGNAL(triggered()), this, SLOT(write()));
	QAction *updateAction = new QAction(tr("&Update Firmware..."), this);
	QAction *eepromAction = new QAction(tr("&EEPROM Editor..."), this);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(exitAction);

	QMenu *configMenu = menuBar()->addMenu(tr("&Program"));
	configMenu->addAction(readAction);
	configMenu->addAction(writeAction);
	configMenu->addAction(updateAction);

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

void Window::write(void)
{
	// FIXME: write eeprom to AVR
}

void Window::read(void)
{
	// FIXME: read eeprom from AVR
	emit eepromUpdated();
}

void Window::updateByte(void)
{
	eeprom[eepromAddr->value()] = eepromData->value();
	emit eepromUpdated();
}

void Window::updateEepromTable(void)
{
	uint32_t rows = eepromSize / 16;
	QString eepromContents = QString();
	for(uint8_t r = 0; r < rows; r++)
	{
		QString line = QString("%1:  ").arg(r*16, 4, 16, QChar('0'));
		for(uint8_t c = 0; c < 16; c++)
		{
			line.append(QString("%1 ").arg(eeprom[r*16 + c], 2, 16, QChar('0')));
			if(7 == c)
				line.append(" ");
		}
		eepromContents.append(line).append("\n");
	}
	int scrollPosition = eepromTable->verticalScrollBar()->value();
	eepromTable->setText(eepromContents);
	eepromTable->verticalScrollBar()->setValue(scrollPosition);
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
	nodeList->insertItem(1, tr("Generic (ATMega1284)"));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(nodeList, 0, 0, 1, 2);
	layout->addWidget(okButton, 1, 0);
	layout->addWidget(cancelButton, 1, 1);
	
	setLayout(layout);
	setWindowTitle(tr("Select Node"));
}



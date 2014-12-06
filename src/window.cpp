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

#include <QtWidgets>

#include "window.h"
#include "hexspinbox.h"

Window::Window()
{
	writeButton = new QPushButton(tr("&Write"));
	writeButton->setFocusPolicy(Qt::NoFocus);

	readButton = new QPushButton(tr("&Read"));
	readButton->setFocusPolicy(Qt::NoFocus);

	QWidget *nodeWidgets = new QWidget();
	QHBoxLayout *nodeLayout = new QHBoxLayout;
	QLabel *nodeAddrLabel = new QLabel(tr("Node Address:"));
	nodeAddr = new HexSpinBox;
	nodeAddr->setValue(0);
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
	transmitInterval->setValue(5.0);
	transmitInterval->setSuffix("s");
	nodeLayout->addWidget(transmitIntervalLabel, 0);
	nodeLayout->addWidget(transmitInterval, 0);
	nodeLayout->addStretch(1);  // Add sacrificial stretch space to middle
	nodeLayout->addWidget(readButton);
	nodeLayout->addWidget(writeButton);
	nodeWidgets->setLayout(nodeLayout);

	tabWidget = new QTabWidget();

	QAction *openAction = new QAction(tr("&Open..."), this);
	QAction *exitAction = new QAction(tr("E&xit"), this);
	readAction = new QAction(tr("&Read EEPROM"), this);
	writeAction = new QAction(tr("&Write EEPROM"), this);
	QAction *updateAction = new QAction(tr("&Update Firmware..."), this);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(exitAction);
	connect(exitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));

	QMenu *configMenu = menuBar()->addMenu(tr("&Program"));
	configMenu->addAction(readAction);
	configMenu->addAction(writeAction);
	configMenu->addAction(updateAction);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(tabWidget);
	layout->addWidget(nodeWidgets);

    QWidget *widget = new QWidget;
    setCentralWidget(widget);
	widget->setLayout(layout);

	setWindowTitle(tr("MRGui Programmer"));
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
	nodeList->insertItem(0, tr("Generic MRBus"));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(nodeList, 0, 0, 1, 2);
	layout->addWidget(okButton, 1, 0);
	layout->addWidget(cancelButton, 1, 1);
	
	setLayout(layout);
	setWindowTitle(tr("Select Node"));
}


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
#include "avr.h"

Window::Window()
{
	QLabel *timeoutLabel = new QLabel(tr("Timeout:"));
	QLabel *timeout0Label = new QLabel(tr("West"));
	QLabel *timeout1Label = new QLabel(tr("East"));
	QLabel *timeout2Label = new QLabel(tr("North"));
	QLabel *timeout3Label = new QLabel(tr("South"));
	timeout0 = new QSpinBox;
	timeout1 = new QSpinBox;
	timeout2 = new QSpinBox;
	timeout3 = new QSpinBox;
	timeout0->setRange(0, 255);
	timeout0->setSingleStep(1);
	timeout0->setSuffix("s");
	timeout1->setRange(0, 255);
	timeout1->setSingleStep(1);
	timeout1->setSuffix("s");
	timeout2->setRange(0, 255);
	timeout2->setSingleStep(1);
	timeout2->setSuffix("s");
	timeout3->setRange(0, 255);
	timeout3->setSingleStep(1);
	timeout3->setSuffix("s");

	QLabel *debounceLabel = new QLabel(tr("Interlocking:"));
	debounce = new QSpinBox;
	debounce->setRange(0, 255);
	debounce->setSingleStep(1);
	debounce->setSuffix("s");

	QLabel *clockLabel = new QLabel(tr("Fast Clock:"));
	QLabel *clockSourceLabel = new QLabel(tr("Address"));
	clockSource = new HexSpinBox;
	clockSource->setValue(0);
	clockSource->setPrefix("0x");

	QLabel *maxDeadReckoningLabel = new QLabel(tr("Timeout"));
	maxDeadReckoning = new QSpinBox;
	maxDeadReckoning->setRange(0, 255);
	maxDeadReckoning->setSingleStep(1);
	maxDeadReckoning->setSuffix("s");

	QLabel *detectorPolarityLabel = new QLabel(tr("Polarity:"));
	detectorPolarity = new QComboBox();
	detectorPolarity->addItem("High = Train Present");
	detectorPolarity->addItem("Low = Train Present");

	QButtonGroup *ledPolarityGroup = new QButtonGroup();
	ledPolarityAnode = new QRadioButton("Common Anode");
	ledPolarityAnode->setChecked(true);
	ledPolarityCathode = new QRadioButton("Common Cathode");
	ledPolarityGroup->addButton(ledPolarityAnode);
	ledPolarityGroup->addButton(ledPolarityCathode);

	QPushButton *writeButton = new QPushButton(tr("&Write"));
	writeButton->setFocusPolicy(Qt::NoFocus);
	connect(writeButton, SIGNAL(clicked()), this, SLOT(write()));

	QPushButton *readButton = new QPushButton(tr("&Read"));
	readButton->setFocusPolicy(Qt::NoFocus);
	connect(readButton, SIGNAL(clicked()), this, SLOT(read()));

	QPushButton *firmwareButton = new QPushButton(tr("&Update Firmware"));
	firmwareButton->setFocusPolicy(Qt::NoFocus);

	// Create widget layout
	QWidget *detectorPage = new QWidget();
	QGridLayout *detectorLayout = new QGridLayout;
	QHBoxLayout *timeoutLayout = new QHBoxLayout;
	timeoutLayout->addWidget(timeout0Label);
	timeoutLayout->addWidget(timeout0);
	timeoutLayout->addSpacing(10);
	timeoutLayout->addWidget(timeout1Label);
	timeoutLayout->addWidget(timeout1);
	timeoutLayout->addSpacing(10);
	timeoutLayout->addWidget(timeout2Label);
	timeoutLayout->addWidget(timeout2);
	timeoutLayout->addSpacing(10);
	timeoutLayout->addWidget(timeout3Label);
	timeoutLayout->addWidget(timeout3);
	timeoutLayout->addSpacing(10);
	timeoutLayout->addWidget(debounceLabel);
	timeoutLayout->addWidget(debounce);
	detectorLayout->addWidget(timeoutLabel, 0, 0);
	detectorLayout->addLayout(timeoutLayout, 0, 1, Qt::AlignLeft);
	detectorLayout->addWidget(detectorPolarityLabel, 1, 0);
	detectorLayout->addWidget(detectorPolarity, 1, 1, Qt::AlignLeft);
	detectorLayout->setColumnStretch(0, 0);
	detectorLayout->setColumnStretch(1, 1);
	detectorPage->setLayout(detectorLayout);



	QWidget *signalPage = new QWidget();
	QFormLayout *signalLayout = new QFormLayout;
	QHBoxLayout *ledPolarityLayout = new QHBoxLayout;
	ledPolarityLayout->addWidget(ledPolarityAnode);
	ledPolarityLayout->addWidget(ledPolarityCathode);
	signalLayout->addRow(tr("LED Polarity:"), ledPolarityLayout);
	blinky = new QDoubleSpinBox;
	blinky->setRange(0, 25.5);
	blinky->setSingleStep(0.1);
	blinky->setDecimals(1);
	blinky->setSuffix("s");
	signalLayout->addRow(tr("Blink Period:"), blinky);
	signalLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	signalPage->setLayout(signalLayout);



	QWidget *turnoutPage = new QWidget();
	QFormLayout *turnoutLayout = new QFormLayout;
	turnoutPolarity0 = new QComboBox();
	turnoutPolarity0->addItem(tr("Low = Main, High = Siding"));
	turnoutPolarity0->addItem(tr("High = Main, Low = Siding"));
	turnoutLayout->addRow(tr("West Polarity:"), turnoutPolarity0);
	turnoutPolarity2 = new QComboBox();
	turnoutPolarity2->addItem(tr("Low = Main, High = Siding"));
	turnoutPolarity2->addItem(tr("High = Main, Low = Siding"));
	turnoutLayout->addRow(tr("North Polarity:"), turnoutPolarity2);
	turnoutLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	turnoutPage->setLayout(turnoutLayout);



	QWidget *timingPage = new QWidget();
	QFormLayout *timingLayout = new QFormLayout;
	lockout = new QSpinBox;
	lockout->setRange(0, 255);
	lockout->setSingleStep(1);
	lockout->setSuffix("s");
	timingLayout->addRow(tr("Lockout Time:"), lockout);
	timelock = new QSpinBox;
	timelock->setRange(0, 255);
	timelock->setSingleStep(1);
	timelock->setSuffix("s");
	timingLayout->addRow(tr("Timelock Time:"), timelock);
	timingLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	timingPage->setLayout(timingLayout);



	QWidget *schedulePage = new QWidget();
	QGridLayout *scheduleLayout = new QGridLayout;
	QHBoxLayout *clockLayout = new QHBoxLayout;
	clockLayout->addWidget(clockSourceLabel);
	clockLayout->addWidget(clockSource);
	clockLayout->addSpacing(10);
	clockLayout->addWidget(maxDeadReckoningLabel);
	clockLayout->addWidget(maxDeadReckoning);
	scheduleLayout->addWidget(clockLabel, 4, 0);
	scheduleLayout->addLayout(clockLayout, 4, 1, Qt::AlignLeft);
	scheduleLayout->setColumnStretch(0, 0);
	scheduleLayout->setColumnStretch(1, 1);
	schedulePage->setLayout(scheduleLayout);



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
//	nodeLayout->addWidget(firmwareButton);
	nodeWidgets->setLayout(nodeLayout);

	QTabWidget *iiabTabs = new QTabWidget();
	iiabTabs->addTab(detectorPage, "Detectors");
	iiabTabs->addTab(signalPage, "Signals");
	iiabTabs->addTab(turnoutPage, "Turnouts");
	iiabTabs->addTab(timingPage, "Timing");
	iiabTabs->addTab(schedulePage, "Schedules");

	QAction *readAction = new QAction(tr("&Read"), this);
	QAction *writeAction = new QAction(tr("&Write"), this);
	QAction *updateAction = new QAction(tr("&Update Firmware..."), this);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(updateAction);

	QMenu *configMenu = menuBar()->addMenu(tr("&Config"));
	configMenu->addAction(readAction);
	connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
	configMenu->addAction(writeAction);
	connect(writeAction, SIGNAL(triggered()), this, SLOT(write()));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(iiabTabs);
	layout->addWidget(nodeWidgets);

    QWidget *widget = new QWidget;
    setCentralWidget(widget);
	widget->setLayout(layout);

	setWindowTitle(tr("MRGui Programmer"));
}

void Window::write(void)
{
	uint8_t data[256];

	data[0] = nodeAddr->value();
	avrWriteEEPROM(0x00, data, 1);

	data[0] = (int)((transmitInterval->value() * 10) / 256) & 0xFF;
	data[1] = (int)(transmitInterval->value() * 10) & 0xFF;
	avrWriteEEPROM(0x02, data, 2);
}

void Window::read(void)
{
	uint8_t data[256];

	avrReadEEPROM(0x00, data, 1);
	nodeAddr->setValue(data[0]);

	avrReadEEPROM(0x02, data, 2);
	transmitInterval->setValue((data[0]*256 + data[1]) / 10.0);
}


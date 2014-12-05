/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-iiab.cpp
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

#include "node-iiab.h"
#include "window.h"
#include "hexspinbox.h"
#include "avr.h"

Node_IIAB::Node_IIAB()
{
	// Create widget layout
	QWidget *detectorPage = new QWidget();
	QGridLayout *detectorLayout = new QGridLayout;
	QHBoxLayout *timeoutLayout = new QHBoxLayout;
	QLabel *timeoutLabel = new QLabel(tr("Timeout:"));

	QLabel *timeout0Label = new QLabel(tr("West"));
	timeout0 = new QSpinBox;
	timeout0->setRange(0, 255);
	timeout0->setSingleStep(1);
	timeout0->setSuffix("s");
	timeoutLayout->addWidget(timeout0Label);
	timeoutLayout->addWidget(timeout0);
	timeoutLayout->addSpacing(10);

	QLabel *timeout1Label = new QLabel(tr("East"));
	timeout1 = new QSpinBox;
	timeout1->setRange(0, 255);
	timeout1->setSingleStep(1);
	timeout1->setSuffix("s");
	timeoutLayout->addWidget(timeout1Label);
	timeoutLayout->addWidget(timeout1);
	timeoutLayout->addSpacing(10);

	QLabel *timeout2Label = new QLabel(tr("North"));
	timeout2 = new QSpinBox;
	timeout2->setRange(0, 255);
	timeout2->setSingleStep(1);
	timeout2->setSuffix("s");
	timeoutLayout->addWidget(timeout2Label);
	timeoutLayout->addWidget(timeout2);
	timeoutLayout->addSpacing(10);

	QLabel *timeout3Label = new QLabel(tr("South"));
	timeout3 = new QSpinBox;
	timeout3->setRange(0, 255);
	timeout3->setSingleStep(1);
	timeout3->setSuffix("s");
	timeoutLayout->addWidget(timeout3Label);
	timeoutLayout->addWidget(timeout3);
	timeoutLayout->addSpacing(10);

	QLabel *debounceLabel = new QLabel(tr("Interlocking:"));
	debounce = new QSpinBox;
	debounce->setRange(0, 255);
	debounce->setSingleStep(1);
	debounce->setSuffix("s");
	timeoutLayout->addWidget(debounceLabel);
	timeoutLayout->addWidget(debounce);
	detectorLayout->addWidget(timeoutLabel, 0, 0);
	detectorLayout->addLayout(timeoutLayout, 0, 1, Qt::AlignLeft);
	QLabel *detectorPolarityLabel = new QLabel(tr("Polarity:"));
	detectorPolarity = new QComboBox();
	detectorPolarity->addItem("High = Train Present",1);
	detectorPolarity->addItem("Low = Train Present",0);
	detectorLayout->addWidget(detectorPolarityLabel, 1, 0);
	detectorLayout->addWidget(detectorPolarity, 1, 1, Qt::AlignLeft);
	detectorLayout->setColumnStretch(0, 0);
	detectorLayout->setColumnStretch(1, 1);
	detectorPage->setLayout(detectorLayout);



	QWidget *signalPage = new QWidget();
	QFormLayout *signalLayout = new QFormLayout;
	QHBoxLayout *ledPolarityLayout = new QHBoxLayout;
	ledPolarityAnode = new QRadioButton("Common Anode");
	ledPolarityAnode->setChecked(true);
	ledPolarityCathode = new QRadioButton("Common Cathode");
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
	QFormLayout *scheduleLayout = new QFormLayout;
	clockSource = new HexSpinBox;
	clockSource->setValue(0);
	clockSource->setPrefix("0x");
	scheduleLayout->addRow(tr("Fast Clock Address:"), clockSource);
	maxDeadReckoning = new QDoubleSpinBox;
	maxDeadReckoning->setRange(0.0, 25.5);
	maxDeadReckoning->setSingleStep(1.0);
	maxDeadReckoning->setDecimals(1);
	maxDeadReckoning->setValue(5.0);
	maxDeadReckoning->setSuffix("s");
	scheduleLayout->addRow(tr("Fast Clock Timeout:"), maxDeadReckoning);
	simTrainWindow = new QSpinBox;
	simTrainWindow->setRange(0, 255);
	simTrainWindow->setSingleStep(1);
	simTrainWindow->setSuffix("min");
	scheduleLayout->addRow(tr("Trigger Window:"), simTrainWindow);
	scheduleLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	schedulePage->setLayout(scheduleLayout);



	tabWidget->addTab(detectorPage, "Detectors");
	tabWidget->addTab(signalPage, "Signals");
	tabWidget->addTab(turnoutPage, "Turnouts");
	tabWidget->addTab(timingPage, "Timing");
	tabWidget->addTab(schedulePage, "Schedules");


	// Connect read/write functions
	connect(readButton, SIGNAL(clicked()), this, SLOT(read()));
	connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
	connect(writeButton, SIGNAL(clicked()), this, SLOT(write()));
	connect(writeAction, SIGNAL(triggered()), this, SLOT(write()));

}

void Node_IIAB::write(void)
{
	uint8_t data[256];

	data[0] = nodeAddr->value();
	avrWriteEEPROM(0x00, data, 1);

	data[0] = (int)((transmitInterval->value() * 10) / 256) & 0xFF;
	data[1] = (int)(transmitInterval->value() * 10) & 0xFF;
	avrWriteEEPROM(0x02, data, 2);

	data[0] = timeout0->value();
	data[1] = timeout1->value();
	data[2] = timeout2->value();
	data[3] = timeout3->value();
	avrWriteEEPROM(EE_TIMEOUT_SECONDS, data, 4);

	data[0] = lockout->value();
	avrWriteEEPROM(EE_LOCKOUT_SECONDS, data, 1);

	data[0] = timelock->value();
	avrWriteEEPROM(EE_TIMELOCK_SECONDS, data, 1);

	data[0] = debounce->value();
	avrWriteEEPROM(EE_DEBOUNCE_SECONDS, data, 1);

	data[0] = clockSource->value();
	avrWriteEEPROM(EE_CLOCK_SOURCE_ADDRESS, data, 1);

	data[0] = maxDeadReckoning->value() * 10;
	avrWriteEEPROM(EE_MAX_DEAD_RECKONING, data, 1);

	data[0] = simTrainWindow->value();
	avrWriteEEPROM(EE_SIM_TRAIN_WINDOW, data, 1);

	data[0] = blinky->value() * 10;
	avrWriteEEPROM(EE_BLINKY_DECISECS, data, 1);

	//FIXME: Add polarity0 write
}

void Node_IIAB::read(void)
{
	uint8_t data[256];
	int index;

	avrReadEEPROM(MRBUS_EE_DEVICE_ADDR, data, 1);
	nodeAddr->setValue(data[0]);

	avrReadEEPROM(MRBUS_EE_DEVICE_UPDATE_H, &data[0], 1);
	avrReadEEPROM(MRBUS_EE_DEVICE_UPDATE_L, &data[1], 1);
	transmitInterval->setValue((data[0]*256 + data[1]) / 10.0);

	avrReadEEPROM(EE_TIMEOUT_SECONDS, data, 4);
	timeout0->setValue(data[0]);
	timeout1->setValue(data[1]);
	timeout2->setValue(data[2]);
	timeout3->setValue(data[3]);

	avrReadEEPROM(EE_LOCKOUT_SECONDS, data, 1);
	lockout->setValue(data[0]);

	avrReadEEPROM(EE_TIMELOCK_SECONDS, data, 1);
	timelock->setValue(data[0]);

	avrReadEEPROM(EE_DEBOUNCE_SECONDS, data, 1);
	debounce->setValue(data[0]);

	avrReadEEPROM(EE_CLOCK_SOURCE_ADDRESS, data, 1);
	clockSource->setValue(data[0]);

	avrReadEEPROM(EE_MAX_DEAD_RECKONING, data, 1);
	maxDeadReckoning->setValue(data[0] / 10.0);

	avrReadEEPROM(EE_SIM_TRAIN_WINDOW, data, 1);
	simTrainWindow->setValue(data[0]);

	avrReadEEPROM(EE_BLINKY_DECISECS, data, 1);
	blinky->setValue(data[0] / 10.0);

	avrReadEEPROM(EE_INPUT_POLARITY0, data, 1);
	switch(data[0] & 0x7F)
	{
		case 0x7F:
			index = detectorPolarity->findData(1);
			break;
		case 0x00:
			index = detectorPolarity->findData(0);
			break;
		default:
			index = -1;
			break;
	}
	if(-1 != index)
		detectorPolarity->setCurrentIndex(index);
	else
	{
		// FIXME: Handle mixed cases
	}
}

/*
#define EE_INPUT_POLARITY0      0x20
#define EE_INPUT_POLARITY1      0x21
#define EE_OUTPUT_POLARITY0     0x22
#define EE_OUTPUT_POLARITY1     0x23
#define EE_OUTPUT_POLARITY2     0x24
#define EE_OUTPUT_POLARITY3     0x25
#define EE_OUTPUT_POLARITY4     0x26
#define EE_MISC_CONFIG          0x30
#define EE_SIM_TRAINS           0x40
*/



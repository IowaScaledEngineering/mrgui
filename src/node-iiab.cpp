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

#include "node-iiab.h"

Node_IIAB::Node_IIAB(const char *device, int size) : Window(device, size)
{
	// Create widget layout
	for(int i=0; i<8; i++)
	{
		detectorPolarity[i] = new QComboBox;
		detectorPolarity[i]->addItem("High = Train Present",0);
		detectorPolarity[i]->addItem("Low = Train Present",1);
	}

	QWidget *detectorPage = new QWidget();

	QGroupBox *westGroup = new QGroupBox(tr("West Direction"));
	QFormLayout *westLayout = new QFormLayout();
	timeout0 = new QSpinBox;
	timeout0->setRange(0, 255);
	timeout0->setSingleStep(1);
	timeout0->setSuffix("s");
	westLayout->addRow(tr("Timeout:"), timeout0);
	westLayout->addRow(tr("Polarity (Main):"), detectorPolarity[0]);
	westLayout->addRow(tr("Polarity (Siding):"), detectorPolarity[1]);
	westLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	westGroup->setLayout(westLayout);

	QGroupBox *eastGroup = new QGroupBox(tr("East Direction"));
	QFormLayout *eastLayout = new QFormLayout();
	timeout0 = new QSpinBox;
	timeout0->setRange(0, 255);
	timeout0->setSingleStep(1);
	timeout0->setSuffix("s");
	eastLayout->addRow(tr("Timeout:"), timeout0);
	eastLayout->addRow(tr("Polarity:"), detectorPolarity[2]);
	eastLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	eastGroup->setLayout(eastLayout);

	QGroupBox *northGroup = new QGroupBox(tr("North Direction"));
	QFormLayout *northLayout = new QFormLayout();
	timeout0 = new QSpinBox;
	timeout0->setRange(0, 255);
	timeout0->setSingleStep(1);
	timeout0->setSuffix("s");
	northLayout->addRow(tr("Timeout:"), timeout0);
	northLayout->addRow(tr("Polarity (Main):"), detectorPolarity[3]);
	northLayout->addRow(tr("Polarity (Siding):"), detectorPolarity[4]);
	northLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	northGroup->setLayout(northLayout);

	QGroupBox *southGroup = new QGroupBox(tr("South Direction"));
	QFormLayout *southLayout = new QFormLayout();
	timeout0 = new QSpinBox;
	timeout0->setRange(0, 255);
	timeout0->setSingleStep(1);
	timeout0->setSuffix("s");
	southLayout->addRow(tr("Timeout:"), timeout0);
	southLayout->addRow(tr("Polarity:"), detectorPolarity[5]);
	southLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	southGroup->setLayout(southLayout);

	QGroupBox *interlockingGroup = new QGroupBox(tr("Interlocking"));
	QFormLayout *interlockingLayout = new QFormLayout();
	debounce = new QSpinBox;
	debounce->setRange(0, 255);
	debounce->setSingleStep(1);
	debounce->setSuffix("s");
	interlockingLayout->addRow(tr("Timeout:"), debounce);
	interlockingLayout->addRow(tr("Polarity:"), detectorPolarity[6]);
	interlockingLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	interlockingGroup->setLayout(interlockingLayout);

	QGridLayout *detectorLayout = new QGridLayout;
//	detectorLayout->addWidget(westGroup, 0, 0);
//	detectorLayout->addWidget(eastGroup, 0, 1);
//	detectorLayout->addWidget(northGroup, 1, 0);
//	detectorLayout->addWidget(southGroup, 1, 1);
//	detectorLayout->addWidget(interlockingGroup, 2, 0);
//	detectorLayout->addWidget(interchangeGroup, 2, 1);
	detectorLayout->addWidget(westGroup, 1, 0);
	detectorLayout->addWidget(eastGroup, 1, 2);
	detectorLayout->addWidget(northGroup, 0, 1);
	detectorLayout->addWidget(southGroup, 2, 1);
	detectorLayout->addWidget(interlockingGroup, 1, 1);
	detectorPage->setLayout(detectorLayout);
	setStyleSheet("QGroupBox{border: 1px solid gray; border-radius:5px; font-weight: bold; margin-top: 1ex; margin-bottom: 1ex;} QGroupBox::title{subcontrol-origin: margin; subcontrol-position: top left; left: 10px; padding: 0 3px;}");



	QWidget *interchangePage = new QWidget();
	QFormLayout *interchangeLayout = new QFormLayout();
	interchangeLayout->addRow(tr("Detector Polarity:"), detectorPolarity[7]);
	interchangePolarity = new QComboBox;
	interchangePolarity->addItem("High = Track Power Applied",0);
	interchangePolarity->addItem("Low = Track Power Applied",1);
	interchangeLayout->addRow(tr("Relay Polarity:"), interchangePolarity);
	interchangeLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	interchangePage->setLayout(interchangeLayout);



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



	tabWidget->insertTab(0, detectorPage, "Detectors");
	tabWidget->insertTab(1, signalPage, "Signals");
	tabWidget->insertTab(2, turnoutPage, "Turnouts");
	tabWidget->insertTab(3, timingPage, "Timing");
	tabWidget->insertTab(4, interchangePage, "Interchange");
	tabWidget->insertTab(5, schedulePage, "Schedules");
	tabWidget->setCurrentIndex(0);


	// Connect read/write functions
	connect(readButton, SIGNAL(clicked()), this, SLOT(read()));
	connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
	connect(writeButton, SIGNAL(clicked()), this, SLOT(write()));
	connect(writeAction, SIGNAL(triggered()), this, SLOT(write()));

	// Initialize EEPROM array
	unsigned int i;
	for(i=0; i<sizeof(eeprom); i++)
	{
		eeprom[i] = 0;
	}
}

void Node_IIAB::write(void)
{
	eeprom[MRBUS_EE_DEVICE_ADDR] = nodeAddr->value();
	eeprom[MRBUS_EE_DEVICE_UPDATE_H] = (int)((transmitInterval->value() * 10) / 256) & 0xFF;
	eeprom[MRBUS_EE_DEVICE_UPDATE_L] = (int)(transmitInterval->value() * 10) & 0xFF;
	eeprom[EE_TIMEOUT_SECONDS+0] = timeout0->value();
	eeprom[EE_TIMEOUT_SECONDS+1] = timeout1->value();
	eeprom[EE_TIMEOUT_SECONDS+2] = timeout2->value();
	eeprom[EE_TIMEOUT_SECONDS+3] = timeout3->value();
	eeprom[EE_LOCKOUT_SECONDS] = lockout->value();
	eeprom[EE_TIMELOCK_SECONDS] = timelock->value();
	eeprom[EE_DEBOUNCE_SECONDS] = debounce->value();
	eeprom[EE_CLOCK_SOURCE_ADDRESS] = clockSource->value();
	eeprom[EE_MAX_DEAD_RECKONING] = maxDeadReckoning->value() * 10;
	eeprom[EE_SIM_TRAIN_WINDOW] = simTrainWindow->value();
	eeprom[EE_BLINKY_DECISECS] = blinky->value() * 10;

	//FIXME: Add polarity0 write
}

void Node_IIAB::read(void)
{
	nodeAddr->setValue(eeprom[MRBUS_EE_DEVICE_ADDR]);
	transmitInterval->setValue((eeprom[MRBUS_EE_DEVICE_UPDATE_H]*256 + eeprom[MRBUS_EE_DEVICE_UPDATE_L]) / 10.0);
	timeout0->setValue(eeprom[EE_TIMEOUT_SECONDS+0]);
	timeout1->setValue(eeprom[EE_TIMEOUT_SECONDS+1]);
	timeout2->setValue(eeprom[EE_TIMEOUT_SECONDS+2]);
	timeout3->setValue(eeprom[EE_TIMEOUT_SECONDS+3]);
	lockout->setValue(eeprom[EE_LOCKOUT_SECONDS]);
	timelock->setValue(eeprom[EE_TIMELOCK_SECONDS]);
	debounce->setValue(eeprom[EE_DEBOUNCE_SECONDS]);
	clockSource->setValue(eeprom[EE_CLOCK_SOURCE_ADDRESS]);
	maxDeadReckoning->setValue(eeprom[EE_MAX_DEAD_RECKONING] / 10.0);
	simTrainWindow->setValue(eeprom[EE_SIM_TRAIN_WINDOW]);
	blinky->setValue(eeprom[EE_BLINKY_DECISECS] / 10.0);

// Fix polarity
//	avrReadEEPROM(EE_INPUT_POLARITY0, data, 1);
//	switch(data[0] & 0x7F)
//	{
//		case 0x7F:
//			index = detectorPolarity->findData(1);
//			break;
//		case 0x00:
//			index = detectorPolarity->findData(0);
//			break;
//		default:
//			index = -1;
//			break;
//	}
//	if(-1 != index)
//		detectorPolarity->setCurrentIndex(index);
//	else
//	{
//		// FIXME: Handle mixed cases
//	}
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



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
	setStyleSheet("QGroupBox{border: 1px solid gray; border-radius:5px; font-weight: bold; margin-top: 1ex; margin-bottom: 1ex;} QGroupBox::title{subcontrol-origin: margin; subcontrol-position: top left; left: 10px; padding: 0 3px;}");

	for(int i=0; i<8; i++)
	{
		detectorPolarity[i] = new QComboBox;
		detectorPolarity[i]->addItem("High = Train Present",0);
		detectorPolarity[i]->addItem("Low = Train Present",1);
		// Connect signals
		connect(detectorPolarity[i], SIGNAL(currentIndexChanged(int)), this, SLOT(detectorPolarityUpdated()));
		// Set defaults
		detectorPolarity[i]->setCurrentIndex(0);
	}
	detectorPolarityUpdated();  // Force update for initial value, even if value not changed

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
	// Connect signals
	connect(timeout0, SIGNAL(valueChanged(int)), this, SLOT(timeout0Updated()));
	// Set defaults
	timeout0->setValue(2);
	timeout0Updated();  // Force update for initial value, even if value not changed

	QGroupBox *eastGroup = new QGroupBox(tr("East Direction"));
	QFormLayout *eastLayout = new QFormLayout();
	timeout1 = new QSpinBox;
	timeout1->setRange(0, 255);
	timeout1->setSingleStep(1);
	timeout1->setSuffix("s");
	eastLayout->addRow(tr("Timeout:"), timeout1);
	eastLayout->addRow(tr("Polarity:"), detectorPolarity[2]);
	eastLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	eastGroup->setLayout(eastLayout);
	// Connect signals
	connect(timeout1, SIGNAL(valueChanged(int)), this, SLOT(timeout1Updated()));
	// Set defaults
	timeout1->setValue(2);
	timeout1Updated();  // Force update for initial value, even if value not changed

	QGroupBox *northGroup = new QGroupBox(tr("North Direction"));
	QFormLayout *northLayout = new QFormLayout();
	timeout2 = new QSpinBox;
	timeout2->setRange(0, 255);
	timeout2->setSingleStep(1);
	timeout2->setSuffix("s");
	northLayout->addRow(tr("Timeout:"), timeout2);
	northLayout->addRow(tr("Polarity (Main):"), detectorPolarity[3]);
	northLayout->addRow(tr("Polarity (Siding):"), detectorPolarity[4]);
	northLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	northGroup->setLayout(northLayout);

	QGroupBox *southGroup = new QGroupBox(tr("South Direction"));
	QFormLayout *southLayout = new QFormLayout();
	timeout3 = new QSpinBox;
	timeout3->setRange(0, 255);
	timeout3->setSingleStep(1);
	timeout3->setSuffix("s");
	southLayout->addRow(tr("Timeout:"), timeout3);
	southLayout->addRow(tr("Polarity:"), detectorPolarity[5]);
	southLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	southGroup->setLayout(southLayout);
	// Connect signals
	connect(timeout2, SIGNAL(valueChanged(int)), this, SLOT(timeout2Updated()));
	// Set defaults
	timeout2->setValue(2);
	timeout2Updated();  // Force update for initial value, even if value not changed

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
	// Connect signals
	connect(timeout3, SIGNAL(valueChanged(int)), this, SLOT(timeout3Updated()));
	// Set defaults
	timeout3->setValue(2);
	timeout3Updated();  // Force update for initial value, even if value not changed

	QGridLayout *detectorLayout = new QGridLayout;
	detectorLayout->addWidget(westGroup, 1, 0);
	detectorLayout->addWidget(eastGroup, 1, 2);
	detectorLayout->addWidget(northGroup, 0, 1);
	detectorLayout->addWidget(southGroup, 2, 1);
	detectorLayout->addWidget(interlockingGroup, 1, 1);
	detectorPage->setLayout(detectorLayout);



	QWidget *interchangePage = new QWidget();
	QFormLayout *interchangeLayout = new QFormLayout();
	interchangeLayout->addRow(tr("Detector Polarity:"), detectorPolarity[7]);
	interchangePolarity = new QComboBox;
	interchangePolarity->addItem("High = Track Power Applied",0);
	interchangePolarity->addItem("Low = Track Power Applied",1);
	interchangeLayout->addRow(tr("Relay Polarity:"), interchangePolarity);
	interchangeLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	interchangePage->setLayout(interchangeLayout);



	for(int i=0; i<8; i++)
	{
		signalPolarity[i] = new QComboBox;
		signalPolarity[i]->addItem("Common Cathode",0);
		signalPolarity[i]->addItem("Common Anode",7);
	}

	QWidget *signalPage = new QWidget();

	QGroupBox *westSignalGroup = new QGroupBox(tr("West Direction"));
	QFormLayout *westSignalLayout = new QFormLayout();
	westSignalLayout->addRow(tr("Polarity (Main):"), signalPolarity[0]);
	westSignalLayout->addRow(tr("Polarity (Siding):"), signalPolarity[1]);
	westSignalLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	westSignalGroup->setLayout(westSignalLayout);

	QGroupBox *eastSignalGroup = new QGroupBox(tr("East Direction"));
	QFormLayout *eastSignalLayout = new QFormLayout();
	eastSignalLayout->addRow(tr("Polarity (Top):"), signalPolarity[2]);
	eastSignalLayout->addRow(tr("Polarity (Bottom):"), signalPolarity[3]);
	eastSignalLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	eastSignalGroup->setLayout(eastSignalLayout);

	QGroupBox *northSignalGroup = new QGroupBox(tr("North Direction"));
	QFormLayout *northSignalLayout = new QFormLayout();
	northSignalLayout->addRow(tr("Polarity (Main):"), signalPolarity[4]);
	northSignalLayout->addRow(tr("Polarity (Siding):"), signalPolarity[5]);
	northSignalLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	northSignalGroup->setLayout(northSignalLayout);

	QGroupBox *southSignalGroup = new QGroupBox(tr("South Direction"));
	QFormLayout *southSignalLayout = new QFormLayout();
	southSignalLayout->addRow(tr("Polarity (Top):"), signalPolarity[6]);
	southSignalLayout->addRow(tr("Polarity (Bottom):"), signalPolarity[7]);
	southSignalLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	southSignalGroup->setLayout(southSignalLayout);
	
	QWidget *blinkyWidget = new QWidget();
	QFormLayout *blinkyLayout = new QFormLayout();
	blinky = new QDoubleSpinBox;
	blinky->setRange(0, 25.5);
	blinky->setSingleStep(0.1);
	blinky->setDecimals(1);
	blinky->setSuffix("s");
	blinkyLayout->addRow(tr("Blink Time:"), blinky);
	blinkyLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	blinkyWidget->setLayout(blinkyLayout);

	QGridLayout *signalLayout = new QGridLayout;
	signalLayout->addWidget(westSignalGroup, 1, 0);
	signalLayout->addWidget(eastSignalGroup, 1, 2);
	signalLayout->addWidget(northSignalGroup, 0, 1);
	signalLayout->addWidget(southSignalGroup, 2, 1);
	signalLayout->addWidget(blinkyWidget, 1, 1);
	signalLayout->setAlignment(blinkyWidget, Qt::AlignCenter);
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
}


void Node_IIAB::detectorPolarityUpdated(void)
{
	for(int i=0; i<8; i++)
	{
		if(detectorPolarity[i]->itemData(detectorPolarity[i]->currentIndex()).toBool())
		{
			eeprom[EE_INPUT_POLARITY0] |= (1 << i);
		}
		else
		{
			eeprom[EE_INPUT_POLARITY0] &= ~(1 << i);
		}
	}
	updateEepromTable();
}

void Node_IIAB::timeout0Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+0] = timeout0->value();
	updateEepromTable();
}

void Node_IIAB::timeout1Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+1] = timeout1->value();
	updateEepromTable();
}

void Node_IIAB::timeout2Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+2] = timeout2->value();
	updateEepromTable();
}

void Node_IIAB::timeout3Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+3] = timeout3->value();
	updateEepromTable();
}


void Node_IIAB::node2eeprom(void)
{
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
}

void Node_IIAB::eeprom2node(void)
{
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

void Node_IIAB::write(void)
{
	// FIXME: write
}

void Node_IIAB::read(void)
{
	// FIXME: read
}



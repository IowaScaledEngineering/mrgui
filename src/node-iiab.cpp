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

#include <stdint.h>

#include "node-iiab.h"

Node_IIAB::Node_IIAB(void) : Window("atmega328p")
{
	// Create widget layout
	setStyleSheet("QGroupBox{border: 1px solid gray; border-radius:5px; font-weight: bold; margin-top: 1ex; margin-bottom: 1ex;} QGroupBox::title{subcontrol-origin: margin; subcontrol-position: top left; left: 10px; padding: 0 3px;}");

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
	timeout1 = new QSpinBox;
	timeout1->setRange(0, 255);
	timeout1->setSingleStep(1);
	timeout1->setSuffix("s");
	eastLayout->addRow(tr("Timeout:"), timeout1);
	eastLayout->addRow(tr("Polarity:"), detectorPolarity[2]);
	eastLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	eastGroup->setLayout(eastLayout);

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
	detectorLayout->addWidget(westGroup, 1, 0);
	detectorLayout->addWidget(eastGroup, 1, 2);
	detectorLayout->addWidget(northGroup, 0, 1);
	detectorLayout->addWidget(southGroup, 2, 1);
	detectorLayout->addWidget(interlockingGroup, 1, 1);
	detectorPage->setLayout(detectorLayout);



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



	QWidget *interchangePage = new QWidget();
	QFormLayout *interchangeLayout = new QFormLayout();
	interchangeLayout->addRow(tr("Detector Polarity:"), detectorPolarity[7]);
	interchangePolarity = new QComboBox;
	interchangePolarity->addItem("High = Track Power Applied",0);
	interchangePolarity->addItem("Low = Track Power Applied",1);
	interchangeLayout->addRow(tr("Relay Polarity:"), interchangePolarity);
	interchangeLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	interchangePage->setLayout(interchangeLayout);



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


	// Connect signals
	for(int i=0; i<8; i++)
	{
		// Connect signals
		connect(detectorPolarity[i], SIGNAL(currentIndexChanged(int)), this, SLOT(detectorPolarityUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(detectorPolaritySet()));
	}
	connect(timeout0, SIGNAL(valueChanged(int)), this, SLOT(timeout0Updated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(timeout0Set()));
	connect(timeout1, SIGNAL(valueChanged(int)), this, SLOT(timeout1Updated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(timeout1Set()));
	connect(timeout2, SIGNAL(valueChanged(int)), this, SLOT(timeout2Updated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(timeout2Set()));
	connect(timeout3, SIGNAL(valueChanged(int)), this, SLOT(timeout3Updated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(timeout3Set()));
	connect(debounce, SIGNAL(valueChanged(int)), this, SLOT(debounceUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(debounceSet()));
	//FIXME: signal polarity
	connect(blinky, SIGNAL(valueChanged(double)), this, SLOT(blinkyUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(blinkySet()));
	//FIXME: turnout polarity
	connect(lockout, SIGNAL(valueChanged(int)), this, SLOT(lockoutUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(lockoutSet()));
	connect(timelock, SIGNAL(valueChanged(int)), this, SLOT(timelockUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(timelockSet()));
	//FIXME: interchange polarity

	// Set defaults
	for(int i=0; i<8; i++)
	{
		detectorPolarity[i]->setCurrentIndex(0);
	}
	detectorPolarityUpdated();  // Force update for initial value, even if value not changed
	timeout0->setValue(15);
	timeout0Updated();
	timeout1->setValue(15);
	timeout1Updated();
	timeout2->setValue(15);
	timeout2Updated();
	timeout3->setValue(15);
	timeout3Updated();
	debounce->setValue(3);
	debounceUpdated();
	//FIXME: signal polarity
	blinky->setValue(0.5);
	blinkyUpdated();
	//FIXME: turnout polarity
	lockout->setValue(12);
	lockoutUpdated();
	timelock->setValue(30);
	timelockUpdated();
	//FIXME: interchange polarity
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

void Node_IIAB::detectorPolaritySet(void)
{
	// Save a copy of the eeprom value since each widget update will trigger a rewrite of the eeprom
	// This will wipe out any subsequent updates if the eeprom byte is not saved.
	uint8_t polarity = eeprom[EE_INPUT_POLARITY0];
	for(int i=0; i<8; i++)
	{
		if(polarity & (1 << i))
		{
			detectorPolarity[i]->setCurrentIndex(detectorPolarity[i]->findData(1));
		}
		else
		{
			detectorPolarity[i]->setCurrentIndex(detectorPolarity[i]->findData(0));
		}
	}
	updateEepromTable();
}

void Node_IIAB::timeout0Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+0] = timeout0->value();
	updateEepromTable();
}

void Node_IIAB::timeout0Set(void)
{
	timeout0->setValue(eeprom[EE_TIMEOUT_SECONDS+0]);
	updateEepromTable();
}

void Node_IIAB::timeout1Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+1] = timeout1->value();
	updateEepromTable();
}

void Node_IIAB::timeout1Set(void)
{
	timeout1->setValue(eeprom[EE_TIMEOUT_SECONDS+1]);
	updateEepromTable();
}

void Node_IIAB::timeout2Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+2] = timeout2->value();
	updateEepromTable();
}

void Node_IIAB::timeout2Set(void)
{
	timeout2->setValue(eeprom[EE_TIMEOUT_SECONDS+2]);
	updateEepromTable();
}

void Node_IIAB::timeout3Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+3] = timeout3->value();
	updateEepromTable();
}

void Node_IIAB::timeout3Set(void)
{
	timeout3->setValue(eeprom[EE_TIMEOUT_SECONDS+3]);
	updateEepromTable();
}

void Node_IIAB::debounceUpdated(void)
{
	eeprom[EE_DEBOUNCE_SECONDS] = debounce->value();
	updateEepromTable();
}

void Node_IIAB::debounceSet(void)
{
	debounce->setValue(eeprom[EE_DEBOUNCE_SECONDS]);
	updateEepromTable();
}

void Node_IIAB::blinkyUpdated(void)
{
	eeprom[EE_BLINKY_DECISECS] = blinky->value() * 10;
	updateEepromTable();
}

void Node_IIAB::blinkySet(void)
{
	blinky->setValue(eeprom[EE_BLINKY_DECISECS] / 10.0);
	updateEepromTable();
}

void Node_IIAB::lockoutUpdated(void)
{
	eeprom[EE_LOCKOUT_SECONDS] = lockout->value();
	updateEepromTable();
}

void Node_IIAB::lockoutSet(void)
{
	lockout->setValue(eeprom[EE_LOCKOUT_SECONDS]);
	updateEepromTable();
}

void Node_IIAB::timelockUpdated(void)
{
	eeprom[EE_TIMELOCK_SECONDS] = timelock->value();
	updateEepromTable();
}

void Node_IIAB::timelockSet(void)
{
	timelock->setValue(eeprom[EE_TIMELOCK_SECONDS]);
	updateEepromTable();
}




/*
void Node_IIAB::node2eeprom(void)
{
	eeprom[EE_CLOCK_SOURCE_ADDRESS] = clockSource->value();
	eeprom[EE_MAX_DEAD_RECKONING] = maxDeadReckoning->value() * 10;
	eeprom[EE_SIM_TRAIN_WINDOW] = simTrainWindow->value();
}

void Node_IIAB::eeprom2node(void)
{
	clockSource->setValue(eeprom[EE_CLOCK_SOURCE_ADDRESS]);
	maxDeadReckoning->setValue(eeprom[EE_MAX_DEAD_RECKONING] / 10.0);
	simTrainWindow->setValue(eeprom[EE_SIM_TRAIN_WINDOW]);
}
*/

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



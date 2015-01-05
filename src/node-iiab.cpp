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

Node_IIAB::Node_IIAB(void) : Window("atmega328")
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
		signalPolarity[i]->addItem("Common Cathode (-)",0);
		signalPolarity[i]->addItem("Common Anode (+)",7);
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
	turnoutPolarity0->addItem(tr("Low = Main, High = Siding"),0);
	turnoutPolarity0->addItem(tr("High = Main, Low = Siding"),1);
	turnoutLayout->addRow(tr("West Polarity:"), turnoutPolarity0);
	turnoutPolarity2 = new QComboBox();
	turnoutPolarity2->addItem(tr("Low = Main, High = Siding"),0);
	turnoutPolarity2->addItem(tr("High = Main, Low = Siding"),1);
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
	simTrainWindow->setSuffix(" min (fast)");
	scheduleLayout->addRow(tr("Trigger Window:"), simTrainWindow);

	// Build Simulated Train table
	int columns = 7;
	QTableWidget *simTrainSchedule = new QTableWidget(NUM_SIM_TRAINS, columns);
	QStringList simTrainHeader;
	simTrainHeader << "Enable" << "Direction" << "Start Time" << "Duration" << "Approach" << "Sound Trigger" << "Interchange";
	simTrainSchedule->setHorizontalHeaderLabels(simTrainHeader);
	simTrainSchedule->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	simTrainSchedule->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	simTrainSchedule->setSelectionMode(QAbstractItemView::NoSelection);
	QWidget *simTrainEnableWidget[NUM_SIM_TRAINS], *simTrainInterchangeWidget[NUM_SIM_TRAINS];
	QHBoxLayout *simTrainEnableLayout[NUM_SIM_TRAINS], *simTrainInterchangeLayout[NUM_SIM_TRAINS];
	for (int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainEnable[i] = new QCheckBox;
		simTrainEnableWidget[i] = new QWidget;
		simTrainEnableLayout[i] = new QHBoxLayout;
		simTrainEnableLayout[i]->addWidget(simTrainEnable[i]);
		simTrainEnableLayout[i]->setAlignment(Qt::AlignCenter);
		simTrainEnableLayout[i]->setContentsMargins(0,0,0,0);
		simTrainEnableWidget[i]->setLayout(simTrainEnableLayout[i]);
		simTrainSchedule->setCellWidget(i, 0, simTrainEnableWidget[i]);
		
		simTrainDirection[i] = new QComboBox;
		simTrainDirection[i]->addItem("Eastbound",0);
		simTrainDirection[i]->addItem("Westbound",1);
		simTrainDirection[i]->addItem("Southbound",2);
		simTrainDirection[i]->addItem("Northbound",3);
		simTrainSchedule->setCellWidget(i, 1, simTrainDirection[i]);
		
		simTrainTime[i] = new QDateTimeEdit;
		simTrainTime[i]->setDisplayFormat("hh:mm");
		simTrainTime[i]->setAlignment(Qt::AlignHCenter);
		simTrainSchedule->setCellWidget(i, 2, simTrainTime[i]);

		simTrainTotal[i] = new QSpinBox;
		simTrainTotal[i]->setRange(0, 255);
		simTrainTotal[i]->setSingleStep(1);
		simTrainTotal[i]->setSuffix("s");
		simTrainTotal[i]->setAlignment(Qt::AlignHCenter);
		simTrainSchedule->setCellWidget(i, 3, simTrainTotal[i]);

		simTrainApproach[i] = new QSpinBox;
		simTrainApproach[i]->setRange(0, 255);
		simTrainApproach[i]->setSingleStep(1);
		simTrainApproach[i]->setSuffix("s");
		simTrainApproach[i]->setAlignment(Qt::AlignHCenter);
		simTrainSchedule->setCellWidget(i, 4, simTrainApproach[i]);

		simTrainSound[i] = new QComboBox;
		simTrainSound[i]->addItem("None",0);
		simTrainSound[i]->addItem("Sound 0",1);
		simTrainSound[i]->addItem("Sound 1",2);
		simTrainSound[i]->addItem("Both",3);
		simTrainSchedule->setCellWidget(i, 5, simTrainSound[i]);
		
		simTrainInterchange[i] = new QCheckBox;
		simTrainInterchangeWidget[i] = new QWidget;
		simTrainInterchangeLayout[i] = new QHBoxLayout;
		simTrainInterchangeLayout[i]->addWidget(simTrainInterchange[i]);
		simTrainInterchangeLayout[i]->setAlignment(Qt::AlignCenter);
		simTrainInterchangeLayout[i]->setContentsMargins(0,0,0,0);
		simTrainInterchangeWidget[i]->setLayout(simTrainInterchangeLayout[i]);
		simTrainSchedule->setCellWidget(i, 6, simTrainInterchangeWidget[i]);
	}
	int width = 0;
	for(int i=0; i<columns; i++)
	{
		width += simTrainSchedule->columnWidth(i);
	}
	width += simTrainSchedule->verticalHeader()->sectionSize(0);  // Add width of one section of the header
	width += simTrainSchedule->verticalScrollBar()->sizeHint().width();
	width += 10;
	simTrainSchedule->setMinimumWidth(width);
	
	QSizePolicy schedulePolicy = simTrainSchedule->sizePolicy();
	schedulePolicy.setVerticalStretch(true);
	simTrainSchedule->setSizePolicy(schedulePolicy);

	scheduleLayout->addRow(tr("Simulated Trains:"), simTrainSchedule);
	scheduleLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	schedulePage->setLayout(scheduleLayout);



	tabWidget->insertTab(0, detectorPage, "Detectors");
	tabWidget->insertTab(1, signalPage, "Signals");
	tabWidget->insertTab(2, turnoutPage, "Turnouts");
	tabWidget->insertTab(3, timingPage, "Timing");
	tabWidget->insertTab(4, interchangePage, "Interchange");
	tabWidget->insertTab(5, schedulePage, "Schedules");
	tabWidget->setCurrentIndex(0);

	setDefaults();

	// Connect signals (after defaults set to avoid excess calls)
	for(int i=0; i<8; i++)
	{
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
	for(int i=0; i<8; i++)
	{
		connect(signalPolarity[i], SIGNAL(currentIndexChanged(int)), this, SLOT(signalPolarityUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(signalPolaritySet()));
	}
	connect(blinky, SIGNAL(valueChanged(double)), this, SLOT(blinkyUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(blinkySet()));
	connect(turnoutPolarity0, SIGNAL(currentIndexChanged(int)), this, SLOT(turnoutPolarityUpdated()));
	connect(turnoutPolarity2, SIGNAL(currentIndexChanged(int)), this, SLOT(turnoutPolarityUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(turnoutPolaritySet()));
	connect(lockout, SIGNAL(valueChanged(int)), this, SLOT(lockoutUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(lockoutSet()));
	connect(timelock, SIGNAL(valueChanged(int)), this, SLOT(timelockUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(timelockSet()));
	connect(interchangePolarity, SIGNAL(currentIndexChanged(int)), this, SLOT(interchangePolarityUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(interchangePolaritySet()));

	connect(clockSource, SIGNAL(valueChanged(int)), this, SLOT(clockSourceUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(clockSourceSet()));
	connect(maxDeadReckoning, SIGNAL(valueChanged(double)), this, SLOT(maxDeadReckoningUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(maxDeadReckoningSet()));
	connect(simTrainWindow, SIGNAL(valueChanged(int)), this, SLOT(simTrainWindowUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainWindowSet()));
	for (int i=0; i<NUM_SIM_TRAINS; i++)
	{
		connect(simTrainEnable[i], SIGNAL(stateChanged(int)), this, SLOT(simTrainEnableUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainEnableSet()));
		
		connect(simTrainDirection[i], SIGNAL(currentIndexChanged(int)), this, SLOT(simTrainDirectionUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainDirectionSet()));
		
		connect(simTrainTime[i], SIGNAL(timeChanged(QTime)), this, SLOT(simTrainTimeUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainTimeSet()));

		connect(simTrainTotal[i], SIGNAL(valueChanged(int)), this, SLOT(simTrainTotalUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainTotalSet()));

		connect(simTrainApproach[i], SIGNAL(valueChanged(int)), this, SLOT(simTrainApproachUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainApproachSet()));

		connect(simTrainSound[i], SIGNAL(currentIndexChanged(int)), this, SLOT(simTrainSoundUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainSoundSet()));
		
		connect(simTrainInterchange[i], SIGNAL(stateChanged(int)), this, SLOT(simTrainInterchangeUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(simTrainInterchangeSet()));
	}
	
	// Connect to reset action
	connect(this, SIGNAL(resetDefaults()), this, SLOT(setDefaults()));
}


void Node_IIAB::setDefaults(void)
{
	// Set unused bits to zero
	eeprom[EE_INPUT_POLARITY0] = 0;
	eeprom[EE_INPUT_POLARITY1] = 0;
	eeprom[EE_OUTPUT_POLARITY0] = 0;
	eeprom[EE_OUTPUT_POLARITY1] = 0;
	eeprom[EE_OUTPUT_POLARITY2] = 0;
	eeprom[EE_OUTPUT_POLARITY3] = 0;
	eeprom[EE_OUTPUT_POLARITY4] = 0;
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] = 0;
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_DIRECTION] = 0;
	}

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
	for(int i=0; i<8; i++)
	{
		signalPolarity[i]->setCurrentIndex(0);
	}
	signalPolarityUpdated();
	blinky->setValue(0.5);
	blinkyUpdated();
	turnoutPolarity0->setCurrentIndex(0);
	turnoutPolarity2->setCurrentIndex(0);
	lockout->setValue(12);
	lockoutUpdated();
	timelock->setValue(30);
	timelockUpdated();
	interchangePolarity->setCurrentIndex(0);
	interchangePolarityUpdated();

	clockSource->setValue(0xFF);
	clockSourceUpdated();
	maxDeadReckoning->setValue(10);
	maxDeadReckoningUpdated();
	simTrainWindow->setValue(1);
	simTrainWindowUpdated();
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainEnable[i]->setChecked(false);
		simTrainDirection[i]->setCurrentIndex(0);
		simTrainTime[i]->setTime(QTime(0,0));
		simTrainTotal[i]->setValue(30);
		simTrainApproach[i]->setValue(10);
		simTrainSound[i]->setCurrentIndex(0);
		simTrainInterchange[i]->setChecked(false);
	}
	simTrainEnableUpdated();
	simTrainDirectionUpdated();
	simTrainTimeUpdated();
	simTrainTotalUpdated();
	simTrainApproachUpdated();
	simTrainSoundUpdated();
	simTrainInterchangeUpdated();
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
	for(int i=0; i<8; i++)
	{
		detectorPolarity[i]->blockSignals(true);
	}

	for(int i=0; i<8; i++)
	{
		if(eeprom[EE_INPUT_POLARITY0] & (1 << i))
		{
			detectorPolarity[i]->setCurrentIndex(detectorPolarity[i]->findData(1));
		}
		else
		{
			detectorPolarity[i]->setCurrentIndex(detectorPolarity[i]->findData(0));
		}
	}

	for(int i=0; i<8; i++)
	{
		detectorPolarity[i]->blockSignals(false);
	}
}

void Node_IIAB::turnoutPolarityUpdated(void)
{
	if(turnoutPolarity0->itemData(turnoutPolarity0->currentIndex()).toBool())
	{
		eeprom[EE_INPUT_POLARITY1] |= 0x01;
	}
	else
	{
		eeprom[EE_INPUT_POLARITY1] &= ~(0x01);
	}

	if(turnoutPolarity2->itemData(turnoutPolarity2->currentIndex()).toBool())
	{
		eeprom[EE_INPUT_POLARITY1] |= 0x04;
	}
	else
	{
		eeprom[EE_INPUT_POLARITY1] &= ~(0x04);
	}
	
	updateEepromTable();
}

void Node_IIAB::turnoutPolaritySet(void)
{
	turnoutPolarity0->blockSignals(true);
	turnoutPolarity2->blockSignals(true);

	if(eeprom[EE_INPUT_POLARITY1] & 0x01)
	{
		turnoutPolarity0->setCurrentIndex(turnoutPolarity0->findData(1));
	}
	else
	{
		turnoutPolarity0->setCurrentIndex(turnoutPolarity0->findData(0));
	}

	if(eeprom[EE_INPUT_POLARITY1] & 0x04)
	{
		turnoutPolarity2->setCurrentIndex(turnoutPolarity2->findData(1));
	}
	else
	{
		turnoutPolarity2->setCurrentIndex(turnoutPolarity2->findData(0));
	}

	turnoutPolarity0->blockSignals(false);
	turnoutPolarity2->blockSignals(false);
}

void Node_IIAB::interchangePolarityUpdated(void)
{
	if(interchangePolarity->itemData(interchangePolarity->currentIndex()).toBool())
	{
		eeprom[EE_OUTPUT_POLARITY4] |= 0x20;
	}
	else
	{
		eeprom[EE_OUTPUT_POLARITY4] &= ~(0x20);
	}

	updateEepromTable();
}

void Node_IIAB::interchangePolaritySet(void)
{
	interchangePolarity->blockSignals(true);

	if(eeprom[EE_OUTPUT_POLARITY4] & 0x20)
	{
		interchangePolarity->setCurrentIndex(interchangePolarity->findData(1));
	}
	else
	{
		interchangePolarity->setCurrentIndex(interchangePolarity->findData(0));
	}

	interchangePolarity->blockSignals(false);
}

void Node_IIAB::signalPolarityUpdated(void)
{
	uint32_t polarity = 0;
	for(int i=0; i<8; i++)
	{
		polarity |= (signalPolarity[i]->itemData(signalPolarity[i]->currentIndex()).toInt()) << (3*i);
	}
	eeprom[EE_OUTPUT_POLARITY0] = (polarity >>  0) & 0xFF;
	eeprom[EE_OUTPUT_POLARITY1] = (polarity >>  8) & 0xFF;
	eeprom[EE_OUTPUT_POLARITY2] = (polarity >> 16) & 0xFF;
	updateEepromTable();
}

void Node_IIAB::signalPolaritySet(void)
{
	for(int i=0; i<8; i++)
	{
		signalPolarity[i]->blockSignals(true);
	}

	uint32_t polarity = (eeprom[EE_OUTPUT_POLARITY2] << 16) + (eeprom[EE_OUTPUT_POLARITY1] << 8) + (eeprom[EE_OUTPUT_POLARITY0]);
	for(int i=0; i<8; i++)
	{
		uint8_t bits = (polarity >> (3*i)) & 0x07;
		signalPolarity[i]->setCurrentIndex(signalPolarity[i]->findData(bits));
	}

	for(int i=0; i<8; i++)
	{
		signalPolarity[i]->blockSignals(false);
	}
}

void Node_IIAB::timeout0Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+0] = timeout0->value();
	updateEepromTable();
}

void Node_IIAB::timeout0Set(void)
{
	timeout0->setValue(eeprom[EE_TIMEOUT_SECONDS+0]);
}

void Node_IIAB::timeout1Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+1] = timeout1->value();
	updateEepromTable();
}

void Node_IIAB::timeout1Set(void)
{
	timeout1->setValue(eeprom[EE_TIMEOUT_SECONDS+1]);
}

void Node_IIAB::timeout2Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+2] = timeout2->value();
	updateEepromTable();
}

void Node_IIAB::timeout2Set(void)
{
	timeout2->setValue(eeprom[EE_TIMEOUT_SECONDS+2]);
}

void Node_IIAB::timeout3Updated(void)
{
	eeprom[EE_TIMEOUT_SECONDS+3] = timeout3->value();
	updateEepromTable();
}

void Node_IIAB::timeout3Set(void)
{
	timeout3->setValue(eeprom[EE_TIMEOUT_SECONDS+3]);
}

void Node_IIAB::debounceUpdated(void)
{
	eeprom[EE_DEBOUNCE_SECONDS] = debounce->value();
	updateEepromTable();
}

void Node_IIAB::debounceSet(void)
{
	debounce->setValue(eeprom[EE_DEBOUNCE_SECONDS]);
}

void Node_IIAB::blinkyUpdated(void)
{
	eeprom[EE_BLINKY_DECISECS] = blinky->value() * 10;
	updateEepromTable();
}

void Node_IIAB::blinkySet(void)
{
	blinky->setValue(eeprom[EE_BLINKY_DECISECS] / 10.0);
}

void Node_IIAB::lockoutUpdated(void)
{
	eeprom[EE_LOCKOUT_SECONDS] = lockout->value();
	updateEepromTable();
}

void Node_IIAB::lockoutSet(void)
{
	lockout->setValue(eeprom[EE_LOCKOUT_SECONDS]);
}

void Node_IIAB::timelockUpdated(void)
{
	eeprom[EE_TIMELOCK_SECONDS] = timelock->value();
	updateEepromTable();
}

void Node_IIAB::timelockSet(void)
{
	timelock->setValue(eeprom[EE_TIMELOCK_SECONDS]);
}

void Node_IIAB::clockSourceUpdated(void)
{
	eeprom[EE_CLOCK_SOURCE_ADDRESS] = clockSource->value();
	updateEepromTable();
}

void Node_IIAB::clockSourceSet(void)
{
	clockSource->setValue(eeprom[EE_CLOCK_SOURCE_ADDRESS]);
}

void Node_IIAB::maxDeadReckoningUpdated(void)
{
	eeprom[EE_MAX_DEAD_RECKONING] = maxDeadReckoning->value() * 10.0;
	updateEepromTable();
}

void Node_IIAB::maxDeadReckoningSet(void)
{
	maxDeadReckoning->setValue(eeprom[EE_MAX_DEAD_RECKONING] / 10.0);
}

void Node_IIAB::simTrainWindowUpdated(void)
{
	eeprom[EE_SIM_TRAIN_WINDOW] = simTrainWindow->value();
	updateEepromTable();
}

void Node_IIAB::simTrainWindowSet(void)
{
	simTrainWindow->setValue(eeprom[EE_SIM_TRAIN_WINDOW]);
}

void Node_IIAB::simTrainEnableUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		if(simTrainEnable[i]->isChecked())
			eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] |= SIM_TRAIN_FLAGS_ENABLE;
		else
			eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] &= ~SIM_TRAIN_FLAGS_ENABLE;
	}
	updateEepromTable();
}

void Node_IIAB::simTrainEnableSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainEnable[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		if(eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] & SIM_TRAIN_FLAGS_ENABLE)
			simTrainEnable[i]->setChecked(true);
		else
			simTrainEnable[i]->setChecked(false);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainEnable[i]->blockSignals(false);
	}
}

void Node_IIAB::simTrainDirectionUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		uint8_t direction = eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_DIRECTION];
		direction &= ~SIM_TRAIN_DIRECTION_BITMASK;
		direction |= simTrainDirection[i]->itemData(simTrainDirection[i]->currentIndex()).toInt();
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_DIRECTION] = direction;
	}
	updateEepromTable();
}

void Node_IIAB::simTrainDirectionSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainDirection[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		uint8_t direction = eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_DIRECTION] & SIM_TRAIN_DIRECTION_BITMASK;
		simTrainDirection[i]->setCurrentIndex(simTrainDirection[i]->findData(direction));
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainDirection[i]->blockSignals(false);
	}
}

void Node_IIAB::simTrainTimeUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		uint8_t hours = simTrainTime[i]->time().hour();
		uint8_t minutes = simTrainTime[i]->time().minute();
		uint16_t time = (hours * 60) + minutes;
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_TIME + 0] = (time >> 8) & 0xFF;
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_TIME + 1] = (time) & 0xFF;
	}
	updateEepromTable();
}

void Node_IIAB::simTrainTimeSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainTime[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		uint16_t time = (eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_TIME + 0] << 8) + eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_TIME + 1];
		if(time > (23*60 + 59))
		{
			time = 23*60 + 59;
		}
		uint8_t hours = time / 60;
		uint8_t minutes = time % 60;
		simTrainTime[i]->setTime(QTime(hours, minutes));
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainTime[i]->blockSignals(false);
	}
}

void Node_IIAB::simTrainTotalUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_TOTAL] = simTrainTotal[i]->value();
	}
	updateEepromTable();
}

void Node_IIAB::simTrainTotalSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainTotal[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainTotal[i]->setValue(eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_TOTAL]);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainTotal[i]->blockSignals(false);
	}
}

void Node_IIAB::simTrainApproachUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_APPROACH] = simTrainApproach[i]->value();
	}
	updateEepromTable();
}

void Node_IIAB::simTrainApproachSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainApproach[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainApproach[i]->setValue(eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_APPROACH]);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainApproach[i]->blockSignals(false);
	}
}

void Node_IIAB::simTrainSoundUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		uint8_t flags = eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS];
		flags &= ~SIM_TRAIN_FLAGS_SOUND;
		flags |= simTrainSound[i]->itemData(simTrainSound[i]->currentIndex()).toInt();
		eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] = flags;
	}
	updateEepromTable();
}

void Node_IIAB::simTrainSoundSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainSound[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainSound[i]->setCurrentIndex(simTrainSound[i]->findData(eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] & SIM_TRAIN_FLAGS_SOUND));
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainSound[i]->blockSignals(false);
	}
}

void Node_IIAB::simTrainInterchangeUpdated(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		if(simTrainInterchange[i]->isChecked())
			eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] |= SIM_TRAIN_FLAGS_INTERCHANGE;
		else
			eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] &= ~SIM_TRAIN_FLAGS_INTERCHANGE;
	}
	updateEepromTable();
}

void Node_IIAB::simTrainInterchangeSet(void)
{
	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainInterchange[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		if(eeprom[EE_SIM_TRAINS + (6*i) + SIM_TRAIN_FLAGS] & SIM_TRAIN_FLAGS_INTERCHANGE)
			simTrainInterchange[i]->setChecked(true);
		else
			simTrainInterchange[i]->setChecked(false);
	}

	for(int i=0; i<NUM_SIM_TRAINS; i++)
	{
		simTrainInterchange[i]->blockSignals(false);
	}
}



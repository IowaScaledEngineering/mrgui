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
	QLabel *nodeAddrLabel = new QLabel(tr("Node Address:"));
	nodeAddr = new HexSpinBox;
	nodeAddr->setValue(0);
	nodeAddr->setRange(1,254);
	nodeAddr->setPrefix("0x");

	QLabel *updateIntervalLabel = new QLabel(tr("Update Interval:"));
	updateInterval = new QDoubleSpinBox;
	updateInterval->setRange(0.0, 6553.5);
	updateInterval->setSingleStep(1.0);
	updateInterval->setDecimals(1);
	updateInterval->setValue(5.0);
	updateInterval->setSuffix("s");

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

	QLabel *lockoutLabel = new QLabel(tr("Lockout Time:"));
	lockout = new QSpinBox;
	lockout->setRange(0, 255);
	lockout->setSingleStep(1);
	lockout->setSuffix("s");

	QLabel *timelockLabel = new QLabel(tr("Timelock Time:"));
	timelock = new QSpinBox;
	timelock->setRange(0, 255);
	timelock->setSingleStep(1);
	timelock->setSuffix("s");

	QLabel *debounceLabel = new QLabel(tr("Debounce Time:"));
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

	QLabel *blinkyLabel = new QLabel(tr("Blink Period:"));
	blinky = new QSpinBox;
	blinky->setRange(0, 255);
	blinky->setSingleStep(1);
	blinky->setSuffix("s");

	QLabel *turnoutPolarityLabel = new QLabel(tr("Turnout Polarity:"));
	QLabel *turnoutPolarity0Label = new QLabel(tr("West"));
	turnoutPolarity0 = new QCheckBox(tr("Low = Main, High = Siding"));
	turnoutPolarity0->setChecked(false);
	connect(turnoutPolarity0, SIGNAL(stateChanged(int)), this, SLOT(turnoutPolarityText()));
	QLabel *turnoutPolarity2Label = new QLabel(tr("North"));
	turnoutPolarity2 = new QCheckBox(tr("Low = Main, High = Siding"));
	turnoutPolarity2->setChecked(false);
	connect(turnoutPolarity2, SIGNAL(stateChanged(int)), this, SLOT(turnoutPolarityText()));

	QLabel *detectorPolarityLabel = new QLabel(tr("Detector Polarity:"));
	detectorPolarity = new QComboBox();
	detectorPolarity->addItem("High = Train Present");
	detectorPolarity->addItem("Low = Train Present");

	QButtonGroup *ledPolarityGroup = new QButtonGroup();
	QLabel *ledPolarityLabel = new QLabel(tr("LED Polarity:"));
	ledPolarityAnode = new QRadioButton("Common Anode");
	ledPolarityAnode->setChecked(true);
	ledPolarityCathode = new QRadioButton("Common Cathode");
	ledPolarityGroup->addButton(ledPolarityAnode);
	ledPolarityGroup->addButton(ledPolarityCathode);

//#define EE_INPUT_POLARITY0      0x20
//#define EE_INPUT_POLARITY1      0x21
//#define EE_SIGNAL_CONFIG        0x30

	QPushButton *writeButton = new QPushButton(tr("&Write"));
	writeButton->setFocusPolicy(Qt::NoFocus);
	connect(writeButton, SIGNAL(clicked()), this, SLOT(write()));

	QPushButton *readButton = new QPushButton(tr("&Read"));
	readButton->setFocusPolicy(Qt::NoFocus);
	connect(readButton, SIGNAL(clicked()), this, SLOT(read()));

	// Create widget layout
	QGroupBox *mrbusCommonGroup = new QGroupBox(tr("MRBus Common"));
	QHBoxLayout *mrbusCommonLayout = new QHBoxLayout;
	mrbusCommonLayout->addWidget(nodeAddrLabel, 0);
	mrbusCommonLayout->addWidget(nodeAddr, 0);
	mrbusCommonLayout->addSpacing(10);
	mrbusCommonLayout->addWidget(updateIntervalLabel, 0);
	mrbusCommonLayout->addWidget(updateInterval, 0);
	mrbusCommonLayout->addStretch(1);  // Add sacrificial stretch space to end
	mrbusCommonGroup->setLayout(mrbusCommonLayout);

	QGroupBox *iiabGroup = new QGroupBox(tr("Interlocking-In-A-Box"));
	QGridLayout *iiabLayout = new QGridLayout;
	
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
	
	QHBoxLayout *clockLayout = new QHBoxLayout;
	clockLayout->addWidget(clockSourceLabel);
	clockLayout->addWidget(clockSource);
	clockLayout->addSpacing(10);
	clockLayout->addWidget(maxDeadReckoningLabel);
	clockLayout->addWidget(maxDeadReckoning);

	QHBoxLayout *turnoutPolarityLayout = new QHBoxLayout;
	turnoutPolarityLayout->addWidget(turnoutPolarity0Label);
	turnoutPolarityLayout->addWidget(turnoutPolarity0);
	turnoutPolarityLayout->addSpacing(10);
	turnoutPolarityLayout->addWidget(turnoutPolarity2Label);
	turnoutPolarityLayout->addWidget(turnoutPolarity2);
	
	QHBoxLayout *ledPolarityLayout = new QHBoxLayout;
	ledPolarityLayout->addWidget(ledPolarityAnode);
	ledPolarityLayout->addWidget(ledPolarityCathode);

	iiabLayout->addWidget(timeoutLabel, 0, 0);
	iiabLayout->addLayout(timeoutLayout, 0, 1, 1, 2, Qt::AlignLeft);
	iiabLayout->addWidget(lockoutLabel, 1, 0);
	iiabLayout->addWidget(lockout, 1, 1, Qt::AlignLeft);
	iiabLayout->addWidget(timelockLabel, 2, 0);
	iiabLayout->addWidget(timelock, 2, 1, Qt::AlignLeft);
	iiabLayout->addWidget(debounceLabel, 3, 0);
	iiabLayout->addWidget(debounce, 3, 1, Qt::AlignLeft);
	iiabLayout->addWidget(clockLabel, 4, 0);
	iiabLayout->addLayout(clockLayout, 4, 1, Qt::AlignLeft);
	iiabLayout->addWidget(blinkyLabel, 5, 0);
	iiabLayout->addWidget(blinky, 5, 1, Qt::AlignLeft);
	iiabLayout->addWidget(detectorPolarityLabel, 6, 0);
	iiabLayout->addWidget(detectorPolarity, 6, 1, Qt::AlignLeft);
	iiabLayout->addWidget(turnoutPolarityLabel, 7, 0);
	iiabLayout->addLayout(turnoutPolarityLayout, 7, 1, Qt::AlignLeft);
	iiabLayout->addWidget(ledPolarityLabel, 8, 0);
	iiabLayout->addLayout(ledPolarityLayout, 8, 1, Qt::AlignLeft);
	iiabLayout->setColumnStretch(0, 0);
	iiabLayout->setColumnStretch(1, 1);
	iiabGroup->setLayout(iiabLayout);

	QGroupBox *buttonsGroup = new QGroupBox(tr("Actions"));
	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget(writeButton);
	buttonsLayout->addWidget(readButton);
	buttonsLayout->addStretch(1);  // Add sacrificial stretch space to end
	buttonsGroup->setLayout(buttonsLayout);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(mrbusCommonGroup);
	layout->addWidget(iiabGroup);
	layout->addWidget(buttonsGroup);

	setStyleSheet("QGroupBox{border: 1px solid gray; border-radius:5px; margin-top: 1ex; font-weight: bold;} QGroupBox::title{subcontrol-origin: margin; subcontrol-position: top left; padding: 0 3px;}");
	setLayout(layout);
	setWindowTitle(tr("MRGui Programmer"));
}

void Window::turnoutPolarityText(void)
{
	if(turnoutPolarity0->isChecked())
		turnoutPolarity0->setText("High = Main, Low = Diverging");
	else
		turnoutPolarity0->setText("Low = Main, High = Diverging");

	if(turnoutPolarity2->isChecked())
		turnoutPolarity2->setText("High = Main, Low = Diverging");
	else
		turnoutPolarity2->setText("Low = Main, High = Diverging");
}

void Window::write(void)
{
	char buffer[256];
	int byte;

	sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -W 0,%d", nodeAddr->value());
	if(system(buffer)) {};

	printf("value: %f\n", (double)updateInterval->value());
	byte = (int)((updateInterval->value() * 10) / 256) & 0xFF;
	printf("byte: %d\n", byte);
	sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -W 2,%d", byte);
	if(system(buffer)) {};
	byte = (int)(updateInterval->value() * 10) & 0xFF;
	printf("byte: %d\n", byte);
	sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -W 3,%d", byte);
	if(system(buffer)) {};
}

void Window::read(void)
{
	nodeAddr->setValue(readByte(0x00));
	updateInterval->setValue(readWord(0x02) / 10.0);
}

int Window::readByte(int addr)
{
	FILE *fp;
	char buffer[256];
	int value = 0;

	sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -R %d,1 | grep ^%04X", addr, addr);
	printf("%s\n", buffer);
	fp = popen(buffer, "r");
	if (fp != NULL)
	{
		while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
			sscanf(buffer, "%x %x", &addr, &value);
		}
	}
	fclose(fp);
	return (value);
}

int Window::readWord(int addr)
{
	FILE *fp;
	char buffer[256];
	int value = 0, value2 = 0;

	sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -R %d,2 | grep ^%04X", addr, addr);
	printf("%s\n", buffer);
	fp = popen(buffer, "r");
	if (fp != NULL)
	{
		while (fgets(buffer, sizeof(buffer), fp) != NULL)
		{
			sscanf(buffer, "%x %x %x", &addr, &value, &value2);
		}
	}
	fclose(fp);
	return ((value*256)+value2);
}


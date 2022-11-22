/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-irsense.cpp
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2022 Michael Petersen & Nathan Holmes

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

#include "node-irsense.h"

Node_IRSENSE::Node_IRSENSE(void) : Window("attiny13")
{
	proximityThreshold = new QSpinBox;
	proximityThreshold->setRange(0,65535);

	QWidget *proxWidget = new QWidget;
	QFormLayout *proxLayout = new QFormLayout;
	proxLayout->addRow(tr("Proximity Threshold:"), proximityThreshold);
	proxLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
	proxWidget->setLayout(proxLayout);

	QWidget *irsensePage = new QWidget();
	QVBoxLayout *irsenseLayout = new QVBoxLayout;
	irsenseLayout->addWidget(proxWidget);
	irsensePage->setLayout(irsenseLayout);

	tabWidget->insertTab(0, irsensePage, "Threshold Configuration");

	// Disable and get these widgets out of the way
	nodeAddr->setValue(0xFF);
	nodeAddr->setEnabled(false);
	transmitInterval->setValue(6553.5);
	transmitInterval->setEnabled(false);

	setDefaults();

	// Connect signals (after defaults set to avoid excess calls)
	connect(proximityThreshold, SIGNAL(valueChanged(int)), this, SLOT(proximityThresholdUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(proximityThresholdSet()));
	
	// Connect to reset action
	connect(this, SIGNAL(resetDefaults()), this, SLOT(setDefaults()));
}


void Node_IRSENSE::setDefaults(void)
{
	// Set defaults
	proximityThreshold->setValue(0x300);

	proximityThresholdUpdated();
}

void Node_IRSENSE::proximityThresholdUpdated(void)
{
	eeprom[EE_PROX_THR_HIGH] = proximityThreshold->value() >> 8;
	eeprom[EE_PROX_THR_LOW]  = proximityThreshold->value() & 0xFF;
	updateEepromTable();
}

void Node_IRSENSE::proximityThresholdSet(void)
{
	proximityThreshold->blockSignals(true);
	proximityThreshold->setValue(eeprom[EE_PROX_THR_HIGH] *256 + eeprom[EE_PROX_THR_LOW]);
	proximityThreshold->blockSignals(false);
}



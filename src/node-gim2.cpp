/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-gim2.cpp
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

#include "node-gim2.h"

Node_GIM2::Node_GIM2(void) : Window("atmega328")
{
	// Build table
	int columns = 4;
	QTableWidget *gimConfig = new QTableWidget(NUM_OUTPUTS, columns);
	QStringList gimHeader;
	gimHeader << "Source Address" << "Packet Type" << "Byte" << "Bit";
	gimConfig->setHorizontalHeaderLabels(gimHeader);
	gimConfig->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	QStringList gimOutputs;
	for(int x=0; x<8; x++)
	{
		for(char y='A'; y<'H'; y++)
		{
			gimOutputs << QString("%1:%2").arg(x).arg(y);
		}
		gimOutputs << QString("%1:DP").arg(x);
	}
	gimConfig->setVerticalHeaderLabels(gimOutputs);
	gimConfig->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	gimConfig->setSelectionMode(QAbstractItemView::NoSelection);
	QWidget *packetTypeWidget[NUM_OUTPUTS];
	QHBoxLayout *packetTypeLayout[NUM_OUTPUTS];
	for (int i=0; i<NUM_OUTPUTS; i++)
	{
		sourceAddress[i] = new HexSpinBox;
		sourceAddress[i]->setRange(0,255);
		sourceAddress[i]->setPrefix("0x");
		sourceAddress[i]->setAlignment(Qt::AlignCenter);

		gimConfig->setCellWidget(i, 0, sourceAddress[i]);
		
		packetTypeText[i] = new QLineEdit;
		packetTypeText[i]->setMaxLength(1);
		packetTypeText[i]->setAlignment(Qt::AlignCenter);
		packetTypeHex[i] = new HexSpinBox;
		packetTypeHex[i]->setRange(0,255);
		packetTypeHex[i]->setPrefix("0x");
		packetTypeHex[i]->setAlignment(Qt::AlignCenter);
		packetTypeWidget[i] = new QWidget;
		packetTypeLayout[i] = new QHBoxLayout;
		packetTypeLayout[i]->addWidget(packetTypeText[i]);
		packetTypeLayout[i]->addWidget(packetTypeHex[i]);
		packetTypeLayout[i]->setContentsMargins(0,0,0,0);
		packetTypeWidget[i]->setLayout(packetTypeLayout[i]);
		gimConfig->setCellWidget(i, 1, packetTypeWidget[i]);
		
		listenByte[i] = new QSpinBox;
		listenByte[i]->setRange(6,19);
		gimConfig->setCellWidget(i, 2, listenByte[i]);

		listenBit[i] = new QSpinBox;
		listenBit[i]->setRange(0,7);
		gimConfig->setCellWidget(i, 3, listenBit[i]);
	}
	int width = 0;
	for(int i=0; i<columns; i++)
	{
		width += gimConfig->columnWidth(i);
	}
	width += gimConfig->verticalHeader()->sectionSize(0);  // Add width of one section of the header
	width += gimConfig->verticalScrollBar()->sizeHint().width();
	width += 10;
	gimConfig->setMinimumWidth(width);
	
	QSizePolicy gimConfigPolicy = gimConfig->sizePolicy();
	gimConfigPolicy.setVerticalStretch(true);
	gimConfig->setSizePolicy(gimConfigPolicy);

	QWidget *gimPage = new QWidget();
	QVBoxLayout *gimLayout = new QVBoxLayout;
	gimLayout->addWidget(gimConfig);
	gimPage->setLayout(gimLayout);

	tabWidget->insertTab(0, gimPage, "Output Configuration");

	setDefaults();

	// Connect signals (after defaults set to avoid excess calls)
	for(int i=0; i<8; i++)
	{
		connect(sourceAddress[i], SIGNAL(valueChanged(int)), this, SLOT(sourceAddressUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(sourceAddressSet()));
		connect(packetTypeText[i], SIGNAL(textEdited(QString)), this, SLOT(packetTypeTextUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(packetTypeTextSet()));
		connect(packetTypeHex[i], SIGNAL(valueChanged(int)), this, SLOT(packetTypeHexUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(packetTypeHexSet()));
		connect(listenByte[i], SIGNAL(valueChanged(int)), this, SLOT(listenByteUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(listenByteSet()));
		connect(listenBit[i], SIGNAL(valueChanged(int)), this, SLOT(listenBitUpdated()));
		connect(this, SIGNAL(eepromUpdated()), this, SLOT(listenBitSet()));
	}
	
	// Connect to reset action
	connect(this, SIGNAL(resetDefaults()), this, SLOT(setDefaults()));
}


void Node_GIM2::setDefaults(void)
{
	// Set defaults
	for (int i=0; i<NUM_OUTPUTS; i++)
	{
		sourceAddress[i]->setValue(0);
		packetTypeText[i]->setText("");
		packetTypeHex[i]->setValue(0);
		listenByte[i]->setValue(6);
		listenBit[i]->setValue(0);
	}

	sourceAddressUpdated();
	packetTypeTextUpdated();
	packetTypeHexUpdated();
	listenByteUpdated();
	listenBitUpdated();
	
	nodeAddr->setValue(0x20);
	nodeAddrUpdated();  // Force update for initial value, even if value not changed
}

void Node_GIM2::sourceAddressUpdated(void)
{
	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		eeprom[EE_OCC_ADDR+i] = sourceAddress[i]->value();
	}
	updateEepromTable();
}

void Node_GIM2::sourceAddressSet(void)
{
	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		sourceAddress[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		sourceAddress[i]->setValue(eeprom[EE_OCC_ADDR+i]);
	}

	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		sourceAddress[i]->blockSignals(false);
	}
}

void Node_GIM2::packetTypeTextUpdated(void)
{
	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		eeprom[EE_OCC_PKT+i] = packetTypeText[i]->text()[0].toLatin1();
	}
	updateEepromTable();
	packetTypeHexSet();
}

void Node_GIM2::packetTypeTextSet(void)
{
	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		packetTypeText[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		packetTypeText[i]->setText(QChar(eeprom[EE_OCC_PKT+i]));
	}

	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		packetTypeText[i]->blockSignals(false);
	}
}

void Node_GIM2::packetTypeHexUpdated(void)
{
	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		eeprom[EE_OCC_PKT+i] = packetTypeHex[i]->value();
	}
	updateEepromTable();
	packetTypeTextSet();
}

void Node_GIM2::packetTypeHexSet(void)
{
	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		packetTypeHex[i]->blockSignals(true);
	}

	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		packetTypeHex[i]->setValue(eeprom[EE_OCC_PKT+i]);
	}

	for(int i=0; i<NUM_OUTPUTS; i++)
	{
		packetTypeHex[i]->blockSignals(false);
	}
}

void Node_GIM2::listenByteUpdated(void)
{

}

void Node_GIM2::listenByteSet(void)
{

}

void Node_GIM2::listenBitUpdated(void)
{

}

void Node_GIM2::listenBitSet(void)
{

}


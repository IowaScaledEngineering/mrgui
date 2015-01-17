/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-generic.cpp
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

#include "node-generic.h"

Node_Generic::Node_Generic(const char* device) : Window(device)
{
	// Create widget layout
	setStyleSheet("QGroupBox{border: 1px solid gray; border-radius:5px; font-weight: bold; margin-top: 1ex; margin-bottom: 1ex;} QGroupBox::title{subcontrol-origin: margin; subcontrol-position: top left; left: 10px; padding: 0 3px;}");
	
	QWidget *genericPage = new QWidget();
	QVBoxLayout *genericLayout = new QVBoxLayout;
	genericLayout->addWidget(eepromDialog);
	
	// Remove extra button since EEPROM widget is now part of the main window
	eepromControlsLayout->removeWidget(eepromReadButton);
	delete eepromReadButton;

	eepromAction->setEnabled(false);
	
	genericPage->setLayout(genericLayout);
	tabWidget->insertTab(0, genericPage, "EEPROM");
}


/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-iiab.h
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

#ifndef NODE_IIAB_H
#define NODE_IIAB_H

#include <window.h>
#include <stdint.h>

#define EE_TIMEOUT_SECONDS      0x10
// 0x10 - 0x13 = 4 sets of timeouts, one for each direction
#define EE_LOCKOUT_SECONDS      0x14
#define EE_TIMELOCK_SECONDS     0x15
#define EE_DEBOUNCE_SECONDS     0x16
#define EE_CLOCK_SOURCE_ADDRESS 0x18
#define EE_MAX_DEAD_RECKONING   0x19
#define EE_SIM_TRAIN_WINDOW     0x1A
#define EE_BLINKY_DECISECS      0x1F
#define EE_INPUT_POLARITY0      0x20
#define EE_INPUT_POLARITY1      0x21
#define EE_OUTPUT_POLARITY0     0x22
#define EE_OUTPUT_POLARITY1     0x23
#define EE_OUTPUT_POLARITY2     0x24
#define EE_OUTPUT_POLARITY3     0x25
#define EE_OUTPUT_POLARITY4     0x26
#define EE_MISC_CONFIG          0x30
#define EE_SIM_TRAINS           0x40

QT_BEGIN_NAMESPACE
class QDateTimeEdit;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;
class QCheckBox;
class QPushButton;
class QRadioButton;
class QComboBox;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class Node_IIAB : public Window
{
    Q_OBJECT

	public slots:
		void write(void);
		void read(void);
	
	public:
		Node_IIAB();
	
	private:
		HexSpinBox *clockSource;
		QDoubleSpinBox *blinky, *maxDeadReckoning;
		QSpinBox *timeout0, *timeout1, *timeout2, *timeout3, *lockout, *timelock, *debounce, *simTrainWindow;
		QRadioButton *ledPolarityAnode, *ledPolarityCathode;
		QComboBox *turnoutPolarity0, *turnoutPolarity2, *detectorPolarity;
		
		uint8_t eeprom[1024];
};

#endif


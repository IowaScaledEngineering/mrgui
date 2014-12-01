/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     window.h
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

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMainWindow>
#include "hexspinbox.h"

QT_BEGIN_NAMESPACE
class QDateTimeEdit;
class QSpinBox;
class QDoubleSpinBox;
class QGroupBox;
class QCheckBox;
class QRadioButton;
class QComboBox;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public slots:
    void write(void);
    void read(void);
	
public:
    Window();

private:
	HexSpinBox *nodeAddr, *clockSource;
	QDoubleSpinBox *transmitInterval, *blinky, *maxDeadReckoning;
	QSpinBox *timeout0, *timeout1, *timeout2, *timeout3, *lockout, *timelock, *debounce, *simTrainWindow;
	QRadioButton *ledPolarityAnode, *ledPolarityCathode;
	QComboBox *turnoutPolarity0, *turnoutPolarity2, *detectorPolarity;
	
	int readByte(int addr);
	int readWord(int addr);
};

#endif

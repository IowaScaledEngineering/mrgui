/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-irsense.h
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

#ifndef NODE_IRSENSE_H
#define NODE_IRSENSE_H

#include <QtWidgets>

#include "window.h"
#include "hexspinbox.h"

#define EE_PROX_THR_HIGH  0x00
#define EE_PROX_THR_LOW   0x01


class Node_IRSENSE : public Window
{
    Q_OBJECT

	public:
		Node_IRSENSE(void);

	private:
		QSpinBox *proximityThreshold;

	public slots:
	
	private slots:
		void setDefaults(void);
		void proximityThresholdUpdated(void);
		void proximityThresholdSet(void);
};

#endif


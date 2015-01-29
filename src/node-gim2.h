/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-gim2.h
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

#ifndef NODE_GIM2_H
#define NODE_GIM2_H

#include <QtWidgets>

#include "window.h"
#include "hexspinbox.h"

#define NUM_OUTPUTS   64

#define EE_OCC_ADDR       0x10
#define EE_OCC_PKT        0x50
#define EE_OCC_BITBYTE    0x90


class PktTypeSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    PktTypeSpinBox(QWidget *parent = 0);
	typedef enum
	{
		INT,
		HEX,
		CHAR,
	} SpinBoxMode;
	SpinBoxMode spinBoxMode;

protected:
    QValidator::State validate(QString &text, int &pos) const;
    int valueFromText(const QString &text) const;
    QString textFromValue(int value) const;

private:
    QRegExpValidator *hexValidator;
    QRegExpValidator *charValidator;
    QRegExpValidator *intValidator;
};

class Node_GIM2 : public Window
{
    Q_OBJECT

	public:
		Node_GIM2(void);

	private:
		HexSpinBox *sourceAddress[NUM_OUTPUTS];
		PktTypeSpinBox *packetType[NUM_OUTPUTS];
		QSpinBox *listenByte[NUM_OUTPUTS];
		QSpinBox *listenBit[NUM_OUTPUTS];
		QComboBox *pktTypeMode;

	public slots:
	
	private slots:
		void setDefaults(void);
		void pktTypeModeChanged(void);
		void sourceAddressUpdated(void);
		void sourceAddressSet(void);
		void packetTypeUpdated(void);
		void packetTypeSet(void);
		void listenByteUpdated(void);
		void listenByteSet(void);
		void listenBitUpdated(void);
		void listenBitSet(void);
};

#endif


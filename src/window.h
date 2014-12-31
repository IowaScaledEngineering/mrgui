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

#include <QtWidgets>

#include "avrinfo.h"
#include "hexspinbox.h"

#define MRBUS_EE_DEVICE_ADDR         0
#define MRBUS_EE_DEVICE_OPT_FLAGS    1
#define MRBUS_EE_DEVICE_UPDATE_H     2
#define MRBUS_EE_DEVICE_UPDATE_L     3

class Window : public QMainWindow
{
    Q_OBJECT

	public:
		Window(const char*);
		QLabel *eepromDataBinary;
		QPushButton *writeButton, *readButton, *eepromReadButton, *consoleCloseButton;
		QTabWidget *tabWidget;
		HexSpinBox *nodeAddr, *eepromAddr, *eepromData;
		QDoubleSpinBox *transmitInterval;
		QTextEdit *eepromTable, *consoleText;
		QDialog *eepromDialog, *consoleDialog;
		QActionGroup *programmerGroup;
		QAction *programmerAction[sizeof(proginfo)/sizeof(proginfo[0])];
		QTemporaryFile tempFile;
		
		const char *avrDevice;
		uint8_t *eeprom;
		const AVRInfo* getAVRInfo(const char*);
		
		char avrdudePath[1024];
		char avrdudeConfPath[1024];

	private:
		QProcess *avrdudeProcess;
		QString firmwarePath;
		typedef enum
		{
			READ_EEPROM,
			WRITE_EEPROM,
			UPDATE_FIRMWARE,
		} AvrdudeAction;
		AvrdudeAction avrdudeAction;

	signals:
		void eepromUpdated();

	public slots:
		void updateEepromTable(void);
		
	private slots:
		void readStdout(void);
		void readStderr(void);
		void avrdudeDone(void);
		void getAvrdudePath(void);
		void getAvrdudeConfPath(void);
		void cleanupConsole(void);
		uint8_t findProgrammerIndex(void);
		void write(void);
		void read(void);
		void updateFirmware(void);
		void updateByte(void);
		void nodeAddrUpdated(void);
		void nodeAddrSet(void);
		void transmitIntervalUpdated(void);
		void transmitIntervalSet(void);
};

class NodeDialog : public QDialog
{
    Q_OBJECT
	
	public:
		explicit NodeDialog();
		QListWidget *nodeList;
		
	private:
};

#endif


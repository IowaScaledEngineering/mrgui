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
		QActionGroup *programmerGroup, *deviceGroup;
		QAction *eepromAction;
		QAction *programmerAction[sizeof(proginfo)/sizeof(proginfo[0])], *deviceAction[sizeof(avrinfo)/sizeof(avrinfo[0])], *forceAction;
		QTemporaryFile tempFile;
		QHBoxLayout *eepromControlsLayout;
		
		const char *avrDevice;
		uint8_t *eeprom;
		const AVRInfo* getAVRInfo(const char*);
		
		char avrdudePath[1024];
		char avrdudeConfPath[1024];
		QString zadicPath;

	private:
		QAction *aboutAction;
		QProcess *cmdLineProcess;
		QString firmwarePath;
		typedef enum
		{
			READ_EEPROM,
			WRITE_EEPROM,
			UPDATE_FIRMWARE,
		} CmdLineActivity;
		CmdLineActivity cmdLineActivity;
		uint8_t findProgrammerIndex(void);
		QString avrdudeCommandLine(void);
		void drawEepromTable(void);

	signals:
		void eepromUpdated();
		void resetDefaults();

	public slots:
		void updateEepromTable(void);
		void nodeAddrUpdated(void);
		void transmitIntervalUpdated(void);
		
	private slots:
		void reset(void);
		void about(void);
		void setDefaults(void);
		void load(void);
		void save(void);
		void readStdout(void);
		void readStderr(void);
		void cmdLineDone(int);
		void getAvrdudePath(void);
		void getAvrdudeConfPath(void);
		void cleanupConsole(void);
		void write(void);
		void read(void);
		void updateFirmware(void);
#ifdef Q_OS_WIN
		void install(void);
#endif
		void updateByte(void);
		void nodeAddrSet(void);
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


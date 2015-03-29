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

#include <stdint.h>
#include <unistd.h>

#include <QtWidgets>
#include <QDebug>

#include "window.h"
#include "nodes.h"
#include "avrinfo.h"
#include "intelhexmem.h"

Window::Window(const char *device)
{
	QString workingPath = QCoreApplication::applicationDirPath();
	strncpy(avrdudePath, workingPath.toLocal8Bit().data(), sizeof(avrdudePath));
	strncpy(avrdudeConfPath, workingPath.toLocal8Bit().data(), sizeof(avrdudePath));
#if defined(__linux__)
	strncat(avrdudePath, "/avrdude/x86_64-pc-linux-gnu/avrdude-6.0.1/bin/avrdude", sizeof(avrdudePath));
	strncat(avrdudeConfPath, "/avrdude/x86_64-pc-linux-gnu/avrdude-6.0.1/etc/avrdude.conf", sizeof(avrdudeConfPath));
#elif defined(__APPLE__)
	strncat(avrdudePath, "/avrdude-6.0.1/bin/avrdude", sizeof(avrdudePath));
	strncat(avrdudeConfPath, "/avrdude-6.0.1/etc/avrdude.conf", sizeof(avrdudeConfPath));
#elif defined(_WIN32)
	strncat(avrdudePath, "./avrdude-6.0.1/bin/avrdude", sizeof(avrdudePath));
	strncat(avrdudeConfPath, "./avrdude-6.0.1/etc/avrdude.conf", sizeof(avrdudeConfPath));
#endif

	avrDevice = device;
	eeprom = (uint8_t*)malloc(getAVRInfo(avrDevice)->eeprom_size);
	// Preset EEPROM
	for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
		eeprom[i] = 0xFF;
	
	// eepromTable must be declared before setting any defaults so updateEepromTable() can update it
	eepromTable = new QTextEdit();
	eepromTable->setReadOnly(true);
	eepromTable->setLineWrapMode(QTextEdit::NoWrap);
	QFont eepromTableFont;
	eepromTableFont.setFamily("Courier");
	eepromTableFont.setStyleHint(QFont::Monospace);
	eepromTableFont.setFixedPitch(true);
	eepromTableFont.setPointSize(10);
	eepromTable->setFont(eepromTableFont);

	QWidget *eepromControls = new QWidget();
	eepromControlsLayout = new QHBoxLayout;
	QLabel *eepromAddrLabel = new QLabel(tr("Address:"));
	eepromAddr = new HexSpinBox;
	eepromAddr->setRange(0,(getAVRInfo(avrDevice)->eeprom_size)-1);
	eepromAddr->setPrefix("0x");
	eepromControlsLayout->addWidget(eepromAddrLabel, 0);
	eepromControlsLayout->addWidget(eepromAddr, 0);
	eepromControlsLayout->addSpacing(10);
	QLabel *eepromDataLabel = new QLabel(tr("Data:"));
	eepromData = new HexSpinBox;
	eepromData->setRange(0,255);
	eepromData->setPrefix("0x");
	eepromDataBinary = new QLabel(tr(""));
	eepromDataBinary->setFont(eepromTableFont);
	eepromReadButton = new QPushButton(tr("&Read EEPROM"));
	eepromReadButton->setFocusPolicy(Qt::NoFocus);
	eepromControlsLayout->addWidget(eepromDataLabel, 0);
	eepromControlsLayout->addWidget(eepromData, 0);
	eepromControlsLayout->addSpacing(10);
	eepromControlsLayout->addWidget(eepromDataBinary, 0);
	eepromControlsLayout->addStretch(1);  // Add sacrificial stretch space to middle
	eepromControlsLayout->addWidget(eepromReadButton);
	eepromControls->setLayout(eepromControlsLayout);
	connect(eepromAddr, SIGNAL(valueChanged(int)), this, SLOT(updateEepromTable()));
	connect(eepromData, SIGNAL(valueChanged(int)), this, SLOT(updateByte()));
	connect(eepromReadButton, SIGNAL(clicked()), this, SLOT(read()));

	// From https://bugreports.qt-project.org/browse/QTBUG-15809
	updateEepromTable();  // Force update to get some data in the table
	eepromTable->document()->adjustSize();
	eepromTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	int eepromDialogSize;
	eepromDialogSize = eepromTable->minimumSizeHint().width(); // should get correct value, but really - not.
	eepromDialogSize -= eepromTable->verticalScrollBar()->sizeHint().width(); //subtracting incorrect value.
	eepromDialogSize += eepromTable->document()->size().width();
	eepromTable->setMinimumWidth(eepromDialogSize);



	// Create buttons
	writeButton = new QPushButton(tr("&Write EEPROM"));
	writeButton->setFocusPolicy(Qt::NoFocus);
	readButton = new QPushButton(tr("&Read EEPROM"));
	readButton->setFocusPolicy(Qt::NoFocus);
	connect(readButton, SIGNAL(clicked()), this, SLOT(read()));
	connect(writeButton, SIGNAL(clicked()), this, SLOT(write()));

	QWidget *nodeWidgets = new QWidget();
	QHBoxLayout *nodeLayout = new QHBoxLayout;
	QLabel *nodeAddrLabel = new QLabel(tr("Node Address:"));
	nodeAddr = new HexSpinBox;
	nodeAddr->setRange(0,255);
	nodeAddr->setPrefix("0x");
	nodeLayout->addWidget(nodeAddrLabel, 0);
	nodeLayout->addWidget(nodeAddr, 0);
	nodeLayout->addSpacing(10);
	QLabel *transmitIntervalLabel = new QLabel(tr("Transmit Interval:"));
	transmitInterval = new QDoubleSpinBox;
	transmitInterval->setRange(0.0, 6553.5);
	transmitInterval->setSingleStep(1.0);
	transmitInterval->setDecimals(1);
	transmitInterval->setSuffix("s");
	nodeLayout->addWidget(transmitIntervalLabel, 0);
	nodeLayout->addWidget(transmitInterval, 0);
	nodeLayout->addStretch(1);  // Add sacrificial stretch space to middle
	nodeLayout->addWidget(readButton);
	nodeLayout->addWidget(writeButton);
	nodeWidgets->setLayout(nodeLayout);

	setDefaults();

	// Connect signals
	connect(nodeAddr, SIGNAL(valueChanged(int)), this, SLOT(nodeAddrUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(nodeAddrSet()));
	connect(transmitInterval, SIGNAL(valueChanged(double)), this, SLOT(transmitIntervalUpdated()));
	connect(this, SIGNAL(eepromUpdated()), this, SLOT(transmitIntervalSet()));
	


	tabWidget = new QTabWidget();



	eepromDialog = new QDialog(this);
	QVBoxLayout *eepromLayout = new QVBoxLayout;
	eepromLayout->addWidget(eepromTable);
	eepromLayout->addWidget(eepromControls);
	eepromDialog->setLayout(eepromLayout);
	eepromDialog->setWindowTitle("MRGui EEPROM Viewer");
	
	
	
	consoleCloseButton = new QPushButton(tr("Close"));
	consoleText = new QTextEdit();
	consoleText->setReadOnly(true);
	consoleText->setLineWrapMode(QTextEdit::NoWrap);
	consoleText->setMinimumWidth(500);
	consoleText->setMinimumHeight(400);
	consoleText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	QPalette p = consoleText->palette();
	p.setColor(QPalette::Base, QColor(0, 0, 0));
	p.setColor(QPalette::Text, QColor(0, 255, 0));
	consoleText->setPalette(p);
	QFont consoleFont;
	consoleFont.setFamily("Courier");
	consoleFont.setStyleHint(QFont::Monospace);
	consoleFont.setFixedPitch(true);
	consoleFont.setPointSize(8);
	consoleText->setFont(consoleFont);

	consoleDialog = new QDialog();
	QVBoxLayout *consoleLayout = new QVBoxLayout;
	consoleLayout->addWidget(consoleText);
	consoleLayout->addWidget(consoleCloseButton);
	consoleLayout->setAlignment(consoleCloseButton, Qt::AlignCenter);
	consoleDialog->setLayout(consoleLayout);
	consoleDialog->setWindowTitle("Console");
	consoleDialog->setModal(true);
	connect(consoleCloseButton, SIGNAL(clicked()), consoleDialog, SLOT(accept()));
	connect(consoleDialog, SIGNAL(finished(int)), this, SLOT(cleanupConsole()));
	
	
	
	avrdudeProcess = new QProcess(this);
	connect(avrdudeProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
	connect(avrdudeProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));
	connect(avrdudeProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(avrdudeDone(int)));



	// Connect to reset action
	connect(this, SIGNAL(resetDefaults()), this, SLOT(setDefaults()));



	QAction *loadAction = new QAction(tr("&Load Configuration..."), this);
	connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));
	QAction *saveAction = new QAction(tr("&Save Configuration..."), this);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	QAction *exitAction = new QAction(tr("E&xit"), this);
	connect(exitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
	QAction *readAction = new QAction(tr("&Read EEPROM"), this);
	connect(readAction, SIGNAL(triggered()), this, SLOT(read()));
	QAction *writeAction = new QAction(tr("&Write EEPROM"), this);
	connect(writeAction, SIGNAL(triggered()), this, SLOT(write()));
	QAction *updateAction = new QAction(tr("&Update Firmware..."), this);
	connect(updateAction, SIGNAL(triggered()), this, SLOT(updateFirmware()));
	QAction *resetAction = new QAction(tr("Reset Configuration to &Defaults..."), this);
	connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));
	// Declared in .h file so nodes can deactive this menu item
	eepromAction = new QAction(tr("&EEPROM Editor..."), this);
	connect(eepromAction, SIGNAL(triggered()), eepromDialog, SLOT(show()));
	QAction *avrdudeAction = new QAction(tr("Select &avrdude Path..."), this);
	connect(avrdudeAction, SIGNAL(triggered()), this, SLOT(getAvrdudePath()));
	QAction *avrdudeConfAction = new QAction(tr("Select avrdude &Config..."), this);
	connect(avrdudeConfAction, SIGNAL(triggered()), this, SLOT(getAvrdudeConfPath()));

	forceAction = new QAction(tr("&Override Signature Check"), this);
	forceAction->setCheckable(true);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(loadAction);
	fileMenu->addAction(saveAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	QMenu *programMenu = menuBar()->addMenu(tr("&Program"));
	programMenu->addAction(readAction);
	programMenu->addAction(writeAction);
	programMenu->addAction(updateAction);
	programMenu->addSeparator();
	programMenu->addAction(resetAction);
	programMenu->addSeparator();

	QMenu *programmerMenu = new QMenu(tr("&Programmer"));
	programmerGroup = new QActionGroup(this);
	for(uint8_t i=0; i<(sizeof(proginfo)/sizeof(proginfo[0])); i++)
	{
		programmerAction[i] = new QAction(tr(proginfo[i].nice_name), this);
		programmerAction[i]->setCheckable(true);
		programmerGroup->addAction(programmerAction[i]);
		programmerMenu->addAction(programmerAction[i]);
	}
	programmerAction[0]->setChecked(true);
	programMenu->addMenu(programmerMenu);

	QMenu *advancedMenu = menuBar()->addMenu(tr("&Advanced"));
	advancedMenu->addAction(eepromAction);
	advancedMenu->addSeparator();
	advancedMenu->addAction(avrdudeAction);
	advancedMenu->addAction(avrdudeConfAction);
	advancedMenu->addSeparator();
	advancedMenu->addAction(forceAction);

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	QAction *aboutAction = new QAction(tr("&About"), this);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	helpMenu->addAction(aboutAction);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(tabWidget);
	layout->addWidget(nodeWidgets);



    QWidget *widget = new QWidget;
    setCentralWidget(widget);
	widget->setLayout(layout);

	setWindowTitle(tr("MRGui Programmer"));
}

void Window::reset(void)
{
	QMessageBox msgBox;
	msgBox.setText("All changes will be lost!");
	msgBox.setInformativeText("Are you sure you want to reset all values to their defaults?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int ret = msgBox.exec();
	
	switch (ret)
	{
		case QMessageBox::Yes:
			emit resetDefaults();
			break;
		default:
			break;
	}
}

void Window::about(void)
{
	QMessageBox msgBox;
	msgBox.setText("MRGui");
	msgBox.setInformativeText(QString("Iowa Scaled Engineering\nVersion %1").arg(MRGUI_VERSION));
	msgBox.setStandardButtons(QMessageBox::Close);
	msgBox.setDefaultButton(QMessageBox::Close);
	msgBox.exec();
}

void Window::setDefaults(void)
{
	// Set defaults
	nodeAddr->setValue(0x03);
	nodeAddrUpdated();  // Force update for initial value, even if value not changed
	transmitInterval->setValue(5.0);
	transmitIntervalUpdated();  // Force update for initial value, even if value not changed
}

void Window::load(void)
{
	QString path = QFileDialog::getOpenFileName(this, tr("Load Configuration File"), NULL, "EEP Files (*.eep);;All Files (*.*)");
	if(!path.isNull())
	{
		FILE *fptr;
		if( (fptr = fopen(path.toLocal8Bit().data(), "r")) != NULL)
		{
			IntelHexMemory eepromMem(getAVRInfo(avrDevice)->eeprom_size);
			eepromMem.read_ihex(fptr);
			fclose(fptr);

			for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
			{
				eeprom[i] = eepromMem.read_uint8(i);
			}
			updateEepromTable();
			emit eepromUpdated();
		}
	}
}

void Window::save(void)
{
//	QFileDialog dialog(this, tr("Save Configuration File"), NULL, "EEP Files (*.eep);;All Files (*.*)");
//	dialog.setAcceptMode(QFileDialog::AcceptSave);
//	dialog.setDefaultSuffix("eep");
//	dialog.exec();
//	QStringList path = dialog.selectedFiles();

	QString path = QFileDialog::getSaveFileName(this, tr("Save Configuration File"), NULL, "EEP Files (*.eep);;All Files (*.*)", 0, QFileDialog::DontConfirmOverwrite);
	if(!path.isNull())
	{
		if(!path.endsWith(".eep"))
		{
			path.append(".eep");
		}
		
		FILE *fptr;
		int ret = QMessageBox::Yes;

		if( (fptr = fopen(path.toLocal8Bit().data(), "r")) != NULL)
		{
			// File already exists
			fclose(fptr);
			QMessageBox msgBox;
			msgBox.setText(QString("A configuration file named \"%1\" already exists.  Do you want to replace it?").arg(path));
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			msgBox.setDefaultButton(QMessageBox::No);
			ret = msgBox.exec();
		}

		if(QMessageBox::Yes == ret)
		{
			if( (fptr = fopen(path.toLocal8Bit().data(), "w")) != NULL)
			{
				IntelHexMemory eepromMem(getAVRInfo(avrDevice)->eeprom_size);

				for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
				{
					eepromMem.write_uint8(i, eeprom[i]);
				}

				eepromMem.write_ihex(fptr);
				fclose(fptr);
			}
			else
			{
				QMessageBox msgBox;
				msgBox.setText(QString("Error!  File could not be written."));
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setDefaultButton(QMessageBox::Ok);
				ret = msgBox.exec();
			}
		}
	}
}

const AVRInfo* Window::getAVRInfo(const char* part_name)
{
	for(uint32_t i=0; i<sizeof(avrinfo)/sizeof(AVRInfo); i++)
	{
		if (0 == strcmp(part_name, avrinfo[i].part_name))
			return(&avrinfo[i]);
	}
	return(NULL);
}

void Window::readStdout(void)
{
	consoleText->insertPlainText(avrdudeProcess->readAllStandardOutput());
	consoleText->verticalScrollBar()->setValue(consoleText->verticalScrollBar()->maximum());
}

void Window::readStderr(void)
{
	consoleText->insertPlainText(avrdudeProcess->readAllStandardError());
	consoleText->verticalScrollBar()->setValue(consoleText->verticalScrollBar()->maximum());
}

void Window::avrdudeDone(int exitCode)
{
	IntelHexMemory eepromMem(getAVRInfo(avrDevice)->eeprom_size);
	FILE *fptr;

	consoleCloseButton->setEnabled(true);
	
	switch(avrdudeAction)
	{
		case READ_EEPROM:
			if(!exitCode)
			{
				fptr = fdopen(tempFile.handle(), "r");
				eepromMem.read_ihex(fptr);
				tempFile.close();

				for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
				{
					eeprom[i] = eepromMem.read_uint8(i);
				}
				updateEepromTable();
				emit eepromUpdated();
			}
			break;

		case WRITE_EEPROM:
			break;

		case UPDATE_FIRMWARE:
			break;
	}
}

void Window::getAvrdudePath(void)
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select avrdude Executable"));
	if(!path.isNull())
		strncpy(avrdudePath, path.toLocal8Bit().data(), sizeof(avrdudePath));
}

void Window::getAvrdudeConfPath(void)
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select avrdude Configuration File"), NULL, "CONF Files (*.conf);;All Files (*.*)");
	if(!path.isNull())
	{
		strncpy(avrdudeConfPath, path.toLocal8Bit().data(), sizeof(avrdudeConfPath));	
	}
}

void Window::cleanupConsole(void)
{
	if(QProcess::NotRunning != avrdudeProcess->state())
	{
		avrdudeProcess->close();
	}
}

uint8_t Window::findProgrammerIndex(void)
{
	uint8_t programmerIndex;
	for(programmerIndex=0; programmerIndex<(sizeof(proginfo)/sizeof(proginfo[0])); programmerIndex++)
	{
		if(programmerGroup->checkedAction() == programmerAction[programmerIndex])
			break;
	}
	return(programmerIndex);
}

QString Window::avrdudeCommandLine(void)
{
	uint8_t progIdx = findProgrammerIndex();
	QString cmdline = QString("%1 -C %2 -c %3 %4 -p %5 -B1 ").arg(avrdudePath, avrdudeConfPath, proginfo[progIdx].avrdude_name, proginfo[progIdx].avrdude_additional_args, getAVRInfo(avrDevice)->part_name);
	if(forceAction->isChecked())
	{
		cmdline.append("-F ");
	}
	return cmdline;
}

void Window::write(void)
{
	IntelHexMemory eepromMem(getAVRInfo(avrDevice)->eeprom_size);

	for(uint32_t i=0; i<getAVRInfo(avrDevice)->eeprom_size; i++)
	{
		eepromMem.write_uint8(i, eeprom[i]);
	}

	if(tempFile.open())
	{
		FILE *fptr = fdopen(tempFile.handle(), "w");
		eepromMem.write_ihex(fptr);
		fflush(fptr);
		tempFile.close();
	}

	consoleCloseButton->setEnabled(false);
	consoleText->clear();
	consoleDialog->show();
	QString cmdline = avrdudeCommandLine().append(QString("-U eeprom:w:%1:i").arg(tempFile.fileName()));
	consoleText->append(cmdline.append("\n\n- - - Writing EEPROM - - -\n\n"));
	avrdudeAction = WRITE_EEPROM;
	avrdudeProcess->start(cmdline);
}

void Window::read(void)
{
	consoleCloseButton->setEnabled(false);
	consoleText->clear();
	consoleDialog->show();

	tempFile.open();
	QString cmdline = avrdudeCommandLine().append(QString("-U eeprom:r:%1:i").arg(tempFile.fileName()));
	consoleText->append(cmdline.append("\n\n- - - Reading EEPROM - - -\n\n"));
	avrdudeAction = READ_EEPROM;
	avrdudeProcess->start(cmdline);
}

void Window::updateFirmware(void)
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select Firmware File"), firmwarePath, "HEX Files (*.hex);;All Files (*.*)");
	if(!path.isNull())
	{
		firmwarePath = QFileInfo(path).path();
	
		consoleCloseButton->setEnabled(false);
		consoleText->clear();
		consoleDialog->show();
		QString cmdline = avrdudeCommandLine().append(QString("-U flash:w:%1:i").arg(path));
		consoleText->append(cmdline.append("\n\n- - - Updating Firmware - - -\n\n"));
		avrdudeAction = UPDATE_FIRMWARE;
		avrdudeProcess->start(cmdline);
	}
}

void Window::updateByte(void)
{
	eeprom[eepromAddr->value()] = eepromData->value();
	drawEepromTable();  // Don't do a full update since that will be a circular update to the eepromData widget
	emit eepromUpdated();
}

void Window::updateEepromTable(void)
{
	drawEepromTable();
	eepromData->setValue(eeprom[eepromAddr->value()]);
}

void Window::drawEepromTable(void)
{
	uint32_t rows = (getAVRInfo(avrDevice)->eeprom_size) / 16;
	int scrollPosition = eepromTable->verticalScrollBar()->value();
	QString eepromContents = QString();
	for(uint32_t r = 0; r < rows; r++)
	{
		QString line = QString("%1:  ").arg(r*16, 4, 16, QChar('0'));
		for(uint8_t c = 0; c < 16; c++)
		{
			if(r*16+c == (uint32_t)eepromAddr->value())
				line.append(QString("<font style=\"color: black; background-color: #ccccff\">%1</font> ").arg(eeprom[r*16 + c], 2, 16, QChar('0')));
			else
				line.append(QString("%1 ").arg(eeprom[r*16 + c], 2, 16, QChar('0')));
			if(7 == c)
				line.append(" ");
		}
		eepromContents.append(line).append("<br>\n");
	}
	eepromTable->setText(eepromContents);
	eepromTable->verticalScrollBar()->setValue(scrollPosition);

	eepromDataBinary->setText(QString("%1 %2").arg((eepromData->value() >> 4) & 0xF, 4, 2, QChar('0')).arg(eepromData->value() & 0xF, 4, 2, QChar('0')));
}

void Window::nodeAddrUpdated(void)
{
	eeprom[MRBUS_EE_DEVICE_ADDR] = nodeAddr->value();
	updateEepromTable();
}

void Window::nodeAddrSet(void)
{
	nodeAddr->blockSignals(true);
	nodeAddr->setValue(eeprom[MRBUS_EE_DEVICE_ADDR]);
	nodeAddr->blockSignals(false);
}

void Window::transmitIntervalUpdated(void)
{
	eeprom[MRBUS_EE_DEVICE_UPDATE_H] = (int)((transmitInterval->value() * 10) / 256) & 0xFF;
	eeprom[MRBUS_EE_DEVICE_UPDATE_L] = (int)(transmitInterval->value() * 10) & 0xFF;
	updateEepromTable();
}

void Window::transmitIntervalSet(void)
{
	transmitInterval->blockSignals(true);
	transmitInterval->setValue((eeprom[MRBUS_EE_DEVICE_UPDATE_H]*256 + eeprom[MRBUS_EE_DEVICE_UPDATE_L]) / 10.0);
	transmitInterval->blockSignals(false);
}

NodeDialog::NodeDialog()
{
	QPushButton *okButton = new QPushButton(tr("&OK"));
	QPushButton *cancelButton = new QPushButton(tr("&Cancel"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	nodeList = new QListWidget();
	for(unsigned int i=0; i<sizeof(nodeInfo)/sizeof(nodeInfo[0]); i++)
	{
		nodeList->addItem(nodeInfo[i].node_name);
	}

	connect(nodeList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(accept()));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(nodeList, 0, 0, 1, 2);
	layout->addWidget(okButton, 1, 0);
	layout->addWidget(cancelButton, 1, 1);
	
	setLayout(layout);
	setWindowTitle(tr("Select Node"));
}


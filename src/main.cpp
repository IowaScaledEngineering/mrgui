/*************************************************************************
Title:    MRGui           
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     main.cpp
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

#include <QApplication>

#include "window.h"
#include "node-iiab.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
//	NodeDialog nodeDialog;
//	if (QDialog::Accepted == nodeDialog.exec())
//	{
//		QString selectedNode = nodeDialog.nodeList->currentItem()->text();
//		if(!selectedNode.compare("MRB-IIAB"))
//		{
			Node_IIAB window("atmega328p", 1024);
		    window.show();
		    return app.exec();
//		}
//		else if(!selectedNode.compare("Generic (ATMega328)"))
//		{
//			Window window("atmega328p", 1024);
//		    window.show();
//		    return app.exec();
//		}
//		else
//		{
//			app.quit();
//		}
//	}
//	else
//	{
//		app.quit();
//	}
}


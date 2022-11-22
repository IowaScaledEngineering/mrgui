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
#include "nodes.h"
#include "node-generic.h"
#include "node-iiab.h"
#include "node-gim2.h"
#include "node-irsense.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Window *window = NULL;
	
	NodeDialog nodeDialog;
	if (QDialog::Accepted == nodeDialog.exec())
	{
		Nodes node = NODE_GENERIC_MEGA_328P;
		
		for(unsigned int i=0; i<sizeof(nodeInfo)/sizeof(nodeInfo[0]); i++)
		{
			if( !nodeDialog.nodeList->currentItem()->text().compare(nodeInfo[i].node_name) )
			{
				node = nodeInfo[i].node;
			}
		}

		switch(node)
		{
			case NODE_GIM2:
				window = new Node_GIM2;
				break;
			case NODE_IIAB:
				window = new Node_IIAB;
				break;
			case NODE_IRSENSE:
				window = new Node_IRSENSE;
				break;
			case NODE_GENERIC_MEGA_48P:
				window = new Node_Generic("atmega48p");
				break;
			case NODE_GENERIC_MEGA_328:
				window = new Node_Generic("atmega328");
				break;
			case NODE_GENERIC_MEGA_328P:
				window = new Node_Generic("atmega328p");
				break;
			case NODE_GENERIC_MEGA_1284P:
				window = new Node_Generic("atmega1284p");
				break;
		}

		window->show();
		return app.exec();
	}
	else
	{
		app.quit();
		return 0;
	}
}


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
			case NODE_ACSW:
				// FIXME
				app.quit();
				return(0);
				break;
			case NODE_GIM2:
				// FIXME
				app.quit();
				return(0);
				break;
			case NODE_IIAB:
				window = new Node_IIAB;
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

//		QString selectedNode = nodeDialog.nodeList->currentItem()->text();
//		if(!selectedNode.compare("MRB-ACSW"))
//		{
//			Node_ACSW window;
//			window.show();
//			return app.exec();
//		}
//		else if(!selectedNode.compare("MRB-GIM2"))
//		{
//			Node_GIM2 window;
//			window.show();
//			return app.exec();
//		}
//		/*else*/ if(!selectedNode.compare("MRB-IIAB"))
//		{
//			Node_IIAB window;
//			window.show();
//			return app.exec();
//		}
//		else if(!selectedNode.compare("Generic (ATMega328)"))
//		{
//			Window window("atmega328");
//		    window.show();
//		    return app.exec();
//		}
//		else if(!selectedNode.compare("Generic (ATMega328P)"))
//		{
//			Window window("atmega328p");
//		    window.show();
//		    return app.exec();
//		}
//		else if(!selectedNode.compare("Generic (ATMega1284P)"))
//		{
//			Window window("atmega1284p");
//		    window.show();
//		    return app.exec();
//		}
//		else
//		{
//			app.quit();
//		}
	}
	else
	{
		app.quit();
	}
}


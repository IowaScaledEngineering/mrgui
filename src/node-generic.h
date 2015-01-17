/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     node-generic.h
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

#ifndef NODE_GENERIC_H
#define NODE_GENERIC_H

#include <QtWidgets>

#include "window.h"

class Node_Generic : public Window
{
    Q_OBJECT

	public:
		Node_Generic(const char*);

	private:

	public slots:
	
	private slots:
};

#endif


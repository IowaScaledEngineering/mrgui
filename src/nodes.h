/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     nodes.h
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

#ifndef NODES_H
#define NODES_H

typedef enum
{
	NODE_GIM2,
	NODE_IIAB,
	NODE_GENERIC_MEGA_48P,
	NODE_GENERIC_MEGA_328,
	NODE_GENERIC_MEGA_328P,
	NODE_GENERIC_MEGA_1284P,
} Nodes;

typedef struct
{
	Nodes node;
	const char* node_name;
} NodeInfo;

const NodeInfo nodeInfo[] = 
{
	{NODE_GIM2, "MRB-GIM2"},
	{NODE_IIAB, "MRB-IIAB"},
	{NODE_GENERIC_MEGA_48P, "Generic ATmega48P"},
	{NODE_GENERIC_MEGA_328, "Generic ATmega328"},
	{NODE_GENERIC_MEGA_328P, "Generic ATmega328P"},
	{NODE_GENERIC_MEGA_1284P, "Generic ATmega1284P"},
};

#endif


/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     avrinfo.h
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

#ifndef AVRINFO_H
#define AVRINFO_H

#include <stdint.h>

typedef struct
{
	const char* part_name;
	const uint32_t eeprom_size;
} AVRInfo;

const AVRInfo avrinfo[] = 
{
	{"atmega328", 1024},
	{"atmega328p", 1024},
	{"atmega48p", 256},
	{"atmega1284p", 4096},
};

typedef struct
{
	const char* nice_name;
	const char* avrdude_name;
} AVRProgrammerInfo;

const AVRProgrammerInfo proginfo[] = 
{
	{"USBtinyISP", "usbtiny"},
	{"Ferrets", "ferrets"},
};


#endif


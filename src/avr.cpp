/*************************************************************************
Title:    MRGui
Authors:  Michael Petersen <railfan@drgw.net>
          Nathan D. Holmes <maverick@drgw.net>
File:     avr.cpp
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

#include <stdio.h>
#include <stdint.h>

#include "avr.h"

void avrWriteEEPROM(int addr, uint8_t *data, int numBytes)
{
	FILE *fp;
	char buffer[256];
	int i;

	for(i=0; i<numBytes; i++)
	{
		sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -W %d,%d", addr++, *(data+i));
		printf("%s\n", buffer);
		fp = popen(buffer, "r");
		fclose(fp);
	}
}

void avrReadEEPROM(int addr, uint8_t *data, int numBytes)
{
	FILE *fp;
	char buffer[256];
	int i;
	unsigned int value;

	for(i=0; i<numBytes; i++)
	{
		sprintf(buffer, "$(/bin/pwd)/avrdude/avrdude -c usbtiny -p atmega328p -R %d,1 | grep -i ^%04X", addr, addr);
		printf("%s\n", buffer);
		fp = popen(buffer, "r");
		if (fp != NULL)
		{
			while (fgets(buffer, sizeof(buffer), fp) != NULL)
			{
				printf("%s\n", buffer);
				sscanf(buffer, "%*x %x", &value);
				*(data+i) = value;
			}
		}
		fclose(fp);
		addr++;
	}
}


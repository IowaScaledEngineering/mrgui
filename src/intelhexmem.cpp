#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include "intelhexmem.h"

IntelHexMemory::IntelHexMemory()
{
	IntelHexMemory(16, 0xFF);
}

IntelHexMemory::IntelHexMemory(uint32_t size, uint8_t fillByte)
{
	this->memSz = 0;
	this->mem = (uint8_t*)malloc(size);
	this->bytesPerLine = 0x10;
	this->fillByte = fillByte;	
	if (NULL != this->mem)
	{
		this->memSz = size;
		memset(this->mem, this->fillByte, this->memSz);
	}
}

IntelHexMemory::IntelHexMemory(uint32_t size)
{
	this->memSz = 0;
	this->mem = (uint8_t*)malloc(size);
	this->bytesPerLine = 0x10;
	this->fillByte = 0xFF;	
	if (NULL != this->mem)
	{
		this->memSz = size;
		memset(this->mem, this->fillByte, this->memSz);
	}
}

IntelHexMemory::~IntelHexMemory()
{
	if (NULL != mem)
		free(mem);
}


int IntelHexMemory::resize(uint32_t newSize)
{
	uint8_t* newMem = (uint8_t*)malloc(newSize);
	if (NULL == newMem)
		return (-1);
	
	memset(newMem, this->fillByte, newSize);
	
	if (NULL != this->mem)
	{
		memcpy(newMem, this->mem, newSize);
		free(this->mem);
	}
	this->mem = newMem;
	this->memSz = newSize;
	
	return(0);
}

void IntelHexMemory::fillByteSet(uint8_t fillByte)
{
	this->fillByte = fillByte;
}

uint32_t IntelHexMemory::size_get()
{
	return(this->memSz);
}

uint8_t IntelHexMemory::read_uint8(uint32_t address)
{
	if (address >= this->memSz)
		return (0x00);
	
	return(this->mem[address]);
}

uint32_t IntelHexMemory::read_multibyte_be(uint32_t address, uint8_t sz)
{
	uint32_t retval = 0;
	if (address+(sz-1) >= this->memSz)
		return(0);
	
	switch(sz)
	{
		case 4:
			retval = this->mem[address++];
		case 3:
			retval<<=8;
			retval |= this->mem[address++];
		case 2:
			retval<<=8;
			retval |= this->mem[address++];			
		case 1:
			retval<<=8;
			retval |= this->mem[address];
		default:
			return(retval);
	
	}

	return(retval);
}

uint32_t IntelHexMemory::read_multibyte_le(uint32_t address, uint8_t sz)
{
	uint32_t retval = 0;
	if (address+(sz-1) >= this->memSz)
		return(0);
	
	address += sz-1;
	
	switch(sz)
	{
		case 4:
			retval = this->mem[address--];
		case 3:
			retval<<=8;
			retval |= this->mem[address--];
		case 2:
			retval<<=8;
			retval |= this->mem[address--];			
		case 1:
			retval<<=8;
			retval |= this->mem[address];
		default:
			return(retval);
	}

	return(retval);
}

int IntelHexMemory::write_multibyte_be(uint32_t address, uint32_t value, uint8_t sz)
{
	if (address+(sz-1) >= this->memSz)
		this->resize(address+sz);

	if (sz != 4 && value >= (uint32_t)(1<<(8*sz)))
		return(-2);
	
	switch(sz)
	{
		case 4:
			this->mem[address++] = 0xFF & (value>>24);
		case 3:
			this->mem[address++] = 0xFF & (value>>16);
		case 2:
			this->mem[address++] = 0xFF & (value>>8);
		case 1:
			this->mem[address] = 0xFF & (value);
			break;	
	}

	return(0);
}

int IntelHexMemory::write_multibyte_le(uint32_t address, uint32_t value, uint8_t sz)
{
	if (address+(sz-1) >= this->memSz)
		this->resize(address+sz);
	
	if (sz != 4 && value >= (uint32_t)(1<<(8*sz)))
		return(-2);

	address += sz-1;
	
	switch(sz)
	{
		case 4:
			this->mem[address--] = 0xFF & (value>>24);
		case 3:
			this->mem[address--] = 0xFF & (value>>16);
		case 2:
			this->mem[address--] = 0xFF & (value>>8);
		case 1:
			this->mem[address] = 0xFF & (value);
			break;	
	}

	return(0);
}

int IntelHexMemory::write_uint8(uint32_t address, uint8_t value)
{
	return write_multibyte_le(address, value, 1);
}


int IntelHexMemory::write_uint16_be(uint32_t address, uint16_t value)
{
	return write_multibyte_be(address, value, 2);	
}

int IntelHexMemory::write_uint24_be(uint32_t address, uint32_t value)
{
	return write_multibyte_be(address, value, 3);	
}

int IntelHexMemory::write_uint32_be(uint32_t address, uint32_t value)
{
	return write_multibyte_be(address, value, 4);	
}


int IntelHexMemory::write_uint16_le(uint32_t address, uint16_t value)
{
	return write_multibyte_le(address, value, 2);	
}

int IntelHexMemory::write_uint24_le(uint32_t address, uint32_t value)
{
	return write_multibyte_le(address, value, 3);	
}

int IntelHexMemory::write_uint32_le(uint32_t address, uint32_t value)
{
	return write_multibyte_le(address, value, 4);	
}

uint16_t IntelHexMemory::read_uint16_be(uint32_t address)
{
	return((uint16_t)read_multibyte_be(address, 2));
}

uint32_t IntelHexMemory::read_uint24_be(uint32_t address)
{
	return(0x00FFFFFF & read_multibyte_be(address, 3));
}

uint32_t IntelHexMemory::read_uint32_be(uint32_t address)
{
	return(read_multibyte_be(address, 4));
}

uint16_t IntelHexMemory::read_uint16_le(uint32_t address)
{
	return((uint16_t)read_multibyte_le(address, 2));
}

uint32_t IntelHexMemory::read_uint24_le(uint32_t address)
{
	return(0x00FFFFFF & read_multibyte_le(address, 3));
}

uint32_t IntelHexMemory::read_uint32_le(uint32_t address)
{
	return(read_multibyte_le(address, 4));
}

void IntelHexMemory::bytesPerLineSet(uint8_t bytesPerLine)
{
	if (bytesPerLine > 0)
		this->bytesPerLine = bytesPerLine;
}

void IntelHexMemory::write_ihex(FILE* outFile)
{
	uint32_t i, j;
	rewind(outFile);
	
	for(i=0; i<this->memSz;)
	{
		uint32_t recSize = this->memSz - i;
		uint8_t checksum = 0;
		
		if (recSize > this->bytesPerLine)
			recSize = this->bytesPerLine;
			
		fprintf(outFile, ":%02X%04X00", recSize, i);
		
		checksum = recSize + (0xFF & (i>>8)) + (0xFF & i);
		
		for(j=0; j<recSize; j++)
		{
			fprintf(outFile, "%02X", this->mem[i+j]);
			checksum += this->mem[i+j];
		}

		fprintf(outFile, "%02X\n", (uint8_t)(~checksum));
	
		i+=recSize;
	}

	// Record terminator	
	fprintf(outFile, ":00000001FF\n");
}

void IntelHexMemory::text_dump(FILE* outFile)
{
	uint32_t i, j;
	for(i=0; i<this->memSz;)
	{
		uint32_t recSize = this->memSz - i;
		
		if (recSize > this->bytesPerLine)
			recSize = this->bytesPerLine;
			
		fprintf(outFile, "0x%08X | ", i);

		for(j=0; j<recSize; j++)
			fprintf(outFile, "%02X ", this->mem[i+j]);
		printf("\n");
		i+=recSize;
	}	
}

uint32_t hexread(const char* str, uint8_t digits)
{
	uint32_t retval=0;
	uint8_t i;
	for(i = digits; i>0; i--)
	{
		uint8_t v = *(str + i - 1);

		if (v >= 'A' && v <= 'Z')
			v -= 'A' - 10;
		else if (v >= 'a' && v <= 'z')
			v -= 'a' - 10;
		else if (v >= '0' && v <= '9')
			v -= '0';
		else
			v = 0;
		
		retval |= ((uint32_t)v)<<(4*(digits - i));
	}
	return(retval);
}

int IntelHexMemory::read_ihex(FILE* inFile)
{
	uint8_t done = 0;
	if (NULL != this->mem)
	{
		free(this->mem);
		this->mem = NULL;
		this->memSz = 0;
	}

	this->mem = (uint8_t*)malloc(1);
	if (NULL == this->mem)
		return (-1);

	this->memSz = 1;

	rewind(inFile);
	
	while(!done && !feof(inFile))
	{
		// Line Format:
		// 0000000000111111111
		// 0123456789012345678
		// :BBAAAATTddddddCC
		// BB = byte count of data (d) section
		// AAAA = address
		// TT = type
		
		char lineBuffer[1024];	
		uint32_t byteCount = 0;
		uint32_t address = 0;
		uint32_t lineType = 0;
		
		memset(lineBuffer, 0, sizeof(lineBuffer));
		
		if (NULL == fgets(lineBuffer, sizeof(lineBuffer)-1, inFile))
			return(0);
		
		// All Intel Hex lines start with a colon.  If not, skippy skippy
		if (lineBuffer[0] != ':')
			continue;
		// If it's less than 11 bytes long, it's not a valid line
		if (strlen(lineBuffer) < 11)
			continue;
		
		byteCount = hexread(lineBuffer+1, 2);
		address = hexread(lineBuffer+3, 4);
		lineType = hexread(lineBuffer+7, 2);

		switch(lineType)
		{
			case 0x01: // End of file line
				done = 1;
				break;

			case 0x00: // Data line
				for (uint32_t i=0; i<byteCount; i++)
				{
					uint8_t byteValue;
					byteValue = (uint8_t)hexread(lineBuffer + 9 + i*2, 2);
					this->write_uint8(address+i, byteValue);
				}
				break;

			default:
				// Skip, one of those unhandled line type
				break;
		}		
	}
	
	return 0;
}


#ifdef TEST_IHEX

// test main function for intel hex
// compilation commandline: g++ -I. -DTEST_IHEX=1 intelhexmem.cpp -o main

int main(int argc, char* argv[])
{
	int retval = 0;
	IntelHexMemory eepromMem(256);
	

	// First row (16 bytes) is big endian writes
	retval = eepromMem.write_uint32_be(0x00, 0x12345678);
	if (0 != retval)
		printf("write_uint32_be() returned error %d\n", retval);
	retval = eepromMem.write_uint24_be(0x04, 0x123456);
	if (0 != retval)
		printf("write_uint24_be() returned error %d\n", retval);
	retval = eepromMem.write_uint16_be(0x08, 0x1234);
	if (0 != retval)
		printf("write_uint16_be() returned error %d\n", retval);
	retval = eepromMem.write_uint8(0x0C, 0x12);
	if (0 != retval)
		printf("write_uint8() returned error %d\n", retval);

	if(0x12345678 != (retval = eepromMem.read_uint32_be(0x00)))
		printf("read_uint32_be() incorrect readback 0x%08X\n", retval);	
	if(0x123456 != (retval = eepromMem.read_uint24_be(0x04)))
		printf("read_uint24_be() incorrect readback 0x%08X\n", retval);	
	if(0x1234 != (retval = eepromMem.read_uint16_be(0x08)))
		printf("read_uint16_be() incorrect readback 0x%08X\n", retval);	
	if(0x12 != (retval = eepromMem.read_uint8(0x0C)))
		printf("read_uint8() incorrect readback 0x%08X\n", retval);	



	// Second row (16 bytes) is little endian writes
	retval = eepromMem.write_uint32_le(0x10, 0x12345678);
	if (0 != retval)
		printf("write_uint32_le() returned error %d\n", retval);
	retval = eepromMem.write_uint24_le(0x14, 0x123456);
	if (0 != retval)
		printf("write_uint24_le() returned error %d\n", retval);
	retval = eepromMem.write_uint16_le(0x18, 0x1234);
	if (0 != retval)
		printf("write_uint16_le() returned error %d\n", retval);
	retval = eepromMem.write_uint8(0x1C, 0x12);
	if (0 != retval)
		printf("write_uint8() returned error %d\n", retval);
	
	if(0x12345678 != (retval = eepromMem.read_uint32_le(0x10)))
		printf("read_uint32_le() incorrect readback 0x%08X\n", retval);	
	if(0x123456 != (retval = eepromMem.read_uint24_le(0x14)))
		printf("read_uint24_le() incorrect readback 0x%08X\n", retval);	
	if(0x1234 != (retval = eepromMem.read_uint16_le(0x18)))
		printf("read_uint16_le() incorrect readback 0x%08X\n", retval);	
	if(0x12 != (retval = eepromMem.read_uint8(0x1C)))
		printf("read_uint8() incorrect readback 0x%08X\n", retval);	
	
	
	eepromMem.text_dump(stdout);
	
	FILE* ihexOutfile = fopen("test-dump.ihex", "w");
	eepromMem.write_ihex(ihexOutfile);
	fclose(ihexOutfile);

	printf("\n\n -- Readback --\n\n");
	
	FILE* ihexInfile = fopen("test-dump.ihex", "r");
	eepromMem.read_ihex(ihexInfile);
	fclose(ihexInfile);	

	eepromMem.text_dump(stdout);
	
	return(0);
}

#endif

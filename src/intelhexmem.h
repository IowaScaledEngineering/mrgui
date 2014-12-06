#ifndef _INTELHEXMEM_H_
#define _INTELHEXMEM_H_

#include <stdint.h>

class IntelHexMemory
{
	uint8_t* mem;
	uint32_t memSz;
	uint8_t bytesPerLine;
	uint8_t fillByte;
	
	private:
		uint32_t read_multibyte_be(uint32_t address, uint8_t sz);
		uint32_t read_multibyte_le(uint32_t address, uint8_t sz);
		int write_multibyte_be(uint32_t address, uint32_t value, uint8_t sz);
		int write_multibyte_le(uint32_t address, uint32_t value, uint8_t sz);

	
	public:
		IntelHexMemory();
		~IntelHexMemory();
		IntelHexMemory(uint32_t size);
		IntelHexMemory(uint32_t size, uint8_t fillByte);

		uint8_t read_uint8(uint32_t address);
		uint16_t read_uint16_be(uint32_t address);
		uint32_t read_uint24_be(uint32_t address);
		uint32_t read_uint32_be(uint32_t address);
		uint16_t read_uint16_le(uint32_t address);
		uint32_t read_uint24_le(uint32_t address);
		uint32_t read_uint32_le(uint32_t address);

		int write_uint8(uint32_t address, uint8_t value);
		int write_uint16_be(uint32_t address, uint16_t value);
		int write_uint24_be(uint32_t address, uint32_t value);
		int write_uint32_be(uint32_t address, uint32_t value);
		int write_uint16_le(uint32_t address, uint16_t value);
		int write_uint24_le(uint32_t address, uint32_t value);
		int write_uint32_le(uint32_t address, uint32_t value);
		
		void text_dump(FILE* outFile);
		int read_ihex(FILE* inFile);
		int write_ihex(FILE* outFile);
		uint32_t size_get();		
		int resize(uint32_t newSize);
		void bytesPerLineSet(uint8_t bytesPerLine);
		void fillByteSet(uint8_t fileByte);

} ;

#endif


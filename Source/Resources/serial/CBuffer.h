#ifndef CBUFFER_H
#define CBUFFER_H

// include data types
#include ".\mbedLibrary\mbed.h"

#define BUFFER_SIZE 1024
#ifndef TRUE
#define TRUE	0x1
#define FALSE	0x0
#endif

typedef struct
{
	uint8_t data[BUFFER_SIZE];
	uint32_t wptr;
	uint32_t rptr;
}BUFFER_T;

class CBuffer
{
public:
	CBuffer();
	// verify a read is available
	uint8_t IsReadable();
	uint32_t BuffSpace();
	// ways to read - single byte return
	uint8_t ReadBuff(void);
	// read the entire buffer
	uint8_t ReadBuff(uint8_t*, uint32_t);
	// various ways to write data
	uint8_t WriteBuff(uint8_t);
	uint8_t WriteBuff(uint8_t*, uint32_t);
	uint8_t WriteBuff(int8_t*);
	uint8_t WriteBuff(char*);
private:
	// the main item
	volatile BUFFER_T buf;
	// used in BuffSpace
	uint32_t temp_read_loc, temp_write_loc, data_amount;
	// used in ReadBuf - make sure return is not clobbered
	uint8_t temp_data;
	// used in ReadBuff
	uint32_t cnt_a;
	// used in WriteBuff
	uint32_t cnt_b, cnt_c;
};

#endif

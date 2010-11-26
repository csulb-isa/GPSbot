/*
 *	PROJECT: Optimized Class development for ARM Cortex M3	
 *			
 *	DATE STARTED: 19-June-2010
 *		
 *	DATE LAST MODIFIED: 
 *		
 *	AUTHOR: sgrove
 *		
 *	NOTES:
 *		rev 1.0 -> Starting to build the class for use with FIFO packet data.
 *					Initial data size is 8bits. 
 *
 */

#include "CBuffer.h"

CBuffer::CBuffer()
{
	buf.rptr = buf.wptr = 0;
}

uint8_t CBuffer::IsReadable(void)
{
  	// is unread data present??
	if(buf.rptr != buf.wptr)
		return TRUE;
	// std return
	return FALSE;
}

uint32_t CBuffer::BuffSpace()
{
	temp_read_loc = buf.rptr;
	temp_write_loc = buf.wptr;
	data_amount = 0;
	
	while (temp_read_loc != temp_write_loc)
	{
		data_amount++;
		temp_read_loc++;
		if(temp_read_loc >= BUFFER_SIZE)
			temp_read_loc = 0;
	}
	return data_amount;
}

uint8_t CBuffer::ReadBuff(void)
{	
	temp_data = buf.data[buf.rptr++];
	if (buf.rptr >= BUFFER_SIZE)
		buf.rptr = 0;
	// pass it back for use
	return temp_data;
}

uint8_t CBuffer::ReadBuff(uint8_t* data, uint32_t amount)
{
	cnt_a = 0;
	while(IsReadable() == TRUE)
	{
		data[cnt_a++] = buf.data[buf.rptr++];
		if(buf.rptr >= BUFFER_SIZE)
			buf.rptr = 0;
		if((--amount) == 0)
			return TRUE;
	}
	return FALSE;
}

uint8_t CBuffer::WriteBuff(uint8_t data)
{
	// for larger write requests we should
	// check and make sure there is room for 
	// the data.
	buf.data[buf.wptr++] = data;
	// prepare for the next access
	if (buf.wptr >= BUFFER_SIZE)
		buf.wptr = 0;
	return TRUE;
}

uint8_t CBuffer::WriteBuff(uint8_t* data, uint32_t amount)
{
	cnt_b = 0;
	// for larger write requests we should
	// check and make sure there is room for 
	// the data.
	while ((amount--) != 0)
	{
		buf.data[buf.wptr++] = data[cnt_b++];
		// prepare for the next access
		if (buf.wptr >= BUFFER_SIZE)
			buf.wptr = 0;
	}
	return TRUE;
}

uint8_t CBuffer::WriteBuff(char* data)
{
	cnt_c = 0;
	// for larger write requests we should
	// check and make sure there is room for 
	// the data.
	while (data[cnt_c] != 0x00)
	{
		buf.data[buf.wptr++] = data[cnt_c++];
		// prepare for the next access
		if (buf.wptr >= BUFFER_SIZE)
			buf.wptr = 0;
	}
	return TRUE;
}

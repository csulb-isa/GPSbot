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
 *		rev 1.0 -> Starting to build the class from a CSerial class previously built.
 *					Incorporating a new Class, CBuffer to manage tx and rx data. 
 *
 */

#include "CUart.h"

// this is necessary for interrupt members 
CUart3 *CUart3::instance;

CUart3::CUart3(PinName tx, PinName rx)	:	_txbuf(), _rxbuf()
{
	// store the pins that the class has been derived with
	tx_pin = (tx&0xfff);
	rx_pin = (rx&0xfff);

	switch(tx)
	{
		case P0_0:	
		case P0_25:
		case P4_28:
			switch(rx)
			{
				case P0_1:									
					LPC_PINCON->PINSEL0 |= (2<<((tx_pin&0xff)*2));
					LPC_PINCON->PINSEL0 |= (2<<((rx_pin&0xff)*2));
					break;	
				case P0_26:									
					LPC_PINCON->PINSEL1 |= (3<<(((tx_pin&0xff)-16)*2));	
					LPC_PINCON->PINSEL1 |= (3<<(((rx_pin&0xff)-16)*2));	
					break;
				case P4_29:								
					LPC_PINCON->PINSEL9 |= (3<<(((tx_pin&0xff)-16)*2));	
					LPC_PINCON->PINSEL9 |= (3<<(((rx_pin&0xff)-16)*2));	
					break;
				default:
					//while(1);
					break;					
			}
			
			instance = this;									/* point to this object for member ISR routine */	
			NVIC_SetVector(UART3_IRQn, (uint32_t)&_uart3ISR); 	/* change the address of the interrupt handler to a member function */
			NVIC_EnableIRQ(UART3_IRQn);
			break;
		
		default:
			// invalid pin selection for UART0 peripheral
		//	while(1);
			break;	
	}
}

void CUart3::Baud(uint32_t baud)
{
 	// Find out what the Peripheral Clk prescaler
	// is set at before computing
	switch ((LPC_SC->PCLKSEL1>>18)&0x3)
	{
		case 0x00:
		default:
			pclk = SystemFrequency/4;
			break;
		case 0x01:
			pclk = SystemFrequency;
			break;
		case 0x02:
			pclk = SystemFrequency/2;
			break;
		case 0x03:
			pclk = SystemFrequency/8;					
			break;
	}		
					
	fdiv = ((pclk/16)/baud);	// change the baud rate 
	LPC_SC->PCONP |= (1<<25);	// turn on the peripheral control and clock enable
	LPC_UART3->LCR |= 0x83;		// 8 bits, no Parity, 1 Stop bit, enable change to divisor mod registers
	LPC_UART3->DLM = fdiv/256;	// divisor latch MSB register 
	LPC_UART3->DLL = fdiv%256;	// divisor latch LSB register 
	LPC_UART3->LCR = 0x03;		// disable the divisor modifier registers change enable
	LPC_UART3->FCR = 0x07;		// Enable and reset TX and RX FIFO. 
}

void CUart3::Enable()
{
	// turn on the peripheral control and clock enable
	LPC_SC->PCONP |= (1<<25);
	// Enable UART0 interrupt for tx and rx
	LPC_UART3->IER = (IER_RBR | IER_THRE);
}

void CUart3::Disable()
{
	// disable the interrupts
	LPC_UART3->IER = 0;
	// Delete the vector entry
	//NVIC_DisableIRQ(UART3_IRQn);
	//turn off the control and clock enable
	LPC_SC->PCONP &= ~(1<<25);
}

// when the interrupt occurs, point to the instance 
// of the interrupt so access to members (this) 
// becomes available to handle the ISR

void CUart3::_uart3ISR()
{
	instance->uart3ISR();
}

void CUart3::Attach(void(*fptr)(void))
{
	NVIC_SetVector(UART3_IRQn, (uint32_t)fptr);
}

void CUart3::Detach()
{
 	instance = this;
	NVIC_SetVector(UART3_IRQn, (uint32_t)&_uart3ISR);
}

void CUart3::uart3ISR()
{
	// reading IIR will clear the interrupt active bit 
	IIRstatus = LPC_UART3->IIR;
	// baby steps to make it readable 	
	IIRstatus >>= 1;
	IIRstatus &= 0x7;
	
	// determine the cause and handle approperately
	switch(IIRstatus)
	{
		// Peripheral options that are not now supported or used
		case IIR_RLS:
		case IIR_CTI:
		default:
			break;
		// we have new data 
		case IIR_RDA:				
			_rxbuf.WriteBuff(LPC_UART3->RBR);
			break;
			
		// we have completed a transmit -> is there more to send?
		case IIR_THRE:				
			if (_txbuf.IsReadable())
				LPC_UART3->THR = _txbuf.ReadBuff();
			break;		
	}
	
	return;
}

uint8_t CUart3::Write(char* data)
{
	// put data into the buffer
	_txbuf.WriteBuff(data);
	// test the transmit full buffer 
	// may want to test the buffer level instead - come back to this
	if (((LPC_UART3->LSR>>5)&0x1) == 1)			/* test for a full transmit buffer */
		LPC_UART3->THR = _txbuf.ReadBuff();
	
	return TRUE;
}

uint8_t CUart3::Write(uint8_t* data, uint16_t size)
{
	// put data into the buffer
	_txbuf.WriteBuff(data, size);
	// test the transmit full buffer 
	// may want to test the buffer level instead - come back to this
	if (((LPC_UART3->LSR>>5)&0x1) == 1)			/* test for a full transmit buffer */
		LPC_UART3->THR = _txbuf.ReadBuff();
		
	return TRUE;
}

uint8_t CUart3::Write(uint8_t data)
{
	// put data into the buffer
	_txbuf.WriteBuff(data);
	// test the transmit full buffer 
	// may want to test the buffer level instead - come back to this
	if (((LPC_UART3->LSR>>5)&0x1) == 1)			/* test for a full transmit buffer */
		LPC_UART3->THR = _txbuf.ReadBuff();
		
	return TRUE;
}

uint8_t CUart3::IsReadable()
{
	return _rxbuf.IsReadable();
}

uint8_t CUart3::Read()
{
	return _rxbuf.ReadBuff();
}

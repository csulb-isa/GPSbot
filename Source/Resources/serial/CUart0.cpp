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
CUart0 *CUart0::instance;

CUart0::CUart0(PinName tx, PinName rx)	:	_txbuf(), _rxbuf()
{
	// store the pins that the class has been derived with
	tx_pin = (tx&0xfff);
	rx_pin = (rx&0xfff);

	switch(tx)
	{
		case P0_2:
			switch(rx)
			{
				// only valid RX & TX pin combination for UART0
				case P0_3:	
					// tx pin connect
					LPC_PINCON->PINSEL0 |= (1<<((tx_pin&0xff)*2));
					// rx pin connect
					LPC_PINCON->PINSEL0 |= (1<<((rx_pin&0xff)*2));
					break;
				default:
					// invalid pin selection for UART0 peripheral
					//while(1);
					break;
			}
			// must derive an instance for the member based ISR
			// routine, this is not accessable from within the class
			instance = this;
			// pass the new address of the ISR function
			NVIC_SetVector(UART0_IRQn, (uint32_t)&_uart0ISR);
			NVIC_EnableIRQ(UART0_IRQn);
			break;
		
		default:
			// invalid pin selection for UART0 peripheral
			//while(1);
			break;	
	}
}

void CUart0::Baud(uint32_t baud)
{
 	// Find out what the Peripheral Clk prescaler
	// is set at before computing
	switch ((LPC_SC->PCLKSEL0>>6)&0x3)
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
	LPC_SC->PCONP |= (1<<3);	// turn on the peripheral control and clock enable
	LPC_UART0->LCR |= 0x83;		// 8 bits, no Parity, 1 Stop bit, enable change to divisor mod registers
	LPC_UART0->DLM = fdiv/256;	// divisor latch MSB register 
	LPC_UART0->DLL = fdiv%256;	// divisor latch LSB register 
	LPC_UART0->LCR = 0x03;		// disable the divisor modifier registers change enable
	LPC_UART0->FCR = 0x07;		// Enable and reset TX and RX FIFO. 
}

void CUart0::Enable()
{
	// turn on the peripheral control and clock enable
	LPC_SC->PCONP |= (1<<3);
	// Enable UART0 interrupt for tx and rx
	LPC_UART0->IER = (IER_RBR | IER_THRE);
}

void CUart0::Disable()
{
	// disable the interrupts
	LPC_UART0->IER = 0;
	// Delete the vector entry
	//NVIC_DisableIRQ(UART0_IRQn);
	//turn off the control and clock enable
	LPC_SC->PCONP &= ~(1<<3);
}

// when the interrupt occurs, point to the instance 
// of the interrupt so access to members (this) 
// becomes available to handle the ISR

void CUart0::_uart0ISR()
{
	instance->uart0ISR();
}

void CUart0::Attach(void(*fptr)(void))
{
	NVIC_SetVector(UART0_IRQn, (uint32_t)fptr);
}

void CUart0::Detach()
{
 	instance = this;
	NVIC_SetVector(UART0_IRQn, (uint32_t)&_uart0ISR);
}

void CUart0::uart0ISR()
{
	// reading IIR will clear the interrupt active bit 
	IIRstatus = LPC_UART0->IIR;
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
			_rxbuf.WriteBuff(LPC_UART0->RBR);
			break;
			
		// we have completed a transmit -> is there more to send?
		case IIR_THRE:				
			if (_txbuf.IsReadable())
				LPC_UART0->THR = _txbuf.ReadBuff();
			break;		
	}
	
	return;
}

uint8_t CUart0::Write(char* data)
{
	// put data into the buffer
	_txbuf.WriteBuff(data);
	// test the transmit full buffer 
	// may want to test the buffer level instead - come back to this
	if (((LPC_UART0->LSR>>5)&0x1) == 1)
		LPC_UART0->THR = _txbuf.ReadBuff();
	
	return TRUE;
}

uint8_t CUart0::Write(uint8_t* data, uint16_t size)
{
	// put data into the buffer
	_txbuf.WriteBuff(data, size);
	// test the transmit full buffer 
	// may want to test the buffer level instead - come back to this
	if (((LPC_UART0->LSR>>5)&0x1) == 1)			/* test for a full transmit buffer */
		LPC_UART0->THR = _txbuf.ReadBuff();
		
	return TRUE;
}

uint8_t CUart0::Write(uint8_t data)
{
	// put data into the buffer
	_txbuf.WriteBuff(data);
	// test the transmit full buffer 
	// may want to test the buffer level instead - come back to this
	if (((LPC_UART0->LSR>>5)&0x1) == 1)			/* test for a full transmit buffer */
		LPC_UART0->THR = _txbuf.ReadBuff();
		
	return TRUE;
}

uint8_t CUart0::IsReadable()
{
	return _rxbuf.IsReadable();
}

uint8_t CUart0::Read()
{
	return _rxbuf.ReadBuff();
}

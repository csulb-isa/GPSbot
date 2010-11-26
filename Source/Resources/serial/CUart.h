#ifndef CUART_H
#define CUART_H

// Class files that are building blocks for the class
#include ".\mbedLibrary\PinNames.h"
#include ".\mbedLibrary\cmsis_nvic.h"
#include ".\Resources\serial\CBuffer.h"

#ifndef SystemFrequency
#define SystemFrequency	96000000UL
#endif

#ifndef TRUE
#define TRUE	0x1
#define FALSE	0x0
#endif

/* Interrupt bit defines for register IER */
#define IER_RBR		(0x1<<0)
#define IER_THRE	(0x1<<1)
#define IER_RLS		(0x1<<2)
#define IER_ABE		(0x1<<9)
/* Interrupt bit definitions for register IIR */
#define IIR_RLS		0x3
#define IIR_RDA		0x2
#define IIR_CTI		0x6
#define IIR_THRE	0x1
#define IIR_MASK	0xe

class CUart0
{
private:
	// pointer from the instance to the acutal method
	static void _uart0ISR();
	// make sure we can have member ISR functions
	static CUart0 *instance;
	// location that the ISR exists at
	void uart0ISR();
	// pins that are used to build the object
	uint32_t tx_pin, rx_pin;
	// used in Baud method
	uint32_t fdiv, pclk;
	// used in ISR to read the flag that set the ISR off
	// and to clear the action
	uint8_t IIRstatus;
	// buffers for data
	CBuffer _txbuf;
	CBuffer _rxbuf;

public:
	// constructor
	CUart0(PinName, PinName);
	// change the baudrate of the object
	void Baud(uint32_t);
	// enable the object and disable the interrupts
	void Enable();
	void Disable();
	// method for attaching and detaching an external interrupt
	void Attach(void (*fptr)(void));
	void Detach();
	// methods for reading
	uint8_t IsReadable();
	uint8_t Read();
	// methods for writing
	uint8_t Write(char*);
	uint8_t Write(uint8_t*, uint16_t);
	uint8_t Write(uint8_t);
};

class CUart1
{
private:
	// make sure we can have member ISR functions
	static CUart1 *instance;
	// location that the ISR exists at
	void uart1ISR();
	// pointer from the instance to the acutal method
	static void _uart1ISR();
	// pins that are used to build the object
	uint32_t tx_pin, rx_pin;
	// used in Baud method
	uint32_t fdiv, pclk;
	// used in ISR to read the flag that set the ISR off
	// and to clear the action
	uint8_t IIRstatus;
	// buffers for data
	CBuffer _txbuf;
	CBuffer _rxbuf;

public:
	// constructor
	CUart1(PinName, PinName);
	// change the baudrate of the object
	void Baud(uint32_t);
	// enable the object and disable the interrupts
	void Enable();
	void Disable();
	// method for attaching and detaching an external interrupt
	void Attach(void (*fptr)(void));
	void Detach();
	// methods for reading
	uint8_t IsReadable();
	uint8_t Read();
	// methods for writing
	uint8_t Write(char*);
	uint8_t Write(uint8_t*, uint16_t);
	uint8_t Write(uint8_t);
};

class CUart2
{
private:
	// make sure we can have member ISR functions
	static CUart2 *instance;
	// location that the ISR exists at
	void uart2ISR();
	// pointer from the instance to the acutal method
	static void _uart2ISR();
	// pins that are used to build the object
	uint32_t tx_pin, rx_pin;
	// used in Baud method
	uint32_t fdiv, pclk;
	// used in ISR to read the flag that set the ISR off
	// and to clear the action
	uint8_t IIRstatus;
	// buffers for data
	CBuffer _txbuf;
	CBuffer _rxbuf;

public:
	// constructor
	CUart2(PinName, PinName);
	// change the baudrate of the object
	void Baud(uint32_t);
	// enable the object and disable the interrupts
	void Enable();
	void Disable();
	// method for attaching and detaching an external interrupt
	void Attach(void (*fptr)(void));
	void Detach();
	// methods for reading
	uint8_t IsReadable();
	uint8_t Read();
	// methods for writing
	uint8_t Write(char*);
	uint8_t Write(uint8_t*, uint16_t);
	uint8_t Write(uint8_t);
};

class CUart3
{
private:
	// make sure we can have member ISR functions
	static CUart3 *instance;
	// location that the ISR exists at
	void uart3ISR();
	// pointer from the instance to the acutal method
	static void _uart3ISR();
	// pins that are used to build the object
	uint32_t tx_pin, rx_pin;
	// used in Baud method
	uint32_t fdiv, pclk;
	// used in ISR to read the flag that set the ISR off
	// and to clear the action
	uint8_t IIRstatus;
	// buffers for data
	CBuffer _txbuf;
	CBuffer _rxbuf;

public:
	// constructor
	CUart3(PinName, PinName);
	// change the baudrate of the object
	void Baud(uint32_t);
	// enable the object and disable the interrupts
	void Enable();
	void Disable();
	// method for attaching and detaching an external interrupt
	void Attach(void (*fptr)(void));
	void Detach();
	// methods for reading
	uint8_t IsReadable();
	uint8_t Read();
	// methods for writing
	uint8_t Write(char*);
	uint8_t Write(uint8_t*, uint16_t);
	uint8_t Write(uint8_t);
};

#endif

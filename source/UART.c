/*****************************************************************************
* Copyright (C) 2021 by Taher Ujjainwala
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Taher Ujjainwala and the University of Colorado are not liable for
* any misuse of this material.
*
**************************************************************************/


/*************************
 *
 *
 *
 *    File name   : UART.c
 *    Description : This file defines all the functions to Initialize UART0, Ties the getchar(), sscanf() with sysread(), also ties printf() and putchar() with syswrite()
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *    Date  : 11/09/2021
 *
 *
 */

//Including all the header files
#include "MKL25Z4.h"
#include "uart.h"
#include "cbfifo.h"

//Defining the UART parameters and easy to change in future
#define UART_OVERSAMPLE_RATE 	(16)		//Over sampling rate
#define SYS_CLOCK				(24e6)		//Running the system on 24MHz
#define PARITY      			(0)			//Parity None
#define DATA_BITS   			(0)         //2 modes for data - 8 bits and 9 bits --> For 8 bits - 0 and For 9 bits - 1
#define STOP_BITS   			(1)         //2 modes for Stop bit --> For 1 stop bit - 0 and For 2 Stop bits - 1

//Function Initializing UART
void Init_UART0(uint32_t baud_rate) {
	uint16_t sbr;
	//uint8_t temp;

	//Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;


	//Make sure transmitter and receiver are disabled before initializing
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	//Set UART clock to 24 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);//mcg or mcg/2 clock

	//Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	//Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	//Disable interrupts for RX active edge and LIN break detect, select one stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOP_BITS) | UART0_BDH_LBKDIE(0);

	//Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_BITS) | UART0_C1_PE(PARITY);

	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	//Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	//Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);


	//Enable interrupts. Listing 8.11 on p. 234

	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	//Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);

	//Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	//temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

	//Was getting warming when removed this lines. Hence to remove the warning this is commented
	//if(temp)
	//{}
}

//UART0 Interrupt handler
void UART0_IRQHandler(void) {

	uint8_t ch;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
			UART_S1_FE_MASK | UART_S1_PF_MASK))
	{

			//Clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
					UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			//Read the data register to clear RDRF
			ch = UART0->D;
	}

	if (UART0->S1 & UART0_S1_RDRF_MASK) 			//Read interrupt
	{
		//Received a character
		ch = UART0->D;
		if(!(cbfifo_capacity(RXD) == cbfifo_length(RXD)))
		{
			cbfifo_enqueue(&ch, 1, RXD);			//Enqueue the character to Receive Buffer
		}
		else
		{
			//Discard the character because the queue is full
		}

	}

	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) )  // tx buffer empty
	{
		// can send another character
		if(cbfifo_length(TXD)!=0) //not empty
		{
			 cbfifo_dequeue(&ch, 1, TXD);			//Dequeue the character from Transmit buffer to UART
			 UART0->D = ch;
		}
		else
			UART0->C2 &= ~UART0_C2_TIE_MASK;		//Queue is empty so disabling the Trasnmit interrupt
	}
}

//sysread function tied to work internally like getchar() and sscanf()
//This function dequeques the character from receiver buffer
int __sys_readc(void)
{
	int ch;

	while(cbfifo_length(RXD)==0)				//Will return -1 if the receive buffer is empty
					;
	if (cbfifo_dequeue(&ch, 1, RXD) == 1)
	{
		return ch;
	}
	else
	{
		return -1;
	}
}


//syswrite function tied to work internally like putchar() and printf()
//This function enqueques the character from transmit buffer
int __sys_write(int handle, char *buf, int size)
{

	while(cbfifo_length(TXD) == cbfifo_capacity(TXD))
		;

	if (cbfifo_enqueue(buf, size, TXD) == 0)				//Will return -1 if buffer is full
	{

		return -1;
	}

	else
	{
		// start transmitter if it isn't already running
		if (!(UART0->C2 & UART0_C2_TIE_MASK)) {
		UART0->C2 |= UART0_C2_TIE(1);
		}
		return  0;
	}
}

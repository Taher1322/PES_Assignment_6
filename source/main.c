/*****************************************************************************
* Copyright (C) 2021 by Taher Ujjainwala and Howdy Pierce
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
 *    File name   : hexdump.c
 *    Description : This file defines the function to implement hexdump
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides, PES Assignment 2 and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2  (First tested to pass all Howdy Test cases and used as it is for BreakkfastSerial)
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *
 *    Date  : 11/09/2021
 *
 *
 */

//Including all the header files
#include "sysclock.h"
#include "MKL25Z4.h"
#include "UART.h"
#include "test_cbfifo.h"
#include <stdio.h>
#include "hexdump.h"
#include "command_line.h"
char c;

int main(void)
{
	//Initializing the System Clock and UART
	sysclock_init();
	Init_UART0(38400);

	//Running test suite on Circular Buffer
	test_cbfifo();

	//Interactive statements on terminal
	printf( "Welcome to BreakfastSerial \r\n");
	printf("?");
//	printf("Enter command to perform the following functions as shown:\r\n");
//	printf("'Author' : Name of the Author \r\n");
//	printf("'Dump <address> <total length>(valid length:0 to 640)':Performs a HEXDUMP\r\n");
//	printf("'Help' : Get the help menu\r\n");

//	hexdump(0, 64);
//
//	hexdump(0xa0, 0x20);

	//Enter infinite loop
	while (1)
	{
		serial_input();
	}

  return 0 ;
}

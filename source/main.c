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
 *    File name   : main.c - application entry point
 *    Description : This file call internal functions to read user input data and based on that perform certain actions
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *
 *    Date  : 11/11/2021
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

	//Enter infinite loop
	while (1)
	{
		serial_input();    //Reading user input from terminal
	}

  return 0 ;
}

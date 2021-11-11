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
 *    File name   : command_line.c
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
#include "command_line.h"
#include "uart.h"
#include "cbfifo.h"
#include <string.h>
#include <stdio.h>
#include "command_process.h"

#define SERIAL_READ_BUFFER     	(2000)
#define ENTER_KEY_VALUE 		 	(0X0D)
#define BACKSPACE_KEY_VALUE      	(0X08)

void serial_input()
{
  char buffer[SERIAL_READ_BUFFER];
  char ch;
  int i = 0;

  while(1)
  {
	  while((cbfifo_length(RXD)==0)){
		  ;
	  }
	  cbfifo_dequeue(&ch, 1, RXD);
	  putchar(ch);

	  if(ch == ENTER_KEY_VALUE) //check if user pressed enter
	  {
		  buffer[i]='\0'; //append the string if user pressed enter
		  break;
	  }
	  else if(ch == BACKSPACE_KEY_VALUE)
	  {
		  i--;
		  printf(" \b");

	  }
	  else
	  {
		  buffer[i] =  ch;
		  i++;
	  }

  }
  printf("\n\r");
  process_command(buffer);
}


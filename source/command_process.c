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
 *    File name   : command_process.c
 *    Description : This file defines the function to implement hexdump
 *
 *    Author TAHER UJJAINWALA
 * 	  Tools : MCUXpressor IDE
 * 	  General References used in development:
 * 	  Howdy lectures slides, PES Assignment 2 and Alexander Dean UART section
 * 	  PES Assignment 2 - https://github.com/Taher1322/PES_Assignment_2  (First tested to pass all Howdy Test cases and used as it is for BreakkfastSerial)
 * 	  DEAN UART section - https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo
 *	  Major part of this code was inspired by Howdy Lecture on Command Processing
 *
 *    Date  : 11/11/2021
 *
 *
 */

//Including all the header files
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "hexdump.h"
#include <stdint.h>

#define MAX_HEX_DUMP (640)

typedef void (*command_handler_t)(int, char *argv[]);

//Function prototypes to handle each commands
static void author_handler(int argc, char *argv[]);
static void dump_handler(int argc, char *argv[]);
static void help_handler(int argc, char *argv[]);
static void info_handler(int argc, char *argv[]);

//Structure to define commands
typedef struct {
  const char *name;
  command_handler_t handler;
  const char *help_string;
} command_table_t;

//Look up table with all the possible commands - can be modifies in future with more
static const command_table_t commands[] =
{
		{"author", 	author_handler, 	"author help"},
		{"dump", 	dump_handler, 		"dump help"},
		{"help", 	help_handler, 		"terminal help"},
		{"info", 	info_handler, 		"info help"}
  // TODO: fill out command table here
};

//Calculate the total number of commands
static const int num_commands =
  sizeof(commands) / sizeof(command_table_t);


//Function to print the Author name on user input of Author
static void author_handler (int argc, char *argv[])
{
	printf ("TAHER UJJAINWALA\r\n");
	printf ("?");
}

//Function to decode and call Hex dump
static void dump_handler(int argc, char *argv[])
{
	uint32_t length = 0;
	uint32_t start_address = 0;

	//Reading data from serial terminal and storing that in start_address
	//User can define in decimal as well as hex
	if((*argv[1]>= '0')&&(*argv[1]<= '9'))
	{
		sscanf(argv[1],"%x",&start_address);


		//Reading and storing the input in Hex defined by User else storing the decimal
		if(strstr((char*)argv[2],"0x") || strstr((char*)argv[2],"0X"))
			{sscanf(argv[2],"%x",&length);}
		else
			{sscanf(argv[2],"%d",&length);}

		//Checking the max limit condition of Hex dump
		if(length>MAX_HEX_DUMP)
		{
			printf("Length out of range. Enter length between 1 to 640 in Decimal or 0x01 to 0x280 in Hex\r\n");
			printf ("?");
			return;
		}

		//Calling HEX DUMP
		hexdump(start_address,length);
		printf ("?");
	}
	else
	{
		printf("Unknown Command\r\n");			//Error check if command not entered properly
	    printf ("?");
	}
}


//Function to print all commands when Help command is executed
static void help_handler(int argc, char *argv[])
{
	printf("Command: Author ; Arguments: <> ; Description: Prints a string with your name.\r\n");
	printf("Command: Dump ; Arguments: <Start>, <Len> ; Description: Prints a hexdump of the memory requested \r\n");
	printf("Command: Info ; Arguments: <> ; Description: Prints Build Information.\r\n");
	printf ("?");
}


static void info_handler(int argc, char *argv[])
{
	printf("Version 1.0 built on laptop-303ne06e@ujjai at 2021-11-11 4:33\r\n");
	printf("Commit: 2e06a7fec8a90002f1fe69c027f385c8635ae608\r\n");
	printf("?");
}



//Function to decode the data, perform Lexical Analysis, and run the code for each specific commands
void process_command(char *input)
{
	char *p = input;
	char *end;
	char *argv[10];
	int argc = 0;
	// find end of string
	for (end = input; *end != '\0'; end++)
	         ;

	// Tokenize input in place
	bool in_token = false;
	memset(argv, 0, sizeof(argv));

	//Checking the End of string and adding Null character. Once that is done, storing the string and values in argv which is then passed with argc
	for (p = input; p <end-1; p++)
	{
		switch(in_token){
		case false:
			if((*p!=' ')||(*p!='\t'))
			{
				argv[argc]	= p;
				argc++;
				in_token = true;
			}
			break;
		case true:
			if((*p==' ')||(*p=='\t'))
			{
				*p='\0';
				in_token = false;
			}
			break;
		}
	}


	argv[argc] = NULL;		//Append string

	if (argc == 0)   		//No command
	{
		return;
	}

	//Comparing from lookup table and calling the functions based on user inputs
	 for (int i=0; i < num_commands; i++)
	 {

		 if (strcasecmp(argv[0], commands[i].name) == 0)
		 {
			 commands[i].handler(argc, argv);
			 break;
	     }
		 //Error handling
		 if(i == (num_commands-1))
		 {
			 printf("Unknown Command\r\n");
			 printf ("?");
		 }
	 }

}

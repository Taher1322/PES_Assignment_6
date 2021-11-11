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
 *
 *
 *    Date  : 11/09/2021
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

typedef void (*command_handler_t)(int, char *argv[]);

static void author_handler(int argc, char *argv[]);
static void dump_handler(int argc, char *argv[]);
static void help_handler(int argc, char *argv[]);
static void info_handler(int argc, char *argv[]);

typedef struct {
  const char *name;
  command_handler_t handler;
  const char *help_string;
} command_table_t;

static const command_table_t commands[] =
{
		{"author", 	author_handler, 	"author help"},
		{"dump", 	dump_handler, 		"dump help"},
		{"help", 	help_handler, 		"terminal help"},
		{"info", 	info_handler, 		"info help"}
  // TODO: fill out command table here
};

static const int num_commands =
  sizeof(commands) / sizeof(command_table_t);



static void author_handler (int argc, char *argv[])
{
	printf ("TAHER UJJAINWALA\r\n");
	printf ("?");
}

static void dump_handler(int argc, char *argv[])
{
	uint32_t length = 0;
	uint32_t start_address = 0;

	if((*argv[1]>= '0')&&(*argv[1]<= '9'))
	{
		sscanf(argv[1],"%x",&start_address);


		if(strstr((char*)argv[2],"0x") || strstr((char*)argv[2],"0X"))//if input is in hex
			{sscanf(argv[2],"%x",&length);}
		else
			{sscanf(argv[2],"%d",&length);}

		if(length>640){
			printf("Length out of range. Enter length between 1 to 640 in Decimal or 0x01 to 0x280 in Hex\r\n");
			printf ("?");
			return;
		}

		hexdump(start_address,length);
		printf ("?");
	}
	else
	{
		printf("Unknown Command\r\n");
	    printf ("?");
	}
}


static void help_handler(int argc, char *argv[])
{
	printf("Command: Author ; Arguments: <> ; Description: Prints a string with your name.\r\n");
	printf("Command: Dump ; Arguments: <Start>, <Len> ; Description: Prints a hexdump of the memory requested \r\n");
	printf("Command: Info ; Arguments: <> ; Description: Prints Build Information.\r\n");
	printf ("?");
}

static void info_handler(int argc, char *argv[])
{

}




void process_command(char *input) //code from howdy's lecture 11
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


	argv[argc] = NULL;//append string

	if (argc == 0)   // no command
	{
		return;
	}

	 for (int i=0; i < num_commands; i++)
	 {

		 if (strcasecmp(argv[0], commands[i].name) == 0)
		 {
			 commands[i].handler(argc, argv);
			 break;
	     }
		 if(i == (num_commands-1))
		 {
			 printf("Unknown Command\r\n");
			 printf ("?");
		 }
	 }

}

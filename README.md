# PES_Assignment_6

Code for Assignment 6 for PES, ECEN-5813, Fall 2021 - Due Date >> 11/11/2021 @ 10:30 AM MDT

# Author: TAHER UJJAINWALA </br>

Contact me if you are facing issue in execution of the code @ tauj5361@colorado.edu

# BreakfastSerial </br>

# Project source details </br>
-main.c --> Main script initialized UART and calls serial_input() while to read data from user </br>
-command_line.c --> Reads the data entered by used on terminal and sends the string for further processing </br>
-command_process.c --> Decode the data received from serial and defines the commands line interface functions </br>
-hexdump.c --> prints hex dump from the starting address till the length defined by user </br>
-sysclock. --> This file initialized the system clock </br>
-UART.c --> Initialization function, interrupt handler and standard printf() and getchar() function implementation </br>
-cbfifo.c --> This file implements circular buffer functionalities - enqueue, dequeue, length, capacity and reset </br>
-test_cbfifo.c --> Runs the test cases to make Circular buffer solid </br>

# Project Comments: </br>

1) A circular buffer implementation. You will need two circular buffer objects, one each for the transmit and receive directions. These may be </br>
allocated statically (the preferred approach), or they may be allocated dynamically at initialization time. Each circular buffer should have a  </br>
capacity of 256 bytes. </br>

2) Test code to exercise your circular buffer. You may wish to adapt the automated tests you created for Assignment 2. This code should run at </br>
startup if the DEBUG define is set in your code, in order to give you confidence that your circular buffers are solid. </br>

3) Code to configure UART0 and send and receive data over it. Parameters for this assignment are specified below. Your implementation should be </br>
fully interrupt based. The UART solution should be built atop your circular buffer implementation. </br>

4) Glue code that ties your UART communication code into the standard C library functions. After this glue code is working, a call to printf() or </br>
putchar() on the device should result in characters being sent over UART0 to the PC, and a call to getchar() should result in reading a character that </br>
the user typed on the PC. </br>

5) A command processor that can accept some very simple interactive commands (specified below) and take action on the device </br>


# Developement Details </br>
Software used Developed using MCUExpresso IDE 7.2.0 on Windows 10. </br>
DEBUG Mode - Prints DEBUG Messages to UART terminal at Baud Rate of 38400 and 8N2. </br>
To see the console messages in UART mode change to UART in quick settings and open a new Terminal with 38400 Baud Rate and 8N2 settings </br>

# OUTPUT IMAGES are attached with the files

# Extra Credit
1) Backspace functionality implemented
2) Help functionality implemented
3) Info functionality implemented





 



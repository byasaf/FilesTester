/* 
 * ErrorHandlers.h
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#ifndef _ERROR_HANDLERS_H_
#define _ERROR_HANDLERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

typedef enum _ERR_CODE{
				UNKNOWN_ARGUMENTS =			501,
				MEMORY_ALLOCATION_ERR =		502,			
				CREATE_FILE_HANDLE_ERR =	503,
				CLOSE_FILE_HANDLE_ERR =		504,
				GET_FILE_TIME_ERR =			505,
				GET_FILE_SIZE_ERR =			506,
				OPEN_FILE_ERR =				507,
				UNKNOWN_EXTENSION_ERR =		508
}ERR_CODE;

/*
 *   Description: Simple handler for printing errors 
 *	-------------------------------------------------------
 *   error_code: error-code of the occurred error 
 *   *string: string for general purpose 
 *	-------------------------------------------------------
 *   returns: the error_code 
 */
int ErrorHandler(ERR_CODE error_code, char* string);


#endif
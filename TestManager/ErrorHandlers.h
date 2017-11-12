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
				UNKNOWN_ARGUMENTS=				601,
				MEMORY_ALLOCATION_ERR=			602,		
				OPEN_FILE_ERR=					603,
				CREATE_OUTPUT_DIRECTORY_ERR=	604,
				PROCESS_BRUTAL_TERMINAION=		605,
				SUCCESS_ERR=					0
}ERR_CODE;

/*
 *   Description: Simple handler for printing errors 
 *	-------------------------------------------------------
 *   error_code: error-code of the occurred error 
 *	-------------------------------------------------------
 *   returns: ERR_CODE 
 */
ERR_CODE ErrorHandler(ERR_CODE error_code);

#endif
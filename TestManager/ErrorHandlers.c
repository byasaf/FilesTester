/* 
 * ErrorHandlers.c
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#include "ErrorHandlers.h"

ERR_CODE ErrorHandler(ERR_CODE error_code)
{
	switch(error_code)
	{
	case UNKNOWN_ARGUMENTS:
		printf("\n*** Unknown arguments ***\n");
		return(error_code);
	break;

	case MEMORY_ALLOCATION_ERR:
		printf("\nMemory allocation failed!\n");
		return(error_code);
	break;

	case OPEN_FILE_ERR:
		printf("\nError opening list file\n");
		return(error_code);
	break;
	
	case CREATE_OUTPUT_DIRECTORY_ERR:
		printf("\nCannot create output folder\n");
		exit(error_code);
	break;

	default  : 
		printf("\nUnknown error occurred, error code: 0x%x\n",error_code);
		exit(error_code);
	}
}
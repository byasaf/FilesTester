/* 
 * ErrorHandlers.c
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#include "ErrorHandlers.h"

int ErrorHandler(ERR_CODE error_code, char* string)
{
	switch(error_code)
	{
	case UNKNOWN_ARGUMENTS:
		printf("\n!! Error 0x%x: Unknown arguments !!\n",error_code);
		return(error_code);
	break;

	case MEMORY_ALLOCATION_ERR:
		printf("\n!! Error 0x%x: Memory allocation failed !!\n",error_code);
		return(error_code);
	break;

	case CREATE_FILE_HANDLE_ERR:
		//printf("\n!! Error 0x%x: Open file handle for %s failed !!\n",error_code,string);
		return(error_code);
	break;

	case CLOSE_FILE_HANDLE_ERR:
		//printf("\n!! Error 0x%x: Close file handle failed !!\n",error_code);
		return(error_code);
	break;

	case GET_FILE_TIME_ERR:
		//printf("\n!! Error 0x%x: Error get file time !!\n",error_code);
		return NULL;
	break;

	case GET_FILE_SIZE_ERR:
		//printf("\n!! Error 0x%x: Error get file size !!\n",error_code);
		return(error_code);
	break;

	case OPEN_FILE_ERR:
		//printf("\n!! Error 0x%x: Open file %s failed !!\n",error_code,string);
		return NULL;
	break;

	case UNKNOWN_EXTENSION_ERR:
		//printf("\n!! Error 0x%x: nUnknown extension for %s !!\n",error_code,string);
		return(error_code);
	break;

	default  : 
		printf("\n!! Unknown error occurred, error code: 0x%x\n !!",error_code);
		return(error_code);
	}
}
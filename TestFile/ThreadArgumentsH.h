/* 
 * ThreadArgumentsH.h
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#ifndef _THREAD_ARGUMENTS_H_
#define _THREAD_ARGUMENTS_H_

/*
 *   Description: struct holding the arguments the been sent to the threads
 *	-------------------------------------------------------
 *  file_path: pointer to chars string holding the tested file path
 *  result: pointer to chars string holding the test result
 */
typedef struct _ThreadArguments{
	char* file_path;
	char* result;
}ThreadArgument;

#endif
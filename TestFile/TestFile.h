/* 
 * TestFile.h
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#ifndef _TEST_FILE_H_
#define _TEST_FILE_H_

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#include "ErrorHandlers.h"
#include "Threads.h"
#include "ThreadArgumentsH.h"
#include "StringConversionTools.h"

#define OUTPUT_SUFFIX		"_sol"
#define NUM_OF_TESTS		5	
#define DEFUALT_SLEEP_TIME	10
#define TIME_FORMAT_LENGTH	25
#define SIZE_STRING_LENGTH	10
#define NUM_OF_BYTES		5

enum time_type {CREATED, MODIFIED};

/*
 *   Description: Get Created or Modified time of the specified file
 *	used by GetFileCreationTimeRoutine and GetFileLastModifiedTimeRoutine
 *	-------------------------------------------------------
 *   *file_path: file path string for the requested file
 *   type: type of time-CREATED / MODIFIED
 *	-------------------------------------------------------
 *   returns: time string
 */
char* GetFileTimeString(char* file_path,enum time_type type);

/*
 *   Description: helper function used by GetFileTimeString
 *	 Converts FILETIME to chars string
 *	-------------------------------------------------------
 *   ft: FILETIME to convert
 *   time_string: string for holding the result
 *	-------------------------------------------------------
 *   returns: pointer for the time string holding he result
 */
char* FileTimeToString(FILETIME ft,char* time_string);

/*
 *   Description: helper function used by GetFileSizeRoutine
 *	 Converts DWORD file size to readable chars string
 *	-------------------------------------------------------
 *   file_size: DWORD size to convert
 *   size_string: string for holding the result
 *	-------------------------------------------------------
 *   returns: pointer for the time string holding he result
 */
char* FileSizeToString(DWORD file_size,char* size_string);

/*
 *   Description: Create file handler for win API file functions
 *	-------------------------------------------------------
 *   file_path: chars string holding the file path
 *	-------------------------------------------------------
 *   returns: pointer to HANDLE objects holding the file
 */
HANDLE *CreateFileHandle(char* file_path);

/*
 *   Description: Close file handler
 *	-------------------------------------------------------
 *   *file_handle: pointer to HANDLE object holding the file
 *	-------------------------------------------------------
 *   returns: TRUE- close succeeded FALSE if not
 */
BOOL CloseFileHandle(HANDLE *file_handle);

/*
 *   Description: Logger- printing the results into file
 *	-------------------------------------------------------
 *   *file_name: chars string holding the tested filename
 *   *log_path: chars string holding the log path
 *	 threads_arguments[]: array holding the results of each test thread
 *   threads_exitcodes[]; array holding the exitcoed of each test thread
 *	-------------------------------------------------------
 *   returns: TRUE- close succeeded FALSE if not
 */
void LogTestsResults(char* file_name, char* log_path, ThreadArgument threads_arguments[],DWORD threads_exitcodes[]);

#endif 
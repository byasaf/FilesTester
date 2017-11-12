/* 
 * Threads.h
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#ifndef _TEST_ROUTINES_H_
#define _TEST_ROUTINES_H_

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#include "ErrorHandlers.h"
#include "TestFile.h"
#include "ThreadArgumentsH.h"

/*
 *   Description: Launches all tests threads and wait
 *	-------------------------------------------------------
 *   *file_path: chars string holding the tested file path
 *   *log_path: chars string holding the log path
 *	-------------------------------------------------------
 *   returns: last exitcode recieved from the secondary threads
 *		the string result is been saved into thread_arguments->result
 */
int LaunchThreadsAndWait(char* file_path, char* log_path);

/*
 *   Description: Thread routine: Get file extesion
 *	-------------------------------------------------------
 *   *thread_arguments: pointer for ThreadArgument objects 
 *         holding the arguments been sent to the routine
 *	-------------------------------------------------------
 *   returns: exitcode recieved of this secondary thread
 *		the string result is been saved into thread_arguments->result
 */
int GetFileExtesionRoutine(ThreadArgument* thread_arguments);

/*
 *   Description: Thread routine: Get file size
 *	-------------------------------------------------------
 *   *thread_arguments: pointer for ThreadArgument objects 
 *         holding the arguments been sent to the routine
 *	-------------------------------------------------------
 *   returns: exitcode recieved of this secondary thread
 *		the string result is been saved into thread_arguments->result
 */
int GetFileSizeRoutine(ThreadArgument* thread_argumenst);

/*
 *   Description: Thread routine: Get File Creation Time
 *	-------------------------------------------------------
 *   *thread_arguments: pointer for ThreadArgument objects 
 *         holding the arguments been sent to the routine
 *	-------------------------------------------------------
 *   returns: exitcode recieved of this secondary thread
 *		the string result is been saved into thread_arguments->result
 */
int GetFileCreationTimeRoutine(ThreadArgument* thread_arguments);

/*
 *   Description: Thread routine: Get File Last Modified Time
 *	-------------------------------------------------------
 *   *thread_arguments: pointer for ThreadArgument objects 
 *         holding the arguments been sent to the routine
 *	-------------------------------------------------------
 *   returns: exitcode recieved of this secondary thread
 *		the string result is been saved into thread_arguments->result
 */
int GetFileLastModifiedTimeRoutine(ThreadArgument* thread_arguments);

/*
 *   Description: Thread routine: Get File First Bytes
 *	-------------------------------------------------------
 *   *thread_arguments: pointer for ThreadArgument objects 
 *         holding the arguments been sent to the routine
 *	-------------------------------------------------------
 *   returns: exitcode recieved of this secondary thread
 *		the string result is been saved into thread_arguments->result
 */
int GetFileFirstBytesRoutine(ThreadArgument* thread_arguments);

#endif
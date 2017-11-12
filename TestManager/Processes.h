/* 
 * Processes.h
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#ifndef _PROCESSES_H_
#define _PROCESSES_H_

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#include "ErrorHandlers.h"
#include "StringConversionTools.h"

#define TIMEOUT					360000UL
#define	PROCESS_TIME_STR_LEN	100


typedef struct _TestsProcess
{
	char*						file_name;
	char*						log_path;
	char*						IpCommandLineChars;
	PROCESS_INFORMATION			IpProcessInformation;
	DWORD						last_exit_code;
	BOOL						create_process_succeeded;
} TestsProcess;

/*
 *   Description: Launch single process for testing
 *	-------------------------------------------------------
 *   *test_process: struct object contain details of the proccess 
 *	-------------------------------------------------------
 *   returns: 0- Launch succeed, 1- Launch failed
 */
BOOL LaunchSingleProcess(TestsProcess *test_process);

/*
 *   Description: Launch all processes from array
 *	-------------------------------------------------------
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_processes:	num of proccess returned from list file
 *	-------------------------------------------------------
 *   returns: none
 */
void LaunchAllProcesses(TestsProcess tests_processes[],int num_of_files);

/*
 *   Description: Log processes initial and runtime status 
 *	-------------------------------------------------------
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_processes:	num of proccess returned from list file
 *   *log_path: string for log location
 *   DWORD status_check_frequency: frequency of status reporting (in ms)
 *	-------------------------------------------------------
 *   returns: ERR_CODE of the termination (brutal or not)
 */
ERR_CODE LogProcessesStatus(TestsProcess tests_processes[],int num_of_files,char* log_path, DWORD status_check_frequency);

/*
 *   Description: Sort processes by ID
 *	-------------------------------------------------------
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_processes:	num of proccess returned from list file
 *	-------------------------------------------------------
 *   returns: array of indexes pointing to locations in tests_processes[]
 *            in IDs ascending order.
 */
int* SortProcessesById(TestsProcess tests_processes[],int num_of_processes);

/*
 *   Description: Get process time
 *	-------------------------------------------------------
 *   hProc:	handle for the required process
 *   *time:	string for time
 *	-------------------------------------------------------
 *   returns: pointer to chars array- string of time
 */
char* GetProcessTime(HANDLE hProc, char *time);

/*
 *   Description: Brutally terminate all process
 *	-------------------------------------------------------
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_processes:	num of proccess returned from list file
 *	-------------------------------------------------------
 *   returns: none
 */
void TerminateAllProcess(TestsProcess *tests_processes,int num_of_processes);

/*
 *   Description: Helper function for GetProcessTime
 *				  Subtract two Filetimes ti return run time
 *	-------------------------------------------------------
 *   FILETIME Late:		later time struct object
 *   FILETIME Early:	early time struct object
 *	-------------------------------------------------------
 *   returns: FILETIME Late-Early
 */
FILETIME SubtractFiletimes(FILETIME Late, FILETIME Early);

#endif
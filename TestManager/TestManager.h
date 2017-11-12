/* 
 * TestManager.h
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#ifndef _TEST_MANAGER_H_
#define _TEST_MANAGER_H_

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

#include "Processes.h"
#include "ErrorHandlers.h"
#include "StringConversionTools.h"

#define TESTING_TOOL				"TestFile.exe"
#define OUTPUT_SUFFIX				"_log"
#define LOG_EXTENSION				".txt"
#define MANAGER_LOG_FILENAME		"runtime_logfile.txt"

/*
 *   Description: Inizialize the TestsProcesses array
 *				   by generating LogPath and CommandLine for each process
 *	-------------------------------------------------------
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_processes:	num of proccess returned from list file
 *   *output_directory: string of the output files directory name
 *	-------------------------------------------------------
 *   returns: SUCESS_ERR if initialize succeeded or another ERR_CODE if not
 */
ERR_CODE InizializeTestsProcessesArray(TestsProcess tests_processes[],int num_of_files,char* output_directory);

/*
 *   Description: Extracts files list from file to TestsProcess array
 *	-------------------------------------------------------
 *	 *file_name: string of the list filename
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_files_pointer:	pointer for int vairable to hold the num of proccess returned from list file
 *	-------------------------------------------------------
 *   returns: pointer to array of TestsProcess in the size specified in *num_of_files_pointer
 */
TestsProcess* GetFilesListFromFile(char* file_name,TestsProcess tests_processes[],int* num_of_files_pointer);

/*
 *   Description: Check if the output directory exits, create it if not
 *	-------------------------------------------------------
 *	 *output_directory: string of the output directory 
 *	-------------------------------------------------------
 *   returns: TRUE- directory is ready to use FALSE- if not
 */
BOOL CreateOutputDirectory(char* output_directory);

/*
 *   Description: Generate LogPath for each test, used by InizializeTestsProcessesArray
 *	-------------------------------------------------------
 *	 *tests_process: pointer to TestsProcess object conatian the process details
 *   output_directory: string of the output directory 
 *	-------------------------------------------------------
 *   returns: SUCESS_ERR if initialize succeeded or another ERR_CODE if not
 */
ERR_CODE GenerateLogPath(TestsProcess *tests_process, char *output_directory);

/*
 *   Description: Generate the runtime logfile path for TestManager
 *	-------------------------------------------------------
 *   output_directory: string of the output directory 
 *	-------------------------------------------------------
 *   returns: char* string holding the runtime logfile path
 */
char* GenerateRuntimeLogfilePath(char *output_directory);

/*
 *   Description: Generate CommandLine for each test, used by InizializeTestsProcessesArray
 *	-------------------------------------------------------
 *	 *tests_process: pointer to TestsProcess object conatian the process details
 *	-------------------------------------------------------
 *   returns: SUCESS_ERR if initialize succeeded or another ERR_CODE if not
 */
ERR_CODE GenerateCommandLine(TestsProcess *tests_process);

/*
 *   Description: Free all the memory which was allocated for the tests_processes array
 *				  also close the proccesses handlers
 *	-------------------------------------------------------
 *   tests_processes[]:	 array contains the proccesses details
 *   num_of_processes:	num of proccess returned from list file
 *	-------------------------------------------------------
 *   returns: none
 */
void FreeTestProcessArray(TestsProcess *tests_process, int num_of_files);

#endif
/* 
 * Processes.c
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#include "Processes.h"

BOOL LaunchSingleProcess(TestsProcess *tests_process)
{
    STARTUPINFO				si;
	LPTSTR					IpCommandLine=ConvertCharStringToLPTSTR(tests_process->IpCommandLineChars);

	ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);

	tests_process->create_process_succeeded= CreateProcess(	NULL,
															IpCommandLine,
															NULL,
															NULL,
															FALSE,
															NORMAL_PRIORITY_CLASS,
															NULL,
															NULL,
															&si,
															&tests_process->IpProcessInformation);
	free(IpCommandLine);
	return tests_process->create_process_succeeded;
}
void LaunchAllProcesses(TestsProcess tests_processes[],int num_of_files)
{
	int i;
	for(i=0;i<num_of_files;i++)
	{
		tests_processes[i].create_process_succeeded=LaunchSingleProcess(&tests_processes[i]);
	}
}
ERR_CODE LogProcessesStatus(TestsProcess tests_processes[],int num_of_files,char* log_path, DWORD status_check_frequency)
{
	int			unsorted_i,i;
	int*		sorted_index;
	int			iterations=TIMEOUT/status_check_frequency;
	BOOL		active_processes=FALSE,
				finished_processes=FALSE,
				all_done=FALSE;
	BOOL*		error_flags=NULL;
	char*		time=NULL;
	FILE		*log_file=fopen(log_path,"w");
	if(!log_file)
	{
		ErrorHandler(OPEN_FILE_ERR);
	}

//===================logging initial processes state===================
	for(i=0;i<num_of_files;i++)
	switch(tests_processes[i].create_process_succeeded)
	{
	case 0:
		printf("!!! Failed to create new process to run \"%s\". Error code: 0x%x !!!\n",
		tests_processes[i].IpCommandLineChars,
		tests_processes[i].create_process_succeeded);

		fprintf(log_file,"!!! Failed to create new process to run \"%s\". Error code: 0x%x !!!\n",
		tests_processes[i].IpCommandLineChars,
		tests_processes[i].create_process_succeeded);
	break;
	default:
		//printf("Successfully created a process with ID %d to execute \"%s\"\n",
		//tests_processes[i].IpProcessInformation.dwProcessId,
		//tests_processes[i].IpCommandLineChars);

		fprintf(log_file,"Successfully created a process with ID %d to execute \"%s\"\n",
		tests_processes[i].IpProcessInformation.dwProcessId,
		tests_processes[i].IpCommandLineChars);
	}

	//allocate memory for flags to report about processes ended with error, and time string;
	error_flags=(BOOL*)malloc(num_of_files*sizeof(BOOL));
	time=(char*)malloc(PROCESS_TIME_STR_LEN*sizeof(char));
	if((!error_flags)||(!time))
	{
		ErrorHandler(MEMORY_ALLOCATION_ERR);
	}
	for(i=0;i<num_of_files;i++)
	{
		error_flags[i]=FALSE;
	}

	//generate sorted indexes array
	sorted_index=SortProcessesById(tests_processes,num_of_files);

	//===========loop checking the procceses status until all processes are done or timeout reached=========
	while(FALSE==all_done && iterations--)
	{
		all_done=TRUE;
		for(unsorted_i=0;unsorted_i<num_of_files;unsorted_i++)
		{
			i=sorted_index[unsorted_i];//i is index uses to scan tests_processes array in ascending order using sorted_index array
			tests_processes[i].create_process_succeeded=GetExitCodeProcess(tests_processes[i].IpProcessInformation.hProcess,
								&tests_processes[i].last_exit_code);
			if(tests_processes[i].create_process_succeeded=TRUE)//only if it is a legitimate exitcode
			{
				if(STILL_ACTIVE==tests_processes[i].last_exit_code)
				{
						active_processes=TRUE;
						all_done=all_done & FALSE;
				}
				if(STILL_ACTIVE!=tests_processes[i].last_exit_code) //logging errors immediately
				{
					if(ERROR_SUCCESS!=tests_processes[i].last_exit_code &&
						FALSE==error_flags[i])
					{
						printf("\n!! Error : Process %d (%s) - has ended with error 0x%x\nOne or more tests were failed. Please see log for more information !!\n",
							tests_processes[i].IpProcessInformation.dwProcessId,
							tests_processes[i].file_name,
							tests_processes[i].last_exit_code);
						fprintf(log_file,"\n!! Error : Process %d (%s) - has ended with error 0x%x\nOne or more tests were failed. Please see log for more information !!\n",
							tests_processes[i].IpProcessInformation.dwProcessId,
							tests_processes[i].file_name,
							tests_processes[i].last_exit_code);
						error_flags[i]=TRUE;
					}
					finished_processes=TRUE; 
					all_done=all_done & TRUE;
				}

			}

		}

	//===================printing the active proccess===================
		if(TRUE==active_processes)
		{
			//printf("\nList of running processes:\n");
			fprintf(log_file,"\nList of running processes:\n");
			active_processes=FALSE;
		

			for(unsorted_i=0;unsorted_i<num_of_files;unsorted_i++) 
			{
				i=sorted_index[unsorted_i];
				time=GetProcessTime(tests_processes[i].IpProcessInformation.hProcess,time);
				if(tests_processes[i].create_process_succeeded &&
					STILL_ACTIVE==tests_processes[i].last_exit_code)
					{
						//printf("Process %d is running command %s for %s\n\n",	tests_processes[i].IpProcessInformation.dwProcessId,
						//												tests_processes[i].IpCommandLineChars,time);
						fprintf(log_file,"Process %d is running command %s for %s\n",	tests_processes[i].IpProcessInformation.dwProcessId,
																						tests_processes[i].IpCommandLineChars,time);
					}
			}
		}
	//===================printing the finished proccess===================
		if(TRUE==finished_processes)
		{
			//printf("\nList of finished processes:\n");
			fprintf(log_file,"\nList of finished processes:\n");
			finished_processes=FALSE;
		

			for(unsorted_i=0;unsorted_i<num_of_files;unsorted_i++) 
			{
				i=sorted_index[unsorted_i];
				time=GetProcessTime(tests_processes[i].IpProcessInformation.hProcess,time);
				if(tests_processes[i].create_process_succeeded &&
					STILL_ACTIVE!=tests_processes[i].last_exit_code)
					{
						//printf("Process %d ran command %s and exited with exit code 0x%x...\n",
						//	tests_processes[i].IpProcessInformation.dwProcessId,
						//	tests_processes[i].IpCommandLineChars,
						//	tests_processes[i].last_exit_code,time);
						fprintf(log_file,"Process %d ran command %s and exited with exit code 0x%x after %s\n",
							tests_processes[i].IpProcessInformation.dwProcessId,
							tests_processes[i].IpCommandLineChars,
							tests_processes[i].last_exit_code,time);
					}
			}
		}
		
		Sleep(status_check_frequency);
	}
	if(FALSE==all_done)
	{ //brutally terminate proccess if timeout has reached before all processes have finished
		TerminateAllProcess(tests_processes,num_of_files);
	}
	printf("\nAll the processes have finished. Exiting program.\n");
	fprintf(log_file,"\nAll the processes have finished. Exiting program.\n");
	//safely return
	free(error_flags);
	free(time);
	free(sorted_index);
	fclose(log_file);
	return all_done?SUCCESS_ERR:PROCESS_BRUTAL_TERMINAION;
}
int* SortProcessesById(TestsProcess *tests_process,int num_of_processes)
{
	int		i=0,
			j=0,
			k=0;
	int*	sorted_index=NULL;

	sorted_index=(int*)malloc(num_of_processes*sizeof(int));
	if(sorted_index==NULL)
	{
		ErrorHandler(MEMORY_ALLOCATION_ERR);
	}

	for(i=1, k=0; i<num_of_processes; i++)
	{
		if(tests_process[i].IpProcessInformation.dwProcessId>tests_process[k].IpProcessInformation.dwProcessId)
		{
			k=i;
		}
	}
	for(i=0; i<num_of_processes; i++)
	{
		sorted_index[i]=k;
		for(j=0; j<num_of_processes; j++)
		{

			if(i==0)
			{
				if((tests_process[j].IpProcessInformation.dwProcessId < tests_process[sorted_index[i]].IpProcessInformation.dwProcessId))
				{
					sorted_index[i]=j;
				}
			}
			else
			{
				if((tests_process[j].IpProcessInformation.dwProcessId < tests_process[sorted_index[i]].IpProcessInformation.dwProcessId)&
					(tests_process[j].IpProcessInformation.dwProcessId > tests_process[sorted_index[i-1]].IpProcessInformation.dwProcessId ))
				{
					sorted_index[i]=j;
				}
			}
		}
	}	
	return(sorted_index);
}
char* GetProcessTime(HANDLE hProc, char *time)
{
	FILETIME	ftCreate, 
				ftExit, 
				ftKernel, 
				ftUser, 
				TotalTime;
	SYSTEMTIME	stUTC, stLocal;

	// Retrieve the file times for the file.
	GetProcessTimes(hProc, &ftCreate, &ftExit, &ftKernel, &ftUser);

	FileTimeToSystemTime(&ftUser, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	TotalTime = SubtractFiletimes(ftExit, ftCreate);
	FileTimeToSystemTime(&TotalTime, &stUTC);  
	
	sprintf(time, "%d seconds and %d milliseconds.", stUTC.wSecond, stUTC.wMilliseconds);

	return time;
}
void TerminateAllProcess(TestsProcess *tests_processes,int num_of_processes)
{
	int i;
	for(i=0;i<num_of_processes;i++)
	{
		TerminateProcess(tests_processes[i].IpProcessInformation.hProcess, PROCESS_BRUTAL_TERMINAION);
	}
}
FILETIME SubtractFiletimes(FILETIME Late, FILETIME Early)
{
	typedef unsigned __int64 Unsigned64BitType;
	Unsigned64BitType Late64BitVal;
	Unsigned64BitType Early64BitVal;
	Unsigned64BitType Difference64BitVal;
	FILETIME DifferenceAsFILETIME;
	const Unsigned64BitType Low32BitsMask = 0x00000000FFFFFFFF;

	Late64BitVal = ((Unsigned64BitType)(Late.dwHighDateTime) << 32) +
		Late.dwLowDateTime;
	Early64BitVal = ((Unsigned64BitType)(Early.dwHighDateTime) << 32) +
		Early.dwLowDateTime;
	Difference64BitVal = Late64BitVal - Early64BitVal;

	DifferenceAsFILETIME.dwLowDateTime =
		(DWORD)(Difference64BitVal & Low32BitsMask);
	DifferenceAsFILETIME.dwHighDateTime =
		(DWORD)(Difference64BitVal >> 32);

	return DifferenceAsFILETIME;
}
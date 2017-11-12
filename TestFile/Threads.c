/* 
 * Threads.c
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#include "Threads.h"

int LaunchThreadsAndWait(char* file_path, char* log_path)
{
	HANDLE						threads_handles[NUM_OF_TESTS];
	LPTHREAD_START_ROUTINE		threads_routines[NUM_OF_TESTS];
	DWORD						threads_IDs[NUM_OF_TESTS];
	DWORD						threads_exitcodes[NUM_OF_TESTS];
	ThreadArgument				threads_arguments[NUM_OF_TESTS];
	int							i;
	int							retVal=0;

	//put the routines into array for easier managments
	threads_routines[0]=(LPTHREAD_START_ROUTINE)GetFileExtesionRoutine;
	threads_routines[1]=(LPTHREAD_START_ROUTINE)GetFileSizeRoutine;
	threads_routines[2]=(LPTHREAD_START_ROUTINE)GetFileCreationTimeRoutine;
	threads_routines[3]=(LPTHREAD_START_ROUTINE)GetFileLastModifiedTimeRoutine;
	threads_routines[4]=(LPTHREAD_START_ROUTINE)GetFileFirstBytesRoutine;
	
	//launch the threads
	for(i=0;i<NUM_OF_TESTS;i++)
	{
		threads_arguments[i].file_path=file_path;
		threads_handles[i]=CreateThread(NULL, 
										0, 
										threads_routines[i],
										&threads_arguments[i],
										0,
										&threads_IDs[i]);
	}

	WaitForMultipleObjects(NUM_OF_TESTS,threads_handles,TRUE,INFINITE);

	//retrive exitcodes from all threads
	for(i=0;i<NUM_OF_TESTS;i++)
	{
		GetExitCodeThread(threads_handles[i],&threads_exitcodes[i]);
		if(ERROR_SUCCESS!=threads_exitcodes[i])
		{
			retVal=threads_exitcodes[i];
		}
	}

	LogTestsResults(file_path,log_path,threads_arguments,threads_exitcodes);

 //close handles and free the memory which was allocated for the threads strings results
	for(i=0;i<NUM_OF_TESTS;i++)
	{
		CloseHandle(threads_handles[i]);
		if(NULL!=threads_arguments[i].result)
		{
			free(threads_arguments[i].result);
		}
	}

	return retVal;
}
int GetFileExtesionRoutine(ThreadArgument* thread_arguments)
{
	char *dot = NULL;
	char *extension = NULL;

	Sleep(DEFUALT_SLEEP_TIME);

	dot=strrchr(thread_arguments->file_path, '.');

    if(!dot || dot == thread_arguments->file_path)
	{
		thread_arguments->result=NULL;
		return ErrorHandler(UNKNOWN_EXTENSION_ERR,thread_arguments->file_path);
	}
	extension=(char*)malloc(strlen(dot)+1);
	if(!extension) 
	{
		thread_arguments->result=NULL;
		return ErrorHandler(MEMORY_ALLOCATION_ERR,NULL);
	}
	strcpy(extension,dot);
	thread_arguments->result=extension;

    return ERROR_SUCCESS;
}
int GetFileSizeRoutine(ThreadArgument* thread_arguments)
{
	HANDLE			*file_handle=NULL;
	DWORD			file_size;
	char			*size_string=NULL;

	Sleep(DEFUALT_SLEEP_TIME);

	file_handle=CreateFileHandle(thread_arguments->file_path);
	if(*file_handle == INVALID_HANDLE_VALUE)
	{
		thread_arguments->result=NULL;
		return ErrorHandler(CREATE_FILE_HANDLE_ERR,thread_arguments->file_path);
	}

	size_string=(char*)malloc(SIZE_STRING_LENGTH*sizeof(char));
	if(!size_string) 
	{ //safely return
		CloseFileHandle(file_handle);
		thread_arguments->result=NULL;
		return ErrorHandler(MEMORY_ALLOCATION_ERR,NULL);
	}

	if(!(file_size=GetFileSize(*file_handle,NULL)))
	{ //safely return
		free(size_string); 
		CloseFileHandle(file_handle);
		thread_arguments->result=NULL;
		return ErrorHandler(GET_FILE_SIZE_ERR,NULL);
	}
	CloseFileHandle(file_handle);

	thread_arguments->result=FileSizeToString(file_size,size_string);
	return ERROR_SUCCESS;
}
int GetFileCreationTimeRoutine(ThreadArgument* thread_arguments)
{
	Sleep(DEFUALT_SLEEP_TIME);
	thread_arguments->result=GetFileTimeString(thread_arguments->file_path,CREATED);
	return thread_arguments->result==NULL?GET_FILE_TIME_ERR:ERROR_SUCCESS;
}
int GetFileLastModifiedTimeRoutine(ThreadArgument* thread_arguments)
{
	Sleep(DEFUALT_SLEEP_TIME);
	thread_arguments->result=GetFileTimeString(thread_arguments->file_path,MODIFIED);
	return thread_arguments->result==NULL?GET_FILE_TIME_ERR:ERROR_SUCCESS;
}
int GetFileFirstBytesRoutine(ThreadArgument* thread_arguments)
{
	FILE			*input_file=NULL;
	char			*first_bytes=NULL;

	Sleep(DEFUALT_SLEEP_TIME);

	input_file = fopen(thread_arguments->file_path,"r");
	if(!input_file)
	{ //safely return
		thread_arguments->result=NULL;
		ErrorHandler(OPEN_FILE_ERR,thread_arguments->file_path);
		return OPEN_FILE_ERR;
	}

	first_bytes=(char*)malloc(NUM_OF_BYTES+1); //the number of bytes can be changed using the #define in the .h file
	if(!first_bytes) 
	{ //safely return
		fclose(input_file); 
		thread_arguments->result=NULL;
		return ErrorHandler(MEMORY_ALLOCATION_ERR,NULL);
	}
	
	first_bytes[fread(first_bytes,1,NUM_OF_BYTES,input_file)]='\0';
	fclose(input_file);
	thread_arguments->result=first_bytes;
	return ERROR_SUCCESS;
}

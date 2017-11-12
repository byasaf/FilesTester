/* 
 * TestFile.c
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 
#include "TestFile.h"

int main(int argc, char **argv)
{

	if(argc!=3) return ErrorHandler(UNKNOWN_ARGUMENTS,NULL);

	return LaunchThreadsAndWait(argv[1],argv[2]);
}
char* GetFileTimeString(char* file_path,enum time_type type)
{
	
	HANDLE			*file_handle=CreateFileHandle(file_path);
	FILETIME		ft;
	char*			time_string=NULL;

	if(*file_handle == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	time_string=(char*)malloc(TIME_FORMAT_LENGTH*sizeof(char));
	if(!time_string) 
	{ //safely return
		CloseFileHandle(file_handle);
		ErrorHandler(MEMORY_ALLOCATION_ERR,NULL);
	}
	switch(type) //choosing between created or modified time
	{	
	case CREATED:
		if(!GetFileTime(*file_handle,&ft,NULL,NULL))
		{ //safely return
			free(time_string);
			CloseFileHandle(file_handle);
			ErrorHandler(GET_FILE_TIME_ERR,NULL);
			return NULL;
		}
	break;

	case MODIFIED:
		if(!GetFileTime(*file_handle,NULL,NULL,&ft))
		{ //safely return
			free(time_string);
			CloseFileHandle(file_handle);
			ErrorHandler(GET_FILE_TIME_ERR,NULL);
			return NULL;
		}
	break;
	}

	CloseFileHandle(file_handle);
	FileTimeToString(ft,time_string);

	return time_string;
}
char* FileTimeToString(FILETIME ft,char* time_string)
{
	SYSTEMTIME st_utc,st_local;

	GetSystemTime(&st_utc);
	FileTimeToSystemTime(&ft,&st_utc);
	SystemTimeToTzSpecificLocalTime(NULL,&st_utc,&st_local);
	
	sprintf(time_string, "%02u/%02u/%04u, %02u:%02u:%02u",	st_local.wDay,st_local.wMonth, st_local.wYear,	
															st_local.wHour, st_local.wMinute, st_local.wSecond);
	return time_string;
}
char* FileSizeToString(DWORD file_size,char* size_string)
{
	char *SIZES[] = { "B", "kB", "MB", "GB", "TB"};
    DWORD div = 0;
    DWORD rem = 0;

    while (file_size >= 1024 && div < (sizeof SIZES / sizeof *SIZES))
	{
        rem = (file_size % 1024);
        div++;   
        file_size /= 1024;
    }
	sprintf(size_string, "%.2f%s", (float)file_size + (float)rem / 1024.0, SIZES[div]);
	return size_string;
}
HANDLE *CreateFileHandle(char* file_path)
{
	HANDLE		*file_handle;
	LPTSTR		file_path_LPTSTR;
	file_handle=(HANDLE*)malloc(sizeof(HANDLE));
	if(!file_handle) 
	{
		ErrorHandler(MEMORY_ALLOCATION_ERR,file_path);
	}
	file_path_LPTSTR=ConvertCharStringToLPTSTR(file_path);
	*file_handle = CreateFile(	file_path_LPTSTR,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	free(file_path_LPTSTR);
	return file_handle;
	}
BOOL CloseFileHandle(HANDLE *file_handle)
{
	BOOL ret;
	if(!(ret=CloseHandle(*file_handle)))
		ErrorHandler(CLOSE_FILE_HANDLE_ERR,NULL);
	free(file_handle);
	return ret;
}
void LogTestsResults(char* file_name, char* log_path, ThreadArgument threads_arguments[],DWORD threads_exitcodes[])
{
	FILE *log_file=fopen(log_path,"w");

	if(!log_file)
	{
		ErrorHandler(OPEN_FILE_ERR,log_path);
	}
	//Logging results to file according to exitcodes
	if((OPEN_FILE_ERR == threads_exitcodes[4]) && 
		(CREATE_FILE_HANDLE_ERR == threads_exitcodes[1]) &&
		(GET_FILE_TIME_ERR ==  threads_exitcodes[2]) &&
		(GET_FILE_TIME_ERR ==  threads_exitcodes[3]))
		{fprintf(log_file,"!! Could not open file: %s !!\n",								file_name);}
	else
		{fprintf(log_file,"%s\n",															file_name);}

	if(ERROR_SUCCESS==threads_exitcodes[0])
		{fprintf(log_file,"The file extension of the tested file is \"%s\"\n",				threads_arguments[0].result);}
	else
		{fprintf(log_file,"!! Error 0x%x: Cannot retrieve file extension !!\n",				threads_exitcodes[0]);}

	if(ERROR_SUCCESS==threads_exitcodes[1])
		{fprintf(log_file,"The test file size is %s\n",										threads_arguments[1].result);}
	else
		{fprintf(log_file,"!! Error 0x%x: Cannot retrieve file size !!\n",					threads_exitcodes[1]);}

	if(ERROR_SUCCESS==threads_exitcodes[2])
		{fprintf(log_file,"The file was created on %s\n",									threads_arguments[2].result);}
	else
		{fprintf(log_file,"!! Error 0x%x: Cannot retrieve file's creation time!! \n",		threads_exitcodes[2]);}

	if(ERROR_SUCCESS==threads_exitcodes[3])
		{fprintf(log_file,"The file was last modified on %s\n",								threads_arguments[3].result);}
	else
		{fprintf(log_file,"!! Error 0x%x: Cannot retrieve file's last modified time !!\n",	threads_exitcodes[3]);}

	if(ERROR_SUCCESS==threads_exitcodes[4])
		{fprintf(log_file,"The file's first %d bytes are: %s\n",NUM_OF_BYTES,				threads_arguments[4].result);}
	else
		{fprintf(log_file,"!! Error 0x%x: Cannot retrieve file's first %d bytes !!\n",		threads_exitcodes[4],NUM_OF_BYTES);}


	fclose(log_file);
}
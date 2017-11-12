/* 
 * TestManager.c
 * ===========================
 * Exercise 2
 * ===========================
 * Name: Assaf Ben Yishay
 * ===========================
 */ 

#include "TestManager.h"

int main(int argc, char **argv)
{
	TestsProcess*	tests_processes=NULL;
	char*			files_to_test=NULL;
	char*			output_directory=NULL;
	char*			runtime_logfile=NULL;
	DWORD			status_check_frequency;
	int				num_of_files;
	ERR_CODE		retVal=SUCCESS_ERR;
					

	if(argc!=4) return ErrorHandler(UNKNOWN_ARGUMENTS);

	//initialize variables according to argv arguments
	files_to_test			=argv[1];
	output_directory		=argv[2];
	status_check_frequency	=atoi(argv[3]);
	tests_processes			=GetFilesListFromFile(files_to_test,tests_processes,&num_of_files);
	runtime_logfile			=GenerateRuntimeLogfilePath(output_directory);

	//making sure output directory is ready to use
	if(!CreateOutputDirectory(output_directory))
	{
		ErrorHandler(CREATE_OUTPUT_DIRECTORY_ERR);
	}

	//Inizialize TestsProcesses array
	retVal=InizializeTestsProcessesArray(tests_processes,num_of_files,output_directory);
	if(MEMORY_ALLOCATION_ERR==retVal)
	{
		FreeTestProcessArray(tests_processes,num_of_files);
		return retVal;
	}

	LaunchAllProcesses(tests_processes,num_of_files);
	retVal=LogProcessesStatus(tests_processes,num_of_files,runtime_logfile,status_check_frequency);

	//safely closing the program
	free(runtime_logfile);
	FreeTestProcessArray(tests_processes,num_of_files);

	return retVal;
}
ERR_CODE InizializeTestsProcessesArray(TestsProcess tests_processes[],int num_of_files,char* output_directory)
{
	int i;
	for(i=0;i<num_of_files;i++)
	{
		//generate output path
		if(MEMORY_ALLOCATION_ERR==GenerateLogPath(&tests_processes[i],output_directory))
		{
			return MEMORY_ALLOCATION_ERR;
		}
		//generate command line
		if(MEMORY_ALLOCATION_ERR==GenerateCommandLine(&tests_processes[i]))
		{
			return MEMORY_ALLOCATION_ERR;
		}

	}
}
TestsProcess* GetFilesListFromFile(char* file_name,TestsProcess tests_processes[],int* num_of_files_pointer)
{
	FILE			*input_file=NULL;
	char*			temp_str=NULL;
	char			c='.';
	int				i=0,j,k,
					num_of_files=0,
					max_filename_length=0;


	input_file = fopen(file_name,"r");
	if(!input_file)
	{
		ErrorHandler(OPEN_FILE_ERR);
		return NULL;
	}

	//counting the file number (lines number) and finding the max length string for later allocating memory
	while(EOF!=c)
	{
		c=fgetc(input_file);
		i++;
		if('\n'==c||EOF==c)
		{
			if(i>1) num_of_files++;
			if(i>max_filename_length)
			{
				max_filename_length=i;
			}
			i=0;
		}
	};

	max_filename_length++;
	rewind(input_file);

	tests_processes=(TestsProcess*)malloc(num_of_files*sizeof(TestsProcess));
	if( NULL==tests_processes)
	{
		free(temp_str);
		fclose(input_file);
		ErrorHandler(MEMORY_ALLOCATION_ERR);
		return NULL;
	}
	temp_str=(char*)malloc(max_filename_length);
	if(NULL==temp_str)
	{
		free(temp_str);
		free(tests_processes);
		fclose(input_file);
		ErrorHandler(MEMORY_ALLOCATION_ERR);
		return NULL;
	}

	i=0;
	j=0;
	c='.';
	while(EOF!=c)
	{
		c=fgetc(input_file);
		if(c!='\n' && c!=EOF)
		{
			temp_str[i]=c;
			i++;
		}
		else
		{
			temp_str[i]='\0';
			if(i>0)
			{
				tests_processes[j].file_name=(char*)malloc(i+1);
				if(NULL==tests_processes[j].file_name)
				{
					fclose(input_file);
					ErrorHandler(MEMORY_ALLOCATION_ERR);
					FreeTestProcessArray(tests_processes,num_of_files);
				}
				strcpy(tests_processes[j].file_name,temp_str);
				
				tests_processes[j].file_name[i]='\0';
				j++;
				i=0;
			}
		}
		
	}

	free(temp_str);
	fclose(input_file);
	*num_of_files_pointer=num_of_files;
	return tests_processes;
}
BOOL CreateOutputDirectory(char* output_directory)
{
	LPTSTR temp_lptstr=NULL;
	temp_lptstr=ConvertCharStringToLPTSTR(output_directory);
	if (CreateDirectory(temp_lptstr, NULL) || ERROR_ALREADY_EXISTS == GetLastError())
	{
		free(temp_lptstr);
		return TRUE;
	}
	else
	{
		free(temp_lptstr);
		return FALSE;
	}
}
ERR_CODE GenerateLogPath(TestsProcess *tests_process, char *output_directory)
{
	char*	log_path=NULL;
	char	suffix[]=OUTPUT_SUFFIX;
	char	c=' ';
	int		log_path_length;
	int		i,j;

	log_path_length=strlen(output_directory)+strlen(tests_process->file_name)+strlen(suffix)+2;
		
	log_path=(char*)malloc(log_path_length*sizeof(char)); //allocate memory for output path string
	if(log_path == NULL)                     
	{
		return ErrorHandler(MEMORY_ALLOCATION_ERR);
	}

	strcpy(log_path,output_directory);
	if(output_directory[strlen(log_path)-1]!='\\')
	{
		strcat(log_path,"\\"); //adding \ if needed
	}
	strcat(log_path,tests_process->file_name);

	for(j=0,i=(int)(strrchr(log_path, '.')-log_path);c!='\0';j++,i++) //copying the suffix
	{
		c=suffix[j];
		log_path[i]=c;
	}
	log_path[i]='\0';
	strcat(log_path,LOG_EXTENSION); //adding the log extention LOG_EXTENSION defined in the header file

	tests_process->log_path=log_path;
	return SUCCESS_ERR;
}
char* GenerateRuntimeLogfilePath(char *output_directory)
{ 
	char*	runtime_log_path=NULL;
	char	runtime_filename[]=MANAGER_LOG_FILENAME; //runtime log filename can be changed in the header file
	runtime_log_path=(char*)malloc(strlen(runtime_filename)+strlen(output_directory)+5);
	if(!runtime_log_path)
	{
		ErrorHandler(MEMORY_ALLOCATION_ERR);
	}
	sprintf(runtime_log_path,"%s\\%s",output_directory,runtime_filename);

	return runtime_log_path;
}
ERR_CODE GenerateCommandLine(TestsProcess *tests_process)
{
	char*	command_line_chars=NULL;
	char	testing_tool[]=TESTING_TOOL; //testing tool file name can be changed in the header file
	int		command_line_length;

	command_line_length=strlen(testing_tool)+
						strlen(tests_process->file_name)+
						strlen(tests_process->log_path)+
						10; //add 10 for " sings, '\0' and spaces.

	command_line_chars=(char*)malloc(command_line_length);
	if(command_line_chars == NULL)                     
	{
		return ErrorHandler(MEMORY_ALLOCATION_ERR);
	}
	sprintf(command_line_chars,"\"%s\" \"%s\" \"%s\"", testing_tool, tests_process->file_name, tests_process->log_path);

	tests_process->IpCommandLineChars=command_line_chars;
	return SUCCESS_ERR;
}
void FreeTestProcessArray(TestsProcess *tests_processes, int num_of_files)
{
	int	i;
	for(i=0;i<num_of_files;i++)
	{
		free(tests_processes[i].file_name);
		free(tests_processes[i].IpCommandLineChars);
		free(tests_processes[i].log_path);
		CloseHandle(tests_processes[i].IpProcessInformation.hProcess);
	}
	free(tests_processes);
}


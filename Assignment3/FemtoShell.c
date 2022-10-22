#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RET      0
#define INTERNALCOMMAND 0
#define EXTERNALCOMMAND 1
#define OTHERCOMMAND    2 

char* localVars[1000];
char* localValues[1000];
unsigned int localVarIndex=0;
unsigned int localValueIndex=0;

int checkCommand(char command[])
{
    int i = 0;
	int retResult=1;
    while (command[i])
    {
        if (command[i] == '=')
		{
			retResult=0;
			break;
		}
        i++;
    }
    if(strcmp(command,"set")==0||strcmp(command,"export")==0)
    {
        retResult=2;
    }
    return retResult;
}
int findLocalVarIndx(char* var)
{
	int i;
    int retResult = -1;
	
    for (int i = 0; i < localVarIndex; i++)
    {
        if (strcmp(var, localVars[i]) == 0)
		{
            retResult=i;
			break;
		}
    }
	
    return retResult;
}
void parseCommand(char command[],int commandTybe,const char* delmeter ,char* argv[])
{
	
   
   if(commandTybe==EXTERNALCOMMAND)
   {
	  int idxCommand=0;
	  char * token = strtok(command, delmeter);
   
	   while( token != NULL ) 
	   {
		  argv[idxCommand]=strdup(token);
		  token = strtok(NULL, delmeter);
		  idxCommand++;
	   } 
   }
   else if(commandTybe==INTERNALCOMMAND)
   {
       int isValue=0;
	   char * token = strtok(command, delmeter);
   
	   while( token != NULL ) 
	   {
	       if(isValue==0)
	       {
	           localVars[localVarIndex]=strdup(token);
	           localVarIndex++;
	           isValue=1;
	       }
	       else if(isValue==1)
	       {
	           localValues[localValueIndex]=strdup(token);
	           localValueIndex++;
	           isValue=0;
	       }
		  
		  token = strtok(NULL, delmeter);
	   } 
   }
}
void set()
{
    printf("%d\n",localVarIndex);
    
    if (localVarIndex == 0)
    {
        printf("No local Variables found\n");
    }
    else
    {
        int i;
        
        for ( i= 0; i < localVarIndex;i++)
        {
            printf("localVars[%d]: %s = %s\n", i, localVars[i], localValues[i]);
        }
    }
    
}
void export(char var[])
{
    int i = findLocalVarIndx(var);
    if (i != -1)
    {
        setenv( localVars[i], localValues[i], 1);
    }
    else
        printf("Variable: %s is not exist\n", var);
}

int main()
{
    char* argv[1000];
    char* dummy[10];
	char command[1000];
	
	int* parentStatus;
    

	while (1)
    {
		printf("Ana Gahz Ya Basha$ ");
		fgets(command, 1000, stdin);
		
		int forkResult = fork();
		
		if (forkResult < 0)
		{
            printf("Faild\n");
        }
		else if (forkResult > 0)
        {
            wait(parentStatus);
        }
        else if (forkResult == 0 && strlen(command) > 1)
        {
            int lenCommand = strlen(command);
            command[lenCommand - 1] = '\0';
            
            if (checkCommand(command) == INTERNALCOMMAND)
            {
                parseCommand(command,INTERNALCOMMAND,"=" ,dummy);
            }
            else
            {
                parseCommand(command,EXTERNALCOMMAND," " ,argv);
                if (strcmp(command,"set")==0)
                {
                    set();
                }
                else if(strcmp(command,"export")==0)
                {
                    export(dummy[1]);
                }
                else
                {
                    if (execvp(argv[0], argv) == -1)
                    {
                        printf("%s: command not found\n", argv[0]);
                    }
                    break;
                }
            }
                
        }
	}
	
	
	
	return RET;
}
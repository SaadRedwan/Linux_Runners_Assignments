
#include <stdio.h>
#include <string.h>
      
#define RETURN    0
#define TRUE      1
#define MAXINPUT  200

int main()
{
	const char* exit="exit";
	char command[MAXINPUT];
	char resultCompare=1;

	while(TRUE)
	{
		printf("\nPlease enter a command: ");
         	
         	scanf("%[^\n]s",command);
                getchar();	
		
		//it will return 0 if two string are equal
		resultCompare=strcmp(command, exit);

		if(resultCompare!=0)
		{
			printf("\nYour entered command is: %s\n",command);
		}
		else
		{
			printf("\nGood Bye\n");
			break;
		}
		strcpy(command," ");
	}

	return RETURN;
}
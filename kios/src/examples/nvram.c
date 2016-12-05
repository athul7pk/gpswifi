/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : nvram.c

* Creation Date : 14-11-2016

* Last Modified : Friday 02 December 2016 06:07:02 PM IST

* Created By :  Athul P.K

_._._._._._._._._._._._._._._._._._._._._.*/
#include "includes.h"

char buf[512];
char *nvram_get(int id, const char *token)
{
	char* value = NULL;
	FILE* fd = fopen(CONFIG_FILE_PATH, "r+");
	char str[512];
	if (fd != NULL)
	{
		//printf("File opened\n");
		while (fgets(str, sizeof(str), fd))
		{
			if (str[strlen(str) - 1] == '\n')
			{
				str[strlen(str) - 1] = '\0';
			}
			if (strncmp(str, token, strlen(token)) == 0)
			{
				int i = strlen(token);
				while (str[i] == ' ' || str[i] == '=')
				{
					i++;
				}
				strcpy(buf, &(str[i]));
				value = buf;
				//printf("Found %s = %s\n", token, buf);
				break;
			}
		}
	}
	if(fd) fclose(fd);
	return value;	
}
int nvram_bufset(int index, const char *token, const char *value)
{
	int status = 0;
	FILE* fd = fopen(CONFIG_FILE_PATH, "r+");
	FILE* tempfd = fopen(TEMP_FILE_PATH, "w");
	char str[512];
	printf("%s (%s)->(%s)\n",__FUNCTION__,token,value);
	if (fd != NULL && tempfd != NULL)
	{
		//printf("File opened\n");
		while (fgets(str, sizeof(str), fd))
		{
			//printf("line : %s\n", str);
			if (strncmp(str, token, strlen(token)) == 0)
			{
				//printf("Found\n");
				sprintf(str, "%s=%s\n", token, value);
				status = 1;
			}
			fputs(str, tempfd);
		}
		fclose(fd);
		fclose(tempfd);
	}
	if (status == 1)
	{
		sprintf(str, "cp -v %s %s", TEMP_FILE_PATH, CONFIG_FILE_PATH);
		system(str);
	}
	return status;	
}




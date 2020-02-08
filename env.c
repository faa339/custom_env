/*
Author: Faris Alotaibi

Description: This is an implementation of the 
utility env.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;
#define ARRSIZE 80


int GetNumofVars(char** initialarglist);
void AddVarstoArr(char** argsarr, char** initialarglist, int varcount);
void CopyArgstoEnviron(char** argsarr,int numofvars,int environsize);
void DisplayEnv(void);

int main(int argc, char *argv[])
{
	if(argc==1)
	{
		DisplayEnv();
		return 0;
	}


	//Getting the total # of ptrs in environ for later use
	int environsize = 0;
	char **environcounter = environ;
	while(*environcounter != 0)
	{
		environsize++;
		environcounter++;
	}

	//Array to store env vals from argv, loop to get them 
	int varcount;
	varcount = GetNumofVars(argv);
	char** argsarr = (char **) malloc(sizeof(char *) * varcount);
	AddVarstoArr(argsarr,argv,varcount);
	CopyArgstoEnviron(argsarr,varcount,environsize); 
	DisplayEnv();
	return 0;
}

void DisplayEnv(void)
{
	while(*environ!=0)
	{
		printf("%s\n",*environ);
		environ++;
	}
}

int GetNumofVars(char **initialarglist)
{
	int varnum = 0, i=0;
	while(initialarglist[i] != 0)
	{
		if(strchr(initialarglist[i],'=') == NULL)
		{
			i++;
		}else
		{
			varnum++;
			i++;
		}
	}
	return varnum;
}

void AddVarstoArr(char** argsarr, char** initialarglist, int varcount)
{
	for(int i=1;i<=varcount;i++)
	{
		argsarr[i-1] =(char *) malloc(strlen(initialarglist[i])+1);
		strcpy(argsarr[i-1],initialarglist[i]);
	}
}

void CopyArgstoEnviron(char** argsarr,int numofvars,int environsize)
{
	char **newenviron =(char**) malloc(sizeof(char*)*(environsize+numofvars));
	for(int i=0;i<environsize;i++)
	{
		newenviron[i] = (char*) malloc(strlen(environ[i]) + 1);
		strcpy(newenviron[i],environ[i]);
	}

	for(int j=environsize;j<(numofvars+environsize);j++)
	{
		newenviron[j] = (char*) malloc(strlen(argsarr[j]) + 1);
		strcpy(newenviron[j],argsarr[(j-environsize)]);
	}
	environ = newenviron;
}

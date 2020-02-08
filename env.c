/*
Author: Faris Alotaibi

Description: This is an implementation of the 
utility env. Providing arguments adds them onto 
the end of environ. Usage is:
./env [name=value]...[command [args...]]
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;


int GetNumofVals(char** initialarglist);
void AddVarstoArr(char** valsarr, char** initialarglist, int varcount,int offset);
void CopyValstoEnviron(char** valsarr,int numofvars,int environsize);
void DisplayEnv(void);
void FreeValsArr(char** valsarr,int arrsize);
void ExecHandler(char** arglist, int numofargs);

int main(int argc, char *argv[])
{
	if(argc==1)
	{
		DisplayEnv();
		return 0;
	}

	//Getting the total # of strs in environ for later use
	int environsize = 0;
	char **environcounter = environ;
	while(*environcounter != 0)
	{
		environsize++;
		environcounter++;
	}
	//Array to store env vals from argv, loop to get them 
	int valcount;
	valcount = GetNumofVals(argv);
	char** argsarr = (char **) malloc(sizeof(char *) * (valcount+1));
	argsarr[valcount] = 0;
	if((strcmp(argv[1],"-i")) == 0)
	{
		AddVarstoArr(argsarr,argv,valcount,1);
		environ = argsarr;
		if(argv[valcount+2])
		{
			ExecHandler(&argv[valcount+2],(argc-valcount-2));
			perror("Failed to exec");
			return 0;
		}
		DisplayEnv();
		return 0;
	}else
	{
		AddVarstoArr(argsarr,argv,valcount,0);
		CopyValstoEnviron(argsarr,valcount,environsize);
		if(argv[valcount+1])
		{
			ExecHandler(&argv[valcount+1], (argc-valcount-1));
			perror("Failed to exec");
			return 0;
		} 
		DisplayEnv();
	}

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

int GetNumofVals(char **initialarglist)
{
	int valnum = 0, i=0;
	while(initialarglist[i] != 0)
	{
		if(strchr(initialarglist[i],'=') == NULL)
		{
			i++;
		}else
		{
			valnum++;
			i++;
		}
	}
	return valnum;
}

void AddVarstoArr(char** valsarr, char** initialarglist, int varcount,int offset)
{
	
	for(int i= 1;i<=varcount;i++)
	{
		valsarr[i-1] =(char *) malloc(strlen(initialarglist[i+offset])+1);
		strcpy(valsarr[i-1],initialarglist[i+offset]);
	}
}

void CopyValstoEnviron(char** valsarr,int numofvals,int environsize)
{
	int newsize = environsize + numofvals;
	char **newenviron =(char**) malloc(sizeof(char*)*(newsize+1));
	newenviron[newsize] = 0;
	for(int i=0;i<environsize;i++)
	{
		newenviron[i] = (char*) malloc(strlen(environ[i]) + 1);
		strcpy(newenviron[i],environ[i]);
	}

	for(int j=environsize;j<(numofvals+environsize);j++)
	{
		newenviron[j] = (char*) malloc(strlen(valsarr[j]) + 1);
		strcpy(newenviron[j],valsarr[(j-environsize)]);
	}
	environ =(char **) realloc(newenviron,sizeof(char*) * (environsize+numofvals)); 
	FreeValsArr(valsarr,numofvals);
}

void FreeValsArr(char** valsarr,int arrsize)
{
	for(int i=0;i<arrsize;i++)
		free(valsarr[i]);
	free(valsarr);
}

void ExecHandler(char** arglist, int numofargs)
{
	char** execargs = (char**) malloc((sizeof(char*) * (numofargs+1))) ;
	execargs[numofargs] = 0;
	for(int i=0; i<numofargs; i++)
	{
		execargs[i] = (char*) malloc(strlen(arglist[i]) + 1);
		strcpy(execargs[i],arglist[i]);
	}
	execvp(arglist[0],execargs);
}
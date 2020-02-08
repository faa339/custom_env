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


int getnumofVars(char** initialarglist);
void addargstoArr(char** argsarr, char** initialarglist, int varcount);

int main(int argc, char *argv[])
{
	//If env is called alone, simply display the env vars
	if(argc == 1)
	{
		while(*environ != 0)
		{
			printf("%s\n", *environ);
			environ++;
		}
		return 0;
	}

	//Array to store env vals from argv, loop to get them 
	int varcount;
	varcount = getnumofVars(argv);
	char** argsarr = (char **) malloc(sizeof(char *) * varcount);
	addargstoArr(argsarr,argv,varcount);
	return 0;

}
int getnumofVars(char **initialarglist)
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

void addargstoArr(char** argsarr, char** initialarglist, int varcount)
{
	for(int i=1;i<=varcount;i++)
	{
		argsarr[i] =(char *) malloc(strlen(initialarglist[i])+1);
		strcpy(argsarr[i],initialarglist[i]);
		printf("%s\n",argsarr[i]);
	}
}
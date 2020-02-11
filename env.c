/*
Author: Faris Alotaibi

Description: This is an implementation of the 
utility env. Providing arguments adds them onto 
the end of environ. Usage is:
./env [name=value]...[command [args]...]
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


extern char **environ;


int GetNumofVals(char** initialarglist);
void AddVarstoArr(char** valsarr,char** initialarglist,
	int varcount,int offset);
void CopyValstoEnviron(char** valsarr,int numofvars,int environsize);
void DisplayEnv(void);
void FreeValsArr(char** valsarr,int arrsize);
void ExecHandler(char** arglist,int numofargs);
void EnvChange(char** argv, int valcount, int environsize);
void Errorhandle(void);


int main(int argc, char *argv[])
{
	errno = 0; //We just started so no errors -- will change if so!
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
	//Getting total # of name=val strings from argv 
	int valcount = GetNumofVals(argv);
	if((strcmp(argv[1],"-i")) == 0) //if -i was selected as an option
	{
		//Creating array to store name=val pairs then adding them
		char** argsarr = (char **) malloc(sizeof(char *) * (valcount+1));
		
		argsarr[valcount] = 0;
		AddVarstoArr(argsarr,argv,valcount,1);
		//We pass in 1 as the offset so that we skip -i when adding elts
		environ = argsarr;
		if(argv[valcount+2])//Is there a program after name=value pairs? 
		{
			/*
			If so, pass in the address of the entry with the command name 
			and the count of elements afterwards. In this case, we subtract
			argv[0] and "-i" as well as the number of key=value pairs to
			obtain the correct remaining number of arguments.
			*/
			ExecHandler(&argv[valcount+2],(argc-valcount-2));
			Errorhandle(); //Reached if invalid command specified
		}
		DisplayEnv(); //If no arguments selected, simply display env
		return 0;
	}else 
	{
		/*
		In this case, we need to add the new name=val pairs to 
		environ. We pass in 0 for offset here since theres no -i 
		to skip. We also copy argsarr elements into environ.
		*/
		EnvChange(argv,valcount, environsize);
		char** argsarr = (char **) malloc(sizeof(char *) * (valcount+1));
		Errorhandle();
		argsarr[valcount] = 0;
		
		AddVarstoArr(argsarr,argv,valcount,0);
		CopyValstoEnviron(argsarr,valcount,environsize);
		FreeValsArr(argsarr,valcount); //We dont need argsarr anymore 

		if(argv[valcount+1])
		{
			//Same idea as check @ line 55
			ExecHandler(&argv[valcount+1], (argc-valcount-1));
			Errorhandle();
		} 
		DisplayEnv();
	}

	return 0;
}

void Errorhandle(void)
{
	//Function to simplify error checking -- if errno!=0, some function failed
	if(errno != 0)
	{
		perror("Error");
		exit(EXIT_FAILURE);
	}	
}

void DisplayEnv(void)
{
	//This function prints all variables in environ
	while(*environ!=0)
	{
		printf("%s\n",*environ);
		environ++;
	}
}

int GetNumofVals(char **initialarglist)
{
	//Counts the number of key=val pairs, determined by whether
	// '=' is present 
	int valnum = 0, i = 0;
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

void EnvChange(char** argv, int valcount, int environsize)
{
	/*
	This function is called to see if we have to change any 
	existing values in env. If so, the one changed is replaced 
	in argv with "~IN" 
	*/
	for(int i=1; i<= valcount; i++)
	{
		int namesize = 0;
		while(argv[i][namesize]!= '=')
			namesize++;

		for(int j=0; j<environsize;j++)
		{
			if((memcmp(environ[j],argv[i],namesize)) == 0)
			{
				environ[j] = (char*) malloc(strlen(argv[i])+1);
				Errorhandle();
				strcpy(environ[j],argv[i]);
				argv[i] = "~IN";
			}
		}
	}
}

void AddVarstoArr(char** valsarr,char** initialarglist,
	int varcount,int offset)
{
	//Copy vars in initialarglist to values array
	for(int i= 1;i<=varcount;i++)
	{
		valsarr[i-1] =(char *) malloc(strlen(initialarglist[i+offset])+1);
		Errorhandle();
		strcpy(valsarr[i-1],initialarglist[i+offset]);
	}
}

int SizeChange(char** valsarr, int numvals)
{
	//This function edits the size given if there are any strings
	//that were already copied into environ.
	int newsize = numvals;
	for(int i=0;i<numvals;i++)
	{
		if((strcmp(valsarr[i],"~IN")) == 0)
			newsize--;
	}
	return newsize;
}

void CopyValstoEnviron(char** valsarr,int numofvals,int environsize)
{
	/*
	This function copies all values in valsarr into environ. It 
	does this by first inserting all of environs variables into 
	the array newenviron, then tacking on the valsarr values. 
	*/
	
	int newsize = environsize + SizeChange(valsarr,numofvals);
	char **newenviron =(char**) malloc(sizeof(char*)*(newsize+1));
	Errorhandle();
	newenviron[newsize] = 0;
	for(int i=0;i<environsize;i++)
	{
		newenviron[i] = (char*) malloc(strlen(environ[i]) + 1);
		Errorhandle();
		strcpy(newenviron[i],environ[i]);
	}
	
	int envcounter = environsize;
	//Only copy val if string isnt in environ
	for(int j=0;j<numofvals;j++)
	{
		if((strcmp(valsarr[j],"~IN")) != 0)
		{
			newenviron[envcounter] = (char*) malloc(strlen(valsarr[j])+1);
			Errorhandle();
			strcpy(newenviron[envcounter],valsarr[j]);
			envcounter++;
		}
	}
	//Making sure environ gets the mem it needs
	environ =(char **) realloc(newenviron,sizeof(char*)*(newsize+1));
	Errorhandle();
}

void FreeValsArr(char** valsarr,int arrsize)
{
	//Free the array of name=vals
	for(int i=0;i<arrsize;i++)
		free(valsarr[i]);
	free(valsarr);
	Errorhandle();
}

void ExecHandler(char** arglist, int numofargs)
{
	/*
	Exec the command given with the arguments. Creating
	an array of arguments based on the arglist passed in
	*/
	char** execargs = (char**) malloc((sizeof(char*) * (numofargs+1)));
	Errorhandle();
	execargs[numofargs] = 0;
	for(int i=0; i<numofargs; i++)
	{
		execargs[i] = (char*) malloc(strlen(arglist[i]) + 1);
		Errorhandle();
		strcpy(execargs[i],arglist[i]);
	}
	execvpe(arglist[0],execargs,environ);
	Errorhandle();
}

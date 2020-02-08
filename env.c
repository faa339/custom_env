/*
Author: Faris Alotaibi

Description: This is an implementation of the 
utility env.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char *argv[])
{
	/*
	First step is to see what we've been given
	when env is called -- is there a -i? Is it 
	blank? Are there additional values? Is there 
	a program to be executed and does it need 
	those values or is it being simply executed 
	through env?
	*/

	if(argc == 1)
	{
		while(environ != 0)
		{
			printf("%s\n", *environ);
			environ++;
		}
	}
}
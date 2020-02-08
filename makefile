env: env.c 
	gcc -Wall -std=c99 -o env env.c

clean:
	rm -f env
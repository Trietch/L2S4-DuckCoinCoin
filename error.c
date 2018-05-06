#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void error_report(unsigned long line, int err_c)
{
	char error[256];
	sprintf(error, "%lu: ", line);
	switch(err_c)
	{
		case 2:
			strcat(error, "sprintf()");
			break;
		case 4:
			strcat(error, "malloc()");
			break;
		case 8:
			strcat(error, "i'm not supposed to be here :(");
			break;
	}
	perror(error);
	exit(err_c);	
}

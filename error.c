#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/******************************************************************
 * ERROR_REPORT *                                                 *
 ****************                                                 *
 *	                                                              *
 *	Affiche l'erreur rencontrée et arrete l'éxécution du          *
 *	programme.	                                                  *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void error_report(unsigned long line, int err_c)
{
	char error[32];
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
		case 16:
			strcat(error, "fopen()");
			break;
		case 32:
			strcat(error, "fclose()");
			break;
		case 64:
			strcat(error, "fgets()");
			break;
	}
	perror(error);
	exit(err_c);	
}

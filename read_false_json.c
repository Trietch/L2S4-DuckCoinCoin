#include <stdio.h>
#include <stdlib.h>

#include "defines.h"


/******************************************************************
 * GET_DIFFICULTY_FILE *                                          *
 ***********************                                          *
 *                                                                *
 *  Récupère la difficulté dans le fichier (file_temp).           *
 *	                                                              *
 *  Return : unsigned long                                        *
 *                                                                *
 ******************************************************************/

unsigned long get_difficulty_file(FILE *file_temp)
{
	char buffer[512];
	for ( char i = 0; i < 2; ++i )
	{
		if ( fgets(buffer, 512, file_temp) == NULL )
			error_report(20, ERR_FGETS);
	}
	return atoi(buffer);
}


/******************************************************************
 * GET_BLOCK_NB_FILE *                                            *
 *********************                                            *
 *                                                                *
 *  Récupère le nombre de block dans le fichier (file_temp)       *
 *	                                                              *
 *  Return : unsigned long                                        *
 *                                                                *
 ******************************************************************/

unsigned long get_block_nb_file(FILE *file_temp)
{
	return get_difficulty_file(file_temp);
}

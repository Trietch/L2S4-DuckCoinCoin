#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "blockchain.h"
#include "defines.h"


/******************************************************************
 * JSON_EXPORT *                                                  *
 ***************                                                  *
 *	                                                              *
 *	Exporte la blockchain (b) dans le format JSON dans le         *
 *	fichier (JSON_NAME).                                          *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void json_export(Blockchain *b)
{
	FILE *file = fopen(JSON_NAME, "w+");
	if ( !file )
		error_report(22, ERR_FILE_OPEN);
	fprintf(file, "{");
	fprintf(file, "\"difficulty\": %lu,\"nbBlocks\": %lu,\"BC\": [", get_difficulty(b), get_number_blocks(b));
	export_block(file, get_first_block(b), get_number_blocks(b));
	fprintf(file, "]}");
	printf("Blockchain exported as %s\n", JSON_NAME);
	if ( fclose(file) )
		error_report(30, ERR_FILE_CLOSE);
}

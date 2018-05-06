#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "blockchain.h"
#include "defines.h"

void json_export(Blockchain *b)
{
	FILE *file = fopen(JSON_NAME, "w+");
	if ( !file )
		exit(-1);
	fprintf(file, "{");
	fprintf(file, "\"difficulty\": %lu,\"nbBlocks\": %lu,\"BC\": [", get_difficulty(b), get_number_blocks(b));
	export_block(file, get_first_block(b), get_number_blocks(b));
	fprintf(file, "]}");
	printf("Blockchain exported as %s\n", JSON_NAME);
	fclose(file);
}

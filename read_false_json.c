#include <stdio.h>
#include <stdlib.h>

unsigned long get_difficulty_file(FILE *file_temp)
{
	char buffer[512];
	for ( char i = 0; i < 2; ++i )
		fgets(buffer, 512, file_temp);
	return atoi(buffer);
}

unsigned long get_block_nb_file(FILE *file_temp)
{
	return get_difficulty_file(file_temp);
}

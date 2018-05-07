#define MAIN

#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "blockchain.h"
#include "transaction.h"
#include "defines.h"
#include "validity.h"
#include "cheater.h"
#include "json_export.h"
#include "json.h"
#include "test_json.h"
#include "read_false_json.h"

int main(int argc, char *argv[])
{
	printf("L2S4-DuckCoinCoin | Tristan Marrec\n");
	unsigned long difficulty;
	unsigned long block_nb;
	Blockchain *b;
	unsigned long transaction_nb;

	if ( argc != 4 && argc != 5 && argc != 6 )
	{
		printf("Usage: duckcoincoin DIFFICULTY BLOCKS_NB LOG\n");
		printf("Usage: duckcoincoin DIFFICULTY BLOCKS_NB LOG BLOCK_TO_CHEAT_NB \n");
		printf("Usage: duckcoincoin DIFFICULTY BLOCKS_NB LOG BLOCK_TO_CHEAT_TRANSACTION_NB TRANSACTION_TO_CHEAT_NB \n");
		printf("Usage: duckcoincoin JSON_FILE\n");
		exit(0);
	}
	if ( argc == 2 )
	{
		FILE *file_temp = fopen(JSON_TEMP, "r");
		get_json_value(JSON_NAME);
		difficulty = get_difficulty_file(file_temp);
		block_nb = get_block_nb_file(file_temp);
		b = blockchain_create(difficulty);
		fclose(file_temp);
		return 0;
		/* Non fini */
	}
	
	difficulty = atoi(argv[1]);
	block_nb = atoi(argv[2]);
	show_log = atoi(argv[3]);
	b = blockchain_create(difficulty);
	Transaction **transaction_list;

	for ( unsigned long i = 1; i < block_nb; ++i )
	{
		transaction_list = generate_random_transaction_list(&transaction_nb);
		b = blockchain_add_block(b, transaction_list, transaction_nb);
	}
		
	if ( show_log )
		blockchain_print(b);

	check_validity(b);

	if ( argc == 5 )
	{
		block_cheater(b, atoi(argv[4]));
		check_validity(b);
	}
	else if ( argc == 6 )
	{
		transaction_cheater(b, atoi(argv[4]), atoi(argv[5]));
		check_validity(b);
	}
	if ( show_log )
		blockchain_print(b);
	json_export(b);
	if ( argc != 6 ) //Erreur si free alors que transacations cheated
		blockchain_free(b);
	return 0;
}

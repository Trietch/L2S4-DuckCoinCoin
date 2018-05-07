#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "block.h"
#include "transaction.h"
#include "defines.h"
#include "sha256_utils.h"


/******************************************************************
 * STRUCT _BLOCKCHAIN (aka BLOCKCHAIN) *                          *
 ***************************************                          *
 *                                                                *
 * 	first_block   : Premier Block de la Blockchain                *
 * 	difficulty    : Difficulté de la Blockchain                   *
 * 	number_blocks : Nombre de Blocks de la Blockchain             *
 *                                                                *
 ******************************************************************/

typedef struct _Blockchain
{
	Block *first_block;
	unsigned long difficulty;
	unsigned long number_blocks;
} Blockchain;


/******************************************************************
 * BLOCKCHAIN_CREATE *                                            *
 *********************                                            *
 *	                                                              *
 *	Initialise la structure de données Blockchain avec la         *
 *	difficulté (difficulty) et creer le block genesis.            *
 *	                                                              *
 *  Return : Blockchain*                                          *
 *                                                                *
 ******************************************************************/

Blockchain *blockchain_create(unsigned long difficulty)
{
	printf("Blockchain creation..\n");
	Blockchain *blockchain = malloc(sizeof(struct _Blockchain));
	if ( !blockchain )
		error_report(43, ERR_MALLOC);
	blockchain->difficulty = difficulty;
	blockchain->number_blocks = 1;
	blockchain->first_block = block_create(0, 0, NULL, 0);
	
	char *hash = malloc(HASH_SIZE*sizeof(char));
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4));
	
	if ( !hash || !string_block )
		error_report(50, ERR_MALLOC);
	block_to_string(blockchain->first_block, string_block);
	sha256ofString((BYTE *)string_block, hash);
	block_sign_hash(blockchain->first_block, hash);
	return blockchain;
}


/******************************************************************
 * BLOCKCHAIN_FREE *                                              *
 *******************                                              *
 *	                                                              *
 *	Vide de la mémoire tout les blocks de la blockchain et la     *
 *	blockchain elle même.                                         *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void blockchain_free(Blockchain *b)
{
	block_all_delete(b->first_block);
	free(b);
}


/******************************************************************
 * GET_DIFFICULTY *                                               *
 ******************                                               *
 *	                                                              *
 *	Retourne la difficulté de la blockchain (b).                  *
 *	                                                              *
 *  Return : unsigned long                                        *
 *                                                                *
 ******************************************************************/

unsigned long get_difficulty(Blockchain *b)
{
	return b->difficulty;
}


/******************************************************************
 * DEC_BLOCKCHAIN *                                               *
 ******************                                               *
 *	                                                              *
 *	Décrémente le champ nombre de block de la blockchain (b).     *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void dec_blockchain(Blockchain *b)
{
	(b->number_blocks)--;
}


/******************************************************************
 * GET_NUMBER_BLOCKS *                                            *
 *********************                                            *
 *	                                                              *
 *	Retourne le nombre de blocks de la blockchain (b).            *
 *	                                                              *
 *  Return : unsigned long                                        *
 *                                                                *
 ******************************************************************/

unsigned long get_number_blocks(Blockchain *b)
{
	return b->number_blocks;
}


/******************************************************************
 * BLOCKCHAIN_ADD_BLOCK *                                         *
 ************************                                         *
 *	                                                              *
 *	Ajoute un block à la blochain (blockchain) avec n             *
 *	(transaction_nb) transactions de transaction_list et          * 
 *	incrémente le champ du nombre de block de la blockchain.      *
 *	                                                              *
 *  Return : Blockchain*                                          *
 *                                                                *
 ******************************************************************/

Blockchain *blockchain_add_block(Blockchain *blockchain, Transaction **transaction_list, unsigned long transaction_nb)
{
	blockchain->first_block = block_add(blockchain->first_block, blockchain->difficulty, transaction_list, transaction_nb);
	(blockchain->number_blocks)++;
	return blockchain;
}


/******************************************************************
 * BLOCKCHAIN_PRINT *                                             *
 ********************                                             *
 *	                                                              *
 *	Affiche les champs de la blockchain (blockchain) et les       *
 *	champs de tout les blocks de cette blockchain.                *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void blockchain_print(Blockchain *blockchain)
{
	printf("Difficulty : %lu | Number of blocks : %lu\n", blockchain->difficulty, blockchain->number_blocks);
	printf("   Memory    Index  Timestamp                                                        Last_hash   Nonce\n");
	printf("------------------------------------------------------------------------------------------------------\n");
	block_print(blockchain->first_block);
}


/******************************************************************
 * GET_FIRST_BLOCK *                                              *
 *******************                                              *
 *	                                                              *
 *	Retourne le premier block de la Blockchain (blockchain).      *
 *	                                                              *
 *  Return : Block*                                               *
 *                                                                *
 ******************************************************************/

Block *get_first_block(Blockchain *b)
{
	return b->first_block;
}


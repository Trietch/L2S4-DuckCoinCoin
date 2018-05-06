#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "block.h"
#include "transaction.h"
#include "defines.h"
#include "sha256_utils.h"

typedef struct _Blockchain
{
	Block *first_block;
	unsigned long difficulty;
	unsigned long number_blocks;
} Blockchain;

Blockchain *blockchain_create(unsigned long difficulty)
{
	printf("Blockchain creation..\n");
	Blockchain *blockchain = malloc(sizeof(struct _Blockchain));
	if ( !blockchain )
		error_report(17, ERR_MALLOC);
	blockchain->difficulty = difficulty;
	blockchain->number_blocks = 1;
	blockchain->first_block = block_create(0, 0, NULL, 0);
	
	char *hash = malloc(HASH_SIZE*sizeof(char));
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4));
	block_to_string(blockchain->first_block, string_block);
	if ( !hash )
		error_report(116, ERR_MALLOC);
	sha256ofString((BYTE *)string_block, hash);
	block_sign_hash(blockchain->first_block, hash);
	return blockchain;
}

void blockchain_free(Blockchain *b)
{
	block_all_delete(b->first_block);	
}

unsigned long get_difficulty(Blockchain *b)
{
	return b->difficulty;
}

void dec_blockchain(Blockchain *b)
{
	(b->number_blocks)--;
}

unsigned long get_number_blocks(Blockchain *b)
{
	return b->number_blocks;
}

Blockchain *blockchain_add_block(Blockchain *blockchain, Transaction **transaction_list, unsigned long transaction_nb)
{
	blockchain->first_block = block_add(blockchain->first_block, blockchain->difficulty, transaction_list, transaction_nb);
	(blockchain->number_blocks)++;
	return blockchain;
}

void blockchain_print(Blockchain *blockchain)
{
	printf("Difficulty : %lu | Number of blocks : %lu\n", blockchain->difficulty, blockchain->number_blocks);
	printf("   Memory    Index  Timestamp                                                        Last_hash   Nonce\n");
	printf("------------------------------------------------------------------------------------------------------\n");
	block_print(blockchain->first_block);
}

Block *get_first_block(Blockchain *b)
{
	return b->first_block;
}


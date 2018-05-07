#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "blockchain.h"
#include "block.h"
#include "defines.h"
#include "validity.h"


/******************************************************************
 * BLOCK_CHEATER *                                                *
 *****************                                                *
 *	                                                              *
 *	Supprime le n-ième (num) block de la blockchain (b) et        *
 *	recalcule tout les hash des blocks suivants pour que l'on ne  *
 *	puisse pas detecter la suppression de ce block.	              *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_cheater(Blockchain *b, unsigned long num)
{
	clock_t t1, t2;
	printf("Blockchain cheating block [%lu]..\n", num);
	fflush(stdout);
	t1 = clock();
	Block *block = get_ith_block(get_first_block(b), num);
	block_jump_next(block);
	blockchain_rebuild(block, get_difficulty(b), get_number_blocks(b));
	dec_blockchain(b);
	t2 = clock();
	printf("Done in %lfs\n",(double)((t2-t1)/1000000.0F));
}


/******************************************************************
 * TRANSACTION_CHEATER *                                          *
 ***********************                                          *
 *	                                                              *
 *	Supprime la n-ième (num_transaction) transaction du n_ième    *
 *	(num_block) block de la blockchain (b) et recalcule tout les  *
 *	hash des blocks suivants pour que l'on ne puisse pas detecter *
 *	la suppression de la transaction.                             *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void transaction_cheater(Blockchain *b, unsigned long num_block, unsigned long num_transaction)
{
	clock_t t1, t2;
	printf("Transaction cheating block [%lu] transaction [%lu]..\n", num_block, num_transaction);
	fflush(stdout);
	t1 = clock();
	Block *block = get_ith_block(get_first_block(b), num_block);
	block_del_transaction(block, num_transaction);
	blockchain_rebuild(block, get_difficulty(b), get_number_blocks(b));
	t2 = clock();
	printf("Done in %lfs\n",(double)((t2-t1)/1000000.0F));
}

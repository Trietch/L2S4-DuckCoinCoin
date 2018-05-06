#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "sha256_utils.h"
#include "transaction.h"
#include "defines.h"
#include "merkle_tree.h"

typedef struct _Block
{
	Transaction **transaction_list;
	struct _Block *next;
	char *prev_hash;
	char *hash;
	char *hash_merkle_root;
	char *timestamp;
	unsigned long index;
	unsigned long nonce;
	unsigned long number_transactions;
} Block;

char *getTimeStamp()
{
	char *str = malloc(128*sizeof(char));
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	strftime(str, 128, "%c", p);
	return str;
}

void block_to_string(Block *block, char *string_block)
{
	char *string = calloc(HASH_SIZE*3, sizeof(char));
	if ( !string )
		error_report(26, ERR_MALLOC);
	if ( sprintf(string+strlen(string), "%s%s%lu%s%lu%lu", block->prev_hash, block->hash_merkle_root, block->index, block->timestamp, block->nonce, block->number_transactions) < 1 )
		error_report(29, ERR_SPRINTF);
	strcpy(string_block, string);
	free(string);
}

Block *block_create(unsigned long index, char *prev_hash, Transaction **transaction_list, unsigned long transaction_nb)
{
	Block *block = calloc(1, sizeof(struct _Block));
	block->index = index;
	block->timestamp = getTimeStamp();
	block->prev_hash = prev_hash;
	block->nonce = 0;
	block->transaction_list = transaction_list;
	block->number_transactions = transaction_nb;
	block->hash_merkle_root = get_merkle_root(transaction_list, transaction_nb);
	block->hash = malloc(HASH_SIZE*sizeof(char));
	if ( !block->hash )
		error_report(48, ERR_MALLOC);
	return block;
}

void block_sign_hash(Block *b, char *hash)
{
	b->hash = hash;
}

bool is_good_hash(char *hash, unsigned long difficulty)
{
	for ( unsigned long i = 0; i < difficulty; ++i )
	{
		if ( hash[i] != '0' )
			return false;
	}
	return true;
}

Block *get_last_block(Block *block)
{
	Block **p_block = &block;
	while ( *p_block )
	{
		if ( !(*p_block)->next )
			return *p_block;	
		p_block = &(*p_block)->next;
	}
	error_report(74, ERR_SHOULD_NOT_BE_HERE);
	return NULL;
}

Block *block_add(Block *block, unsigned long difficulty, Transaction **transaction_list, unsigned long transaction_nb)
{
	Block **p_block = &block;
	Block *last_block = NULL;
	unsigned int i = 0;
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4));
	char *hash = malloc(HASH_SIZE*sizeof(char));
	if ( !hash )
		error_report(82, ERR_MALLOC);
	while ( *p_block )
	{
		if ( !(*p_block)->next )
			last_block = *p_block;
		p_block = &(*p_block)->next;
		i++;
	}
	*p_block = block_create(i, last_block->hash, transaction_list, transaction_nb);
	do
	{
		((*p_block)->nonce)++;
		block_to_string(*p_block, string_block);
		sha256ofString((BYTE *)string_block, hash);
	} while ( !is_good_hash(hash, difficulty) );
	if ( show_log )
		printf("%sBLOCK: [%lu] FOUND AFTER %lu TRIES WITH THE HASH : %.*s%s%s%s%s\n", BRED, (*p_block)->index, (*p_block)->nonce, (int)difficulty, hash, WHI, BYEL, hash+strlen(hash)-HASH_SIZE+difficulty+1, WHI);
	block_sign_hash(*p_block, hash);
	if ( *p_block )
		return block;
	error_report(104, ERR_SHOULD_NOT_BE_HERE);
	return NULL;
}

bool is_genesis(Block *b)
{
	return !strcmp(b->hash_merkle_root, "genesis");
}

bool is_prev_hash_valid(Block *b)
{
	if ( show_log )
		printf("Checking blocks previus hash.. ");
	Block *p_block = b;
	char *temp_hash;
	while ( p_block->next )
	{
		temp_hash = p_block->hash;
		p_block = p_block->next;
		if ( !is_genesis(p_block) && strcmp(temp_hash, p_block->prev_hash) )
		{
			printf("%sNOT OK! >:(%s\n", BRED, WHI);
			return false;
		}
	}
	if ( show_log )
		printf("%sOK!%s\n", BGRN, WHI);
	return true;
}

bool is_merkle_root_valid(Block *b)
{
	if ( show_log )
		printf("Checking blocks merkle root.. ");
	Block *p_block = b;
	while ( p_block )
	{
		char *generated_merkle_root = get_merkle_root(p_block->transaction_list, p_block->number_transactions);
		if ( strcmp(p_block->hash_merkle_root, generated_merkle_root) )
		{
			printf("%sNOT OK! >:(%s\n", BRED, WHI);
			return false;
		}
		p_block = p_block->next;
	}
	if ( show_log )	
		printf("%sOK!%s\n", BGRN, WHI);
	return true;
}

bool is_my_hash_valid(Block *b)
{
	if ( show_log )
		printf("Checking blocks hash.. ");
	Block *p_block = b;
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4)+50);
	char *hash = malloc(HASH_SIZE*sizeof(char));
	while ( p_block )
	{
		block_to_string(p_block, string_block);
		if ( !hash )
			error_report(116, ERR_MALLOC);
		sha256ofString((BYTE *)string_block, hash);
		if ( strcmp(p_block->hash, hash) )
		{
			printf("%sNOT OK! >:(%s\n", BRED, WHI);
			return false;
		}
		p_block = p_block->next;
	}
	if ( show_log )
		printf("%sOK!%s\n", BGRN, WHI);
	return true;
}

Block *get_ith_block(Block *b, unsigned long num)
{
	Block *p_block = b;
  	while ( p_block->index != num-1 )
	{
		p_block = p_block->next;
	}
	return p_block;	
}

void block_del_all_transaction(Block *b)
{
	for ( unsigned long i = 0; i < b->number_transactions-1; ++i )
	{
		free(b->transaction_list[i]);
	}
	free(b->transaction_list);	
}

void block_all_delete(Block *b)
{
	if ( b->next )
	{
		block_all_delete(b->next);
	}
	
	if ( b->index != 0 )
	{
		block_del_all_transaction(b);
		free(b->hash);
		free(b->hash_merkle_root);
		free(b);
	}
	/*free(b->hash);
	free(b->prev_hash);
	free(b->hash_merkle_root);
	free(b);*/
	//block_del_all_transaction(b);
}

void block_del_transaction(Block *b, unsigned long trans_nb)
{
	for ( unsigned long i = trans_nb; i < b->number_transactions-1; ++i )
	{
		b->transaction_list[i] = b->transaction_list[i+1];
	}
	(b->number_transactions)--;
	b->hash_merkle_root = get_merkle_root(b->transaction_list, b->number_transactions);
}

void block_jump_next(Block *b)
{
	b->next = b->next->next;
}

void blockchain_rebuild(Block *b, unsigned long difficulty, unsigned long nb_blocks)
{
	Block *p_block = b;
	unsigned long i = b->index;
	char *hash = malloc(HASH_SIZE*sizeof(char));
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4));
	while ( p_block->next )
	{
		if ( p_block->index+1 != p_block->next->index )
			(p_block->next->index)--;
		do
		{
			(p_block->nonce)++;
			block_to_string(p_block, string_block);
			sha256ofString((BYTE *)string_block, hash);
		} while ( !is_good_hash(hash, difficulty) );
		strcpy(p_block->hash, hash);
		p_block->next->prev_hash = p_block->hash;
		p_block = p_block->next;
		printf("%lu/%lu\r", i++, nb_blocks);
		fflush(stdout);
	}
	strcpy(p_block->prev_hash, hash);
	string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4));
	do
	{
		(p_block->nonce)++;
		block_to_string(p_block, string_block);
		sha256ofString((BYTE *)string_block, hash);
	} while ( !is_good_hash(hash, difficulty) );	
	strcpy(p_block->hash, hash);
}

void block_print(Block *block)
{
	Block *p_block = block;
	while ( p_block )
	{
		printf("%p ", (void*)p_block);
		printf("%4lu ", p_block->index);
		printf("%s ", p_block->timestamp);
		printf("%s ", p_block->prev_hash);
		printf("%8ld ", p_block->nonce);
		printf("%4ld ", p_block->number_transactions);
		printf("%s ", p_block->hash_merkle_root);
		printf("| %s", p_block->hash); 
		printf("\n");
		for ( unsigned long i = 0; i < p_block->number_transactions; ++i )
		{
			printf("          Transaction: ");
			transaction_print(p_block->transaction_list[i]);	
		}
		p_block = p_block->next;
	}
	printf("\n");
}

void export_block(FILE *file, Block *block, unsigned long nb_block)
{
	Block *b;
	for ( unsigned long i = nb_block; i > 0; --i )
	{
		b = get_ith_block(block, i);
		fprintf(file, "{");
		fprintf(file, "\"index\": %lu,", b->index);
		fprintf(file, "\"previousHash\": \"%s\",", b->prev_hash);
		fprintf(file, "\"timestamp\": \"%s\",", b->timestamp);
		fprintf(file, "\"nbTransactions\": %lu,", b->number_transactions);
		fprintf(file, "\"transactions\": [");
		for ( unsigned long j = 0; j < b->number_transactions; ++j )
		{
			fprintf(file, "\"%s\"", transaction_to_string(b->transaction_list[j]));
			if ( j != b->number_transactions-1 )
				fprintf(file, ",");
		}
		fprintf(file, "],");
		fprintf(file, "\"merkleRoot\": \"%s\",", b->hash_merkle_root);
		fprintf(file, "\"blockHash\": \"%s\",", b->hash);
		fprintf(file, "\"nonce\": %lu", b->nonce);
		fprintf(file, "}");
		if ( i > 1 )
			fprintf(file, ",");
	}	
}


#ifndef _BLOCK
#define _BLOCK

#include <stdbool.h>

#include "transaction.h"

typedef struct _Block Block;

Block *block_create(unsigned long index, char *prev_hash, Transaction **transaction_list, unsigned long transaction_nb);
Block *block_add(Block *block, unsigned long difficulty, Transaction **transaction_list, unsigned long transaction_nb);
void block_print(Block *block);
Block *get_last_block(Block *block);
bool is_genesis(Block *b);
bool is_my_hash_valid(Block *b);
bool is_prev_hash_valid(Block *b);
bool is_merkle_root_valid(Block *b);
void block_sign_hash(Block *b, char *hash);
Block *get_ith_block(Block *b, unsigned long num);
void block_jump_next(Block *b);
void blockchain_rebuild(Block *b, unsigned long difficulty, unsigned long nb_blocks);
void block_to_string(Block *block, char *string_block);
void block_del_transaction(Block *b, unsigned long trans_nb);
void block_all_delete(Block *b);
void export_block(FILE *file, Block *block, unsigned long nb_block);

#endif

#ifndef _BLOCKCHAIN
#define _BLOCKCHAIN

#include "transaction.h"
#include "block.h"

typedef struct _Blockchain Blockchain;

Blockchain *blockchain_create(unsigned long difficulty);
Blockchain *blockchain_add_block(Blockchain *blockchain, Transaction **transaction_list, unsigned long transaction_nb);
void blockchain_print(Blockchain *blockchain);
Transaction **generate_random_transaction_list();
Block *get_first_block(Blockchain *b);
unsigned long get_difficulty(Blockchain *b);
void blockchain_free(Blockchain *b);
unsigned long get_number_blocks(Blockchain *b);
void dec_blockchain(Blockchain *b);

#endif

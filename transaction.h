#ifndef _TRANSACTION
#define _TRANSACTION

typedef struct _Transaction Transaction;

Transaction *transaction_create(unsigned long amount);
Transaction *generate_random_transaction();
void transaction_print(Transaction *transaction);
size_t get_transaction_size();
void transaction_to_string(Transaction *transaction, char *string);

#endif

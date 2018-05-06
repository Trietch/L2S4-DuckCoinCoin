#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "defines.h"

typedef struct _Transaction
{
	unsigned long amount;
} Transaction;

void transaction_print(Transaction *transaction)
{
	printf("%sSource-Destination : %s%s%10lu%s\n", BBLU, WHI, YEL, transaction->amount, WHI);
}

Transaction *transaction_create(unsigned long amount)
{
	Transaction *transaction = malloc(sizeof(struct _Transaction));
	if ( !transaction )
		error_report(19, ERR_MALLOC);
	transaction->amount = amount;
	if ( show_log )
	{
		printf("	Generating random transaction : ");
		transaction_print(transaction);
	}
	return transaction;
}

Transaction *generate_random_transaction()
{
	return transaction_create(rand()%RANDOM_MAX_TRANSACTIONS);
}

Transaction **generate_random_transaction_list(unsigned long *transaction_nb)
{
	*transaction_nb = (rand()%(TRANSACTIONS_MAX-TRANSACTIONS_MIN))+TRANSACTIONS_MIN;
	Transaction **transaction_list = malloc((*transaction_nb)*sizeof(struct _Transaction));
	if ( !transaction_list )
		error_report(39, ERR_MALLOC);
	for ( unsigned long i = 0; i < *transaction_nb; ++i )
	{
		transaction_list[i] = generate_random_transaction();	
	}
	return transaction_list;
}

size_t get_transaction_size()
{
	return sizeof(struct _Transaction);
}

char *transaction_to_string(Transaction *transaction)
{
	char *string = malloc(31*sizeof(char));
	if ( !string )
		error_report(56, ERR_MALLOC);
	if ( sprintf(string, "Source-Destination :%lu", transaction->amount) < 1 )
		error_report(59, ERR_SPRINTF);
	return string;
}

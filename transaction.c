#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "defines.h"


/******************************************************************
 * STRUCT _TRANSACTION (aka TRANSACTION) *                        *
 *****************************************                        *
 *                                                                *
 * 	amount : Montant de la transaction                            *
 *                                                                *
 ******************************************************************/

typedef struct _Transaction
{
	unsigned long amount;
} Transaction;


/******************************************************************
 * TRANSACTION_PRINT *                                            *
 *********************                                            *
 *	                                                              *
 *	Affiche la transaction.                                       *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void transaction_print(Transaction *transaction)
{
	printf("%sSource-Destination : %s%s%10lu%s\n", BBLU, WHI, YEL, transaction->amount, WHI);
}


/******************************************************************
 * TRANSACTION_CREATE *                                           *
 **********************                                           *
 *	                                                              *
 *	Creer une transaction avec comme champ montant (amount).      *
 *	                                                              *
 *  Return : Transaction*                                         *
 *                                                                *
 ******************************************************************/

Transaction *transaction_create(unsigned long amount)
{
	Transaction *transaction = malloc(sizeof(struct _Transaction));
	if ( !transaction )
		error_report(50, ERR_MALLOC);
	transaction->amount = amount;
	if ( show_log )
	{
		printf("	Generating random transaction : ");
		transaction_print(transaction);
	}
	return transaction;
}

/******************************************************************
 * GENERATE_RANDOM_TRANSACTION *                                  *
 *******************************                                  *
 *	                                                              *
 *	Creer une transaction avec comme champ montant un nombre      *
 *	aléatoire compris entre 0 et (RANDOM_MAX_TRANSACTIONS).       *
 *	                                                              *
 *  Return : Transaction*                                         *
 *                                                                *
 ******************************************************************/

Transaction *generate_random_transaction()
{
	return transaction_create(rand()%RANDOM_MAX_TRANSACTIONS);
}


/******************************************************************
 * GENERATE_RANDOM_TRANSACTION_LIST *                             *
 ************************************                             *
 *	                                                              *
 *	Creer une liste de transactions avec comme champ montant      *
 *	un nombre aléatoire compris entre 0 et                        *
 *	(RANDOM_MAX_TRANSACTIONS).                                    *
 *	Cette liste est de taile comprise entre (TRANSACTIONS_MIN) et *
 *	(TRANSACTION_MAX).                                            *
 *	                                                              *
 *  Return : Transaction**                                        *
 *                                                                *
 ******************************************************************/

Transaction **generate_random_transaction_list(unsigned long *transaction_nb)
{
	*transaction_nb = (rand()%(TRANSACTIONS_MAX-TRANSACTIONS_MIN))+TRANSACTIONS_MIN;
	Transaction **transaction_list = malloc((*transaction_nb)*sizeof(struct _Transaction));
	if ( !transaction_list )
		error_report(96, ERR_MALLOC);
	for ( unsigned long i = 0; i < *transaction_nb; ++i )
	{
		transaction_list[i] = generate_random_transaction();	
	}
	return transaction_list;
}


/******************************************************************
 * GET_TRANSACTION_SIZE *                                         *
 ************************                                         *
 *	                                                              *
 *  Retourne la taille d'une transaction.                         *
 *	                                                              *
 *  Return : size_t                                               *
 *                                                                *
 ******************************************************************/

size_t get_transaction_size()
{
	return sizeof(struct _Transaction);
}


/******************************************************************
 * TRANSACTION_TO_STRING *                                        *
 *************************                                        *
 *	                                                              *
 *  Retourne une chaine de caractère qui représente la            *
 *  transaction (transaction).                                    *
 *	                                                              *
 *  Return : char*                                                *
 *                                                                *
 ******************************************************************/

char *transaction_to_string(Transaction *transaction)
{
	char *string = malloc(31*sizeof(char));
	if ( !string )
		error_report(136, ERR_MALLOC);
	if ( sprintf(string, "Source-Destination :%lu", transaction->amount) < 1 )
		error_report(139, ERR_SPRINTF);
	return string;
}

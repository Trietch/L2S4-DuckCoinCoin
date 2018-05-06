#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "transaction.h"
#include "sha256_utils.h"
#include "defines.h"

typedef struct _Elem
{
	char *data;
	struct _Elem *next;
} Elem;

typedef struct _Queue
{
	struct _Elem *first;
	unsigned long size;
} Queue;

Queue *queue_create()
{
	Queue *q = malloc(sizeof(struct _Queue));
	if ( !q )
		error_report(23, ERR_MALLOC);
	q->first = NULL;
	q->size = 0;
	return q;
}

void queue_add(Queue *q, char *data)
{
	Elem *e = malloc(sizeof(struct _Elem));
	if ( !e )
		error_report(33, ERR_MALLOC);
	e->data = data;
	e->next = NULL;
	if ( q->first != NULL )
	{
		Elem *p_e = q->first;
		while ( p_e->next != NULL )
		{
			p_e = p_e->next;
		}
		p_e->next = e;
	}
	else
	{
		q->first = e;
	}
	(q->size)++;
}

Queue *generate_queue(Transaction **transaction_list, unsigned long transaction_nb, char **hash)
{
	Queue *q = queue_create();
	for ( unsigned long i = 0; i < transaction_nb; ++i )
	{
		if ( !(hash[i]) )
			error_report(58, ERR_MALLOC);
		sha256ofString((BYTE *)transaction_to_string(transaction_list[i]), hash[i]);
		queue_add(q, hash[i]);
	}
	if ( transaction_nb%2 == 1 )
		queue_add(q, hash[transaction_nb-1]);
	return q;
}

char *queue_del(Queue *q)
{
	char *data;
	Elem *e = q->first;
	data = e->data;
	q->first = e->next;
	free(e);
	(q->size)--;
	return data;
}

char *generate_merkle_hash(Queue *q)
{
	char **hash = malloc((q->size)*sizeof(char*));
	char *concat = malloc(HASH_SIZE*2*sizeof(char));
	char *a = malloc(HASH_SIZE*sizeof(char));
	char *b = malloc(HASH_SIZE*sizeof(char));
	char *hash_f = malloc(HASH_SIZE*sizeof(char));
	unsigned long i = 0;
	if ( !hash || !concat || !a || !b )
		error_report(92, ERR_MALLOC);
	while ( q->size > 1 )
	{	
		hash[i] = malloc(65*sizeof(char));
		if ( !hash[i] )
			error_report(101, ERR_MALLOC);
		a = queue_del(q);
		b = queue_del(q);
		if ( sprintf(concat, "%s%s", a, b) < 1 )
			error_report(106, ERR_SPRINTF);
		sha256ofString((BYTE *)concat, hash[i]);
		queue_add(q, hash[i]);
		i++;
	}
	free(concat);
	hash_f = queue_del(q);
	free(q);
	return hash_f;
}

char *get_merkle_root(Transaction **transaction_list, unsigned long transaction_nb)
{
	if ( transaction_nb > 0 )
	{
		char **hash = calloc(transaction_nb, sizeof(char*));
		char *merkle_hash = malloc(65*sizeof(char));
		for ( unsigned long i = 0; i < transaction_nb; ++i )
			hash[i] = malloc(HASH_SIZE*sizeof(char));
		if ( !hash )
			error_report(116, ERR_MALLOC);
		Queue *q = generate_queue(transaction_list, transaction_nb, hash);
		merkle_hash = generate_merkle_hash(q);
		for ( unsigned long i = 0; i < transaction_nb; ++i )
			free(hash[i]);
		free(hash);
		return merkle_hash;
	}
	return "genesis";	
}

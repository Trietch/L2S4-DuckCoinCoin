#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "transaction.h"
#include "sha256_utils.h"
#include "defines.h"


/******************************************************************
 * STRUCT _ELEM (aka ELEM) *                                      *
 ***************************                                      *
 *                                                                *
 * 	data : Chaine de caractère contenant des hash                 *
 * 	next : Pointeur vers le prochain élément de la file           *
 *                                                                *
 ******************************************************************/

typedef struct _Elem
{
	char *data;
	struct _Elem *next;
} Elem;


/******************************************************************
 * STRUCT _QUEUE (aka QUEUE) *                                    *
 *****************************                                    *
 *                                                                *
 * 	first : Pointeur vers l'élément en tête de la file            *
 * 	size  : Nombre d'éléments de la file                          *
 *                                                                *
 ******************************************************************/

typedef struct _Queue
{
	struct _Elem *first;
	unsigned long size;
} Queue;


/******************************************************************
 * QUEUE_CREATE *                                                 *
 ****************                                                 *
 *	                                                              *
 *	Creer une file.                                               *
 *	                                                              *
 *  Return : Queue                                                *
 *                                                                *
 ******************************************************************/

Queue *queue_create()
{
	Queue *q = malloc(sizeof(struct _Queue));
	if ( !q )
		error_report(54, ERR_MALLOC);
	q->first = NULL;
	q->size = 0;
	return q;
}


/******************************************************************
 * QUEUE_ADD *                                                    *
 *************                                                    *
 *	                                                              *
 *	Ajoute la chaine de caractère (data) a la file (q).           *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void queue_add(Queue *q, char *data)
{
	Elem *e = malloc(sizeof(struct _Elem));
	if ( !e )
		error_report(76, ERR_MALLOC);
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


/******************************************************************
 * GENERATE_QUEUE *                                               *
 ******************                                               *
 *	                                                              *
 *	Genere une file avec la liste des transactions                *
 *	(transaction_list). La file ne peut etre que de taille paire. *
 *	                                                              *
 *  Return : Queue                                                *
 *                                                                *
 ******************************************************************/

Queue *generate_queue(Transaction **transaction_list, unsigned long transaction_nb, char **hash)
{
	Queue *q = queue_create();
	char *string = malloc(31*sizeof(char));
	if ( !string )
		error_report(111, ERR_MALLOC);
	for ( unsigned long i = 0; i < transaction_nb; ++i )
	{
		if ( !(hash[i]) )
			error_report(113, ERR_MALLOC);
		transaction_to_string(transaction_list[i], string);
		sha256ofString((BYTE *)string, hash[i]);
		queue_add(q, hash[i]);
	}
	if ( transaction_nb%2 == 1 )
		queue_add(q, hash[transaction_nb-1]);
	free(string);
	return q;
}


/******************************************************************
 * QUEUE_DEL *                                                    *
 *************                                                    *
 *	                                                              *
 *	Supprime l'élément en tête de la file (q) et le renvoie.      *
 *	                                                              *
 *  Return : char*                                                *
 *                                                                *
 ******************************************************************/

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


/******************************************************************
 * GENERATE_MERKLE_HASH *                                         *
 ************************                                         *
 *                                                                *
 * 	Genere le merkle hash de la file (q).                         *
 * 	Pour cela la fonction calcul le hash de la concaténation des  *
 * 	deux premiers éléments de la file et l'ajoute a la fin de la  *
 * 	file, jusqu'a que la file n'ai plus qu'un seul élément.       *
 *	                                                              *
 *  Return : char*                                                *
 *                                                                *
 ******************************************************************/

char *generate_merkle_hash(Queue *q)
{
	char **hash = malloc((q->size)*sizeof(char*));
	char *concat = malloc(HASH_SIZE*2*sizeof(char));
	char *a = malloc(HASH_SIZE*sizeof(char));
	char *b = malloc(HASH_SIZE*sizeof(char));
	char *hash_f = malloc(HASH_SIZE*sizeof(char));
	unsigned long i = 0;
	if ( !hash || !concat || !a || !b || !hash_f )
		error_report(161, ERR_MALLOC);
	while ( q->size > 1 )
	{	
		hash[i] = malloc(HASH_SIZE*sizeof(char));
		if ( !hash[i] )
			error_report(171, ERR_MALLOC);
		a = queue_del(q);
		b = queue_del(q);
		if ( sprintf(concat, "%s%s", a, b) < 1 )
			error_report(176, ERR_SPRINTF);
		sha256ofString((BYTE *)concat, hash[i]);
		queue_add(q, hash[i]);
		i++;
	}
	free(concat);
	hash_f = queue_del(q);
	free(q);
	return hash_f;
}


/******************************************************************
 * GET_MERKLE_ROOT *                                              *
 *******************                                              *
 *                                                                *
 *	Retourne le merkle hash de la liste des transactions          *
 *	(transaction_list).                                           *
 *	                                                              *
 *  Return : char*                                                *
 *                                                                *
 ******************************************************************/

char *get_merkle_root(Transaction **transaction_list, unsigned long transaction_nb)
{
	if ( transaction_nb > 0 )
	{
		char **hash = calloc(transaction_nb, sizeof(char*));
		char *merkle_hash = malloc(HASH_SIZE*sizeof(char));
		if ( !hash || !merkle_hash )
			error_report(204, ERR_MALLOC);
		for ( unsigned long i = 0; i < transaction_nb; ++i )
		{
			hash[i] = malloc(HASH_SIZE*sizeof(char));
			if ( !hash[i] )
				error_report(210, ERR_MALLOC);
		}
		Queue *q = generate_queue(transaction_list, transaction_nb, hash);
		merkle_hash = generate_merkle_hash(q);
		for ( unsigned long i = 0; i < transaction_nb; ++i )
			free(hash[i]);
		free(hash);
		return merkle_hash;
	}
	return "genesis";	
}

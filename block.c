#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "sha256_utils.h"
#include "transaction.h"
#include "defines.h"
#include "merkle_tree.h"


/******************************************************************
 * STRUCT _BLOCK (aka BLOCK) *                                    *
 *****************************                                    *
 *                                                                *
 * 	transaction_list    : Liste des transactions                  *
 * 	next                : Pointeur du prochain Block              *
 * 	prev_hash           : String du hash du Block précédent       *
 * 	hash                : String du hash du Block                 *
 * 	timestamp           : String de la date actuelle              *
 * 	index               : Numéro du Block                         *
 * 	nonce               : Nonce du Block (utilisé pour le minage) *
 * 	number_transactions : Nombre de transactions dans le Block    *
 *                                                                *
 ******************************************************************/

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


/******************************************************************
 * GETTIMESTAMP *                                                 *
 ****************                                                 *
 *	                                                              *
 *	Retourne une chaine de caractères contenant la date actuelle. *
 *	                                                              *
 *  Return : char*                                                *
 *                                                                *
 ******************************************************************/

char *getTimeStamp()
{
	char *str = malloc(128*sizeof(char));
	if ( !str )
		error_report(54, ERR_MALLOC);
	time_t t = time(NULL);
	struct tm *p = localtime(&t);
	strftime(str, 128, "%c", p);
	return str;
}


/******************************************************************
 * BLOCK_TO_STRING *                                              *
 *******************                                              *
 *	                                                              *
 *	Crée une chaine de caractères (string_block) en concatenant   *
 *	tout les champs du block (block).                             *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_to_string(Block *block, char *string_block)
{
	char *string = calloc(HASH_SIZE*3, sizeof(char));
	if ( !string )
		error_report(77, ERR_MALLOC);
	if ( sprintf(string+strlen(string), "%s%s%lu%s%lu%lu", block->prev_hash, block->hash_merkle_root, block->index, block->timestamp, block->nonce, block->number_transactions) < 1 )
		error_report(80, ERR_SPRINTF);
	strcpy(string_block, string);
	free(string);
}


/******************************************************************
 * BLOCK_CREATE *                                                 *
 ****************                                                 *
 *	                                                              *
 *	Initialise la structure de données Block avec les champs      *
 *	données en entrée et en en calculants (les hashs).            *
 *	                                                              *
 *  Return : Block*                                               *
 *                                                                *
 ******************************************************************/

Block *block_create(unsigned long index, char *prev_hash, Transaction **transaction_list, unsigned long transaction_nb)
{
	Block *block = calloc(1, sizeof(struct _Block));
	if ( !block )
		error_report(100, ERR_MALLOC);
	block->index = index;
	block->timestamp = getTimeStamp();
	block->prev_hash = prev_hash;
	block->nonce = 0;
	block->transaction_list = transaction_list;
	block->number_transactions = transaction_nb;
	block->hash_merkle_root = get_merkle_root(transaction_list, transaction_nb);
	block->hash = malloc(HASH_SIZE*sizeof(char));
	if ( !block->hash )
		error_report(110, ERR_MALLOC);
	return block;
}


/******************************************************************
 * BLOCK_SIGN_HASH *                                              *
 *******************                                              *
 *	                                                              *
 *	Met le hash (hash) dans le champ hash du block (b).           *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_sign_hash(Block *b, char *hash)
{
	b->hash = hash;
}


/******************************************************************
 * IS_GOOD_HASH *                                                 *
 ****************                                                 *
 *	                                                              *
 *	Verifie que le hash (hash) respecte bien la difficulté        * 
 *	(difficulty). La fonction verifie donc si il y a n            *
 *	(difficulty) charactère '0' d'affilé au debut du hash.        *
 *	                                                              *
 *  Return : bool                                                 *
 *                                                                *
 ******************************************************************/

bool is_good_hash(char *hash, unsigned long difficulty)
{
	for ( unsigned long i = 0; i < difficulty; ++i )
	{
		if ( hash[i] != '0' )
			return false;
	}
	return true;
}


/******************************************************************
 * GET_LAST_BLOCK *                                               *
 ******************                                               *
 *	                                                              *
 *	Retourne le premier block qui n'a pas de prochain block à     *
 *	partir du block (block).                                      *
 *	                                                              *
 *  Return : Block*                                               *
 *                                                                *
 ******************************************************************/

Block *get_last_block(Block *block)
{
	Block **p_block = &block;
	while ( *p_block )
	{
		if ( !(*p_block)->next )
			return *p_block;	
		p_block = &(*p_block)->next;
	}
	error_report(176, ERR_SHOULD_NOT_BE_HERE);
	return NULL;
}


/******************************************************************
 * BLOCK_ADD *                                                    *
 *************                                                    *
 *	                                                              *
 *	Ajoute un block au block (block), ce nouveau block ayant      *
 *	n (transaction_nb) transactions qui sont (transaction_list).  *
 *	Cherche une bonne nonce pour le block pour avoir un hash      *
 *	valide avec la difficulté (difficulty).                       *
 *	                                                              *
 *  Return : Block*                                               *
 *                                                                *
 ******************************************************************/

Block *block_add(Block *block, unsigned long difficulty, Transaction **transaction_list, unsigned long transaction_nb)
{
	Block **p_block = &block;
	Block *last_block = NULL;
	unsigned int i = 0;
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4));
	char *hash = malloc(HASH_SIZE*sizeof(char));
	if ( !hash || !string_block )
		error_report(199, ERR_MALLOC);
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
	free(string_block);
	if ( *p_block )
		return block;
	error_report(224, ERR_SHOULD_NOT_BE_HERE);
	return NULL;
}


/******************************************************************
 * IS_GENESIS *                                                   *
 **************                                                   *
 *	                                                              *
 *	La fonction regarde si le champ merkle hash du block (b) est  * 
 *	"genesis" ou pas.                                             *
 *	                                                              *
 *  Return : bool                                                 *
 *                                                                *
 ******************************************************************/

bool is_genesis(Block *b)
{
	return !strcmp(b->hash_merkle_root, "genesis");
}


/******************************************************************
 * IS_PREV_HASH_VALID *                                           *
 **********************                                           *
 *	                                                              *
 *	Compare le champ prev hash du prochain block du block (b)     * 
 *	avec le champ hash du block (b).                              *
 *	S'ils sont identiques, alors le hash est valide et la         *
 *	fonction retourne true.                                       *
 *	                                                              *
 *  Return : bool                                                 *
 *                                                                *
 ******************************************************************/

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


/******************************************************************
 * IS_MERKLE_ROOT_VALID *                                         *
 ************************                                         *
 *	                                                              *
 *	Compare le champ merkle hash du block (b) avec le merkle hash *
 *	que la fonction calcule du block (b).                         *
 *	S'ils sont identiques, alors le hash est valide et la         *
 *	fonction retourne true.                                       *
 *	                                                              *
 *  Return : bool                                                 *
 *                                                                *
 ******************************************************************/

bool is_merkle_root_valid(Block *b)
{
	if ( show_log )
		printf("Checking blocks merkle root.. ");
	Block *p_block = b;
	char *generated_merkle_root;
	while ( p_block )
	{
		generated_merkle_root = get_merkle_root(p_block->transaction_list, p_block->number_transactions);
		if ( strcmp(p_block->hash_merkle_root, generated_merkle_root) )
		{
			printf("%sNOT OK! >:(%s\n", BRED, WHI);
			return false;
		}
		p_block = p_block->next;
	}
	if ( generated_merkle_root )
		free(generated_merkle_root);
	if ( show_log )	
		printf("%sOK!%s\n", BGRN, WHI);
	return true;
}


/******************************************************************
 * IS_MY_HASH_VALID *                                             *
 ********************                                             *
 *	                                                              *
 *	Compare le champ hash du block (b) avec le hash que la        *
 *	fonction calcule du block (b). S'ils sont identiques, alors   *
 *	le hash est valide et la fonction retourne true.              *
 *	                                                              *
 *  Return : bool                                                 *
 *                                                                *
 ******************************************************************/

bool is_my_hash_valid(Block *b)
{
	if ( show_log )
		printf("Checking blocks hash.. ");
	Block *p_block = b;
	char *string_block = malloc((HASH_SIZE*sizeof(char))*2+(sizeof(unsigned long)*4)+50);
	char *hash = malloc(HASH_SIZE*sizeof(char));
	if ( !string_block || !hash )
		error_report(332, ERR_MALLOC);
	while ( p_block )
	{
		block_to_string(p_block, string_block);
		if ( !hash )
			error_report(340, ERR_MALLOC);
		sha256ofString((BYTE *)string_block, hash);
		if ( strcmp(p_block->hash, hash) )
		{
			printf("%sNOT OK! >:(%s\n", BRED, WHI);
			free(string_block);
			free(hash);
			return false;
		}
		p_block = p_block->next;
	}
	free(string_block);
	free(hash);
	if ( show_log )
		printf("%sOK!%s\n", BGRN, WHI);
	return true;
}


/******************************************************************
 * GET_ITH_BLOCK *                                                *
 *****************                                                *
 *	                                                              *
 *	Retourne le pointeur du n-ième (num) block a partir du block  *
 *	(b).                                                          *
 *	                                                              *
 *  Return : Block*                                               *
 *                                                                *
 ******************************************************************/

Block *get_ith_block(Block *b, unsigned long num)
{
	Block *p_block = b;
  	while ( p_block->index != num-1 )
	{
		p_block = p_block->next;
	}
	return p_block;	
}


/******************************************************************
 * BLOCK_DEL_ALL_TRANSACTION *                                    *
 *****************************                                    *
 *	                                                              *
 *	Supprime toutes les transactions du block (b).                *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_del_all_transaction(Block *b)
{
	for ( unsigned long i = 0; i <= b->number_transactions-1; ++i )
	{
		free(b->transaction_list[i]);
	}
	free(b->transaction_list);	
}


/******************************************************************
 * BLOCK_ALL_DELETE *                                             *
 ********************                                             *
 *	                                                              *
 *	Supprime tout les blocks a partir du block (b).               *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_all_delete(Block *b)
{
	if ( b->next )
	{
		block_all_delete(b->next);
	}
	free(b->hash);
	free(b->timestamp);
	if ( b->index != 0 )
	{
		block_del_all_transaction(b);
		free(b->hash_merkle_root);
	}
	free(b);
}


/******************************************************************
 * BLOCK_DEL_TRANSACTION *                                        *
 *************************                                        *
 *	                                                              *
 *	Supprime la n-ième (trans_nb) transactions du block (b).      *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_del_transaction(Block *b, unsigned long trans_nb)
{
	for ( unsigned long i = trans_nb; i < b->number_transactions-1; ++i )
	{
		b->transaction_list[i] = b->transaction_list[i+1];
	}
	(b->number_transactions)--;
	free(b->transaction_list[b->number_transactions]);
	b->hash_merkle_root = get_merkle_root(b->transaction_list, b->number_transactions);
}

/******************************************************************
 * BLOCK_JUMP *                                                   *
 **************                                                   *
 *	                                                              *
 *  Modifie le pointeur du prochain block du block (b) en le      *
 *  remplaçant par le prochain block du prochain block.	          *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

void block_jump_next(Block *b)
{
	b->next = b->next->next;
}


/******************************************************************
 * BLOCKCHAIN_REBUILD *                                           *
 **********************                                           *
 *                                                                *
 *  A partir du block (b), recalcule le hash et le précédent hash *
 *  des blocks suivants (lui compris) avec la difficulté donnée   *
 *  (difficulty). Il affiche aussi l'index du bloc qu'il vient de *
 *  trouver sur le nombre total de blocks (nb_blocks).            *
 *	                                                              *
 *  Return : void                                                 *
 *                                                                *
 ******************************************************************/

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
			if ( !hash || !string_block )
				error_report(483, ERR_MALLOC);
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
		if ( !string_block )
			error_report(503, ERR_MALLOC);
		(p_block->nonce)++;
		block_to_string(p_block, string_block);
		sha256ofString((BYTE *)string_block, hash);
	} while ( !is_good_hash(hash, difficulty) );	
	strcpy(p_block->hash, hash);
}


/******************************************************************
 * BLOCK_PRINT *                                                  *
 ***************                                                  *
 *                                                                *
 *	Affiche les champs de tout les blocks à partir du block en    *
 *	entrée (block).                                               *
 *	                                                              *
 *	Return : void                                                 *
 *                                                                *
 ******************************************************************/

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


/******************************************************************
 * EXPORT_BLOCK *                                                 *
 ****************                                                 *
 *                                                                *
 *	Convertis les n (nb_blocks) blocks (block) en JSON et         *
 *	l'écrit dans le fichier (file).                               *
 *	                                                              *
 *	Return : void                                                 *
 *                                                                *
 ******************************************************************/

void export_block(FILE *file, Block *block, unsigned long nb_block)
{
	Block *b;
	char *string = malloc(31*sizeof(char));
	if ( !string )
		error_report(569, ERR_MALLOC);
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
			transaction_to_string(b->transaction_list[j], string);
			fprintf(file, "\"%s\"", string);
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
	free(string);
}


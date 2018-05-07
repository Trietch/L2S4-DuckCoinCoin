#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "blockchain.h"
#include "block.h"


/******************************************************************
 * CHECK_VALIDITY *                                               *
 ******************                                               *
 *	                                                              *
 *  Verifie la validité des blocks de la blockchain (b).          *
 *  ( Verifie la présence d'un block genesis, que les hash        *
 *  soient valides, que les précédent hash aussi et de même       *
 *  pour le merkle root.                                          *
 *	                                                              *
 *  Return : bool                                                 *
 *                                                                *
 ******************************************************************/

bool check_validity(Blockchain *b)
{
	Block *block = get_first_block(b);
	if ( !is_genesis(block) ||
		 !is_my_hash_valid(block) ||
		 !is_prev_hash_valid(block) ||
		 !is_merkle_root_valid(block) )
		return false;
	return true;
}

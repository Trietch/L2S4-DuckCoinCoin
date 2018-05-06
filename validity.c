#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "blockchain.h"
#include "block.h"

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

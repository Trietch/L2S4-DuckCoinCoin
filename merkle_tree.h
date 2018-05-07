#ifndef _MERKLE_TREE
#define _MERKLE_TREE

typedef struct _Element Element;
typedef struct _Queue Queue;

char *get_merkle_root(Transaction **transaction_list, unsigned long transaction_nb);

#endif

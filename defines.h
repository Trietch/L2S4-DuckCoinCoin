#ifndef _DEFINES
#define _DEFINES

#ifdef MAIN
	int show_log;
#else
	extern int show_log;
#endif

#include "error.h"

#define TRANSACTIONS_MIN		1
#define TRANSACTIONS_MAX		10
#define HASH_SIZE 				65
#define RANDOM_MAX_TRANSACTIONS 4294967295 
#define JSON_NAME 				"blockchain.json"
#define JSON_TEMP 				"temp.json"

#define ERR_NO					1
#define ERR_SPRINTF				2
#define ERR_MALLOC				4
#define ERR_SHOULD_NOT_BE_HERE	8
#define ERR_FILE_OPEN			16
#define ERR_FILE_CLOSE			32
#define ERR_FGETS 				64	



#define YEL						"\033[33m"
#define RED						"\033[31m"
#define GRN						"\033[32m"
#define WHI						"\033[0m"

#define BYEL					"\033[1m\033[33m"
#define BGRN 					"\033[1m\033[32m"
#define BRED 					"\033[1m\033[31m"
#define BBLU					"\033[1m\033[34m"

#endif

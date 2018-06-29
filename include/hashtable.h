#pragma once
#include <stdbool.h>

#define HASH_STEP 3

typedef char TKey;
typedef void* TElem;

typedef struct Record {
	TKey key;
	TElem* value;
	int countValues;
} Record;

typedef struct HashTable {
	struct Record** recs;
	int memSize;
	int dataCount;
	int currPos;
} HashTable;

unsigned long HashFunc(const TKey key);

void InitTab(HashTable* ht, unsigned int size);
void ClearTable(HashTable* ht);

bool IsTabEmpty(const HashTable* ht);
bool IsTabFull(const HashTable* ht);

TElem* FindRecordTab(HashTable* ht, TKey k);
void InsRecordTab(HashTable* ht, TKey k, TElem val); 
void DelRecordTab(HashTable* ht, TKey k);


#pragma once
#include <stdbool.h>

#define HASH_STEP 3

typedef char TKey;
typedef char* TElem;

struct Record
{
	TKey key;
	TElem* value;
	int countValues;
};

struct HashTable
{
	struct Record** recs;
	int memSize;
	int dataCount;
	int currPos;
};

unsigned long HashFunc(const TKey key);

void InitTab(struct HashTable* ht, unsigned int size);
void InitInsert(struct HashTable* ht, TKey k);
void ClearTable(struct HashTable* ht);

bool IsTabEmpty(struct HashTable* ht);
bool IsTabFull(struct HashTable* ht);

TElem* FindRecordTab(struct HashTable* ht, TKey k);
void InsRecordTab(struct HashTable* ht, TKey k, TElem val); //The insert function is modified
void DelRecordTab(struct HashTable* ht, TKey k);

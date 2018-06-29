#define _CRT_SECURE_NO_WARNINGS

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

unsigned long HashFunc(const TKey key) {
	unsigned long hashval = 0;
	int tmp = (int)key;

	while (tmp != 0) {
		hashval = (hashval << 3) + tmp;
		tmp = tmp / 10;
	}
	return hashval;
}

void InitTab(HashTable* ht, unsigned int size) {
		ht->recs = calloc(size, sizeof(struct Record*));
		for (int i = 0; i < size; i++)
			ht->recs[i] = NULL;

		ht->memSize = size;
		ht->dataCount = 0;
}

void ClearTable(HashTable * ht) {
	if (ht->recs != NULL) {
		for (int i = 0; i < ht->memSize; i++) {
			if (ht->recs[i] != NULL) {
				int count = ht->recs[i]->countValues;
				for (int i = 0; i < count; i++)
					if (ht->recs[i]->value[i] != NULL) {
						free(ht->recs[i]->value[i]);
						ht->recs[i]->countValues = 0;
						ht->recs[i]->value = NULL;
					}
				free(ht->recs[i]);
				ht->recs[i] = NULL;
			}
		}
		free(ht->recs);

		ht->recs = NULL;
	}
	ht->memSize = 0;
	ht->dataCount = 0;
}

bool IsTabEmpty(const HashTable* ht) {
	return ht->dataCount == 0;
}

bool IsTabFull(const HashTable* ht) {
	return ht->dataCount == ht->memSize;
}

TElem* FindRecordTab(HashTable* ht, TKey k) {
	if ((ht->recs == NULL) | IsTabEmpty(ht)) return NULL;

	ht->currPos = HashFunc(k) % ht->memSize;
	for (int i = 0; i < ht->memSize; i++)
	{
		if (ht->recs[ht->currPos] == NULL) break;
		else if (ht->recs[ht->currPos]->key == k) return ht->recs[ht->currPos]->value;

		ht->currPos = (ht->currPos + HASH_STEP) % ht->memSize;;
	}
	return NULL;
}

void InsRecordTab(HashTable* ht, TKey k, TElem val) {
	if (ht->recs == NULL) return;

	ht->currPos = HashFunc(k) % ht->memSize;

	for (int i = 0; i < ht->memSize; i++)
	{
		if (ht->recs[ht->currPos] != NULL && ht->recs[ht->currPos]->key == k)
		{
			TElem* tmp = ht->recs[ht->currPos]->value;

			ht->recs[ht->currPos]->value= (TElem*)calloc(ht->recs[ht->currPos]->countValues + 1, sizeof(TElem));
			memcpy(ht->recs[ht->currPos]->value, tmp, ht->recs[ht->currPos]->countValues*sizeof(TElem));
			ht->recs[ht->currPos]->value[ht->recs[ht->currPos]->countValues] = val;

			free(tmp);
			ht->recs[ht->currPos]->countValues++;
			break;
		}
		else if (ht->recs[ht->currPos] == NULL)
		{
			ht->recs[ht->currPos] = (Record*)calloc(1, sizeof(struct Record));
			ht->recs[ht->currPos]->key = k;

			ht->recs[ht->currPos]->value = (Record*)calloc(1, sizeof(TElem));
			ht->recs[ht->currPos]->value[0] = val;
			ht->recs[ht->currPos]->countValues = 1;
			ht->dataCount++;
			break;
		}
		ht->currPos = (ht->currPos + HASH_STEP) % ht->memSize;
	}
}
void DelRecordTab(HashTable* ht, TKey k) {
	void* tmp = FindRecordTab(ht, k);
	if (tmp == NULL) return;

	for (int i = 0; i < ht->recs[ht->currPos]->countValues; i++)
		free(ht->recs[ht->currPos]->value[i]);

	free(ht->recs[ht->currPos]->value);
	free(ht->recs[ht->currPos]);
	ht->recs[ht->currPos] = NULL;
	ht->dataCount--;
}
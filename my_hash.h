#include "my_linkedlist.h"

typedef struct tagHashTable_Item
{
    //char *key;
    //char *val;
    int key;
    int val;
}HASHTABLE_ITEM;

typedef struct tagHashTable{
    HASHTABLE_ITEM **pHashTableItem;
    LINKEDLIST_SINGLE **pOverflowBuckets;
    int size; //size of table
    int count; //count of element
}HASHTABLE;

HASHTABLE *newHashTable(int size);
void InsertHashTable(HASHTABLE *pTable, int key, int val);
void printHashTable(HASHTABLE *pTable);
void deleteHashTable(HASHTABLE *pTable);
void HighestFrequnceItem(HASHTABLE *pHashTable,int *maxFrequence, int *key);
int Hash_function(int key,int size);
void InsertHashTableWithReplacePolicy(HASHTABLE *pTable,HASHTABLE_ITEM **ppNewItem );
HASHTABLE_ITEM * GetHashItemFromHashTable(HASHTABLE *pTable, int key, int val);
HASHTABLE_ITEM *NewHashItem(int key, int val);
void DeleteHashItem(HASHTABLE_ITEM *pItem);
void RemoveHashItemFromHashTable(HASHTABLE *pTable, HASHTABLE_ITEM *pItem);
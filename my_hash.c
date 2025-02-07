#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "assert.h"
#include "my_linkedlist.h"
#include "my_hash.h"
#define INT_AS_KEY
#ifdef INT_AS_KEY

HASHTABLE_ITEM *NewHashItem(int key, int val){
    HASHTABLE_ITEM *pItem;

    pItem =(HASHTABLE_ITEM *) malloc(sizeof(HASHTABLE_ITEM));
    assert(pItem);
    
    pItem->key = key;
    pItem->val = val;
    return pItem;
}

void DeleteHashItem(HASHTABLE_ITEM *pItem){
    if(pItem!=NULL){
        free(pItem);
    }
    return;
}
int Hash_function(int key,int size){
    return ((key>=0?key:-key)%size);
}

void PrintHashItem(HASHTABLE_ITEM *pItem){
    if(pItem==NULL){
        return;
    }
    printf("key %4d: frequnce %4d\r\n",pItem->key,pItem->val);
    return;
}


bool isSame(HASHTABLE_ITEM *pItem1, HASHTABLE_ITEM *pItem2){
    return ( (pItem1->key == pItem2->key) ); 
}
#endif

HASHTABLE *newHashTable(int size){
    HASHTABLE *pTable;
    pTable = (HASHTABLE *)malloc(sizeof(HASHTABLE));
    pTable->size = size;
    pTable->count = 0;
    pTable->pHashTableItem = (HASHTABLE_ITEM **)malloc(sizeof(HASHTABLE_ITEM *)*size);
    pTable->pOverflowBuckets = (LINKEDLIST_SINGLE **)malloc(sizeof(LINKEDLIST_SINGLE *)*size);
    for(int index = 0; index<size; index++){
        pTable->pHashTableItem[index] = NULL;
        pTable->pOverflowBuckets[index] = NULL;
    }
    return pTable;
}
void deleteHashTable(HASHTABLE *pTable){
    HASHTABLE_ITEM *pHashTableItem;
    LINKEDLIST_SINGLE *pOverflowBucketsHead,*pOverflowBucketsNode;
    if(pTable==NULL){
        return ;
    }
    //Free Hash array    
    for(int index=0;index<pTable->size;index++){
        pHashTableItem = pTable->pHashTableItem[index];
        DeleteHashItem(pHashTableItem);
    }
    free(pTable->pHashTableItem);
    
    // Free overflow buckets
    for(int index=0;index<pTable->size;index++){
        pOverflowBucketsHead = pTable->pOverflowBuckets[index];
        while(pOverflowBucketsHead){
            pOverflowBucketsNode = pOverflowBucketsHead->pNext;
            if(pOverflowBucketsHead->pval){
                DeleteHashItem(pOverflowBucketsHead->pval);
                free(pOverflowBucketsHead);
            }
            pOverflowBucketsHead = pOverflowBucketsNode;
        }      
    }
    free(pTable->pOverflowBuckets);
    free(pTable);
}
void printHashTable(HASHTABLE *pTable){
    HASHTABLE_ITEM *pHashTableItem;
    LINKEDLIST_SINGLE *pOverflowBucketsHead,*pOverflowBucketsNode;
    if(pTable==NULL){
        return ;
    }
        
    for(int index=0;index<pTable->size;index++){
        pHashTableItem = pTable->pHashTableItem[index];
        PrintHashItem(pHashTableItem);
    }
        
    for(int index=0;index<pTable->size;index++){
        pOverflowBucketsHead = pTable->pOverflowBuckets[index];
        while(pOverflowBucketsHead){
            pOverflowBucketsNode = pOverflowBucketsHead->pNext;
            if(pOverflowBucketsHead->pval){
                PrintHashItem(pOverflowBucketsHead->pval);
            }
            pOverflowBucketsHead = pOverflowBucketsNode;
        }      
    }
}
LINKEDLIST_SINGLE *newOverflowBucketsItem(HASHTABLE_ITEM *pItem){
    LINKEDLIST_SINGLE *pNode;
    pNode = (LINKEDLIST_SINGLE *)newLinkedListSingleNode();
    pNode ->pval = pItem;
    pNode->pNext = NULL;
    return pNode;
}


bool isExistedinBuckets(LINKEDLIST_SINGLE *pOverflowBuckets,HASHTABLE_ITEM *pItem, HASHTABLE_ITEM **ppExistedItem){
    bool isPairExisted = false;
    LINKEDLIST_SINGLE *pCurrent = pOverflowBuckets;
    while(pCurrent!=NULL && !isPairExisted){
        isPairExisted = isSame(pCurrent->pval,pItem);
        if(isPairExisted==true){
            *ppExistedItem = pCurrent->pval;
            return true;
        }
        pCurrent = pCurrent->pNext;
    }
    *ppExistedItem = NULL;
    return false;
}

void InsertHashTableWithReplacePolicy(HASHTABLE *pTable,HASHTABLE_ITEM **ppNewItem ){
    if(pTable == NULL){
        return;
    }
    
    HASHTABLE_ITEM *pItem,*pNewItem,*pExistedItem;
    LINKEDLIST_SINGLE *pBucketItem;
    int index;
    pNewItem = *ppNewItem;
    index = Hash_function(pNewItem->key,pTable->size);

    pItem = pTable->pHashTableItem[index];
    if(pTable->pHashTableItem[index]==NULL){ // new element
        pTable->pHashTableItem[index] = pNewItem;
        pTable->count++;
    }
    else{ //collision happened. append to the head linked list
        if(isSame(pItem,pNewItem)){
            pItem->val = pNewItem->val ; //same key but different value, replace with new value 
            //the key has already been there -scenario 1
            DeleteHashItem(pNewItem);
            *ppNewItem = pItem;
        }
        else if(isExistedinBuckets(pTable->pOverflowBuckets[index],pNewItem, &pExistedItem)){
            pExistedItem->val = pNewItem->val;//replace the value
            //the pair (key,val) has already been there -scenario 2
            DeleteHashItem(pNewItem);
            *ppNewItem = pExistedItem;
        }
        else{ // new element : insert to buckets
            pBucketItem = newOverflowBucketsItem(pNewItem);
            if(pTable->pOverflowBuckets[index]==NULL){
                pTable->pOverflowBuckets[index] = pBucketItem;
            }
            else{
                pBucketItem->pNext = pTable->pOverflowBuckets[index] ;
                pTable->pOverflowBuckets[index] = pBucketItem;
            }
            pTable->count++;
        }    
    }
}


void InsertHashTable(HASHTABLE *pTable, int key, int val){
    if(pTable == NULL){
        return;
    }
    HASHTABLE_ITEM *pItem,*pNewItem,*pExistedItem;
    LINKEDLIST_SINGLE *pBucketItem;
    int index;

    pNewItem = NewHashItem(key,val);

    index = Hash_function(key,pTable->size);
    pItem = pTable->pHashTableItem[index];
    if(pTable->pHashTableItem[index]==NULL){ // first element to be inserted in this position
        pTable->pHashTableItem[index] = pNewItem;
        pTable->count++;
    }
    else{ //collision happened. append to the head linked list
        if(isSame(pItem,pNewItem)){
            pItem->val ++ ;
            //the pair (key,val) has already been there -scenario 1
            DeleteHashItem(pNewItem);
        }
        else if(isExistedinBuckets(pTable->pOverflowBuckets[index],pNewItem, &pExistedItem)){
            pExistedItem->val ++ ; //reference;
            //the pair (key,val) has already been there -scenario 2
            DeleteHashItem(pNewItem);
        }
        else{ // insert to buckets
            pBucketItem = newOverflowBucketsItem(pNewItem);
            if(pTable->pOverflowBuckets[index]==NULL){
                pTable->pOverflowBuckets[index] = pBucketItem;
            }
            else{
                pBucketItem->pNext = pTable->pOverflowBuckets[index] ;
                pTable->pOverflowBuckets[index] = pBucketItem;
            }
            pTable->count++;
        }
        
    }
}
/* remove from the overflow buckets, don't free the item itself
 */
bool RemoveHashItemFromOverflowBuckets(LINKEDLIST_SINGLE **ppOverflowBuckets,HASHTABLE_ITEM *pItem){
    LINKEDLIST_SINGLE *pCurr,*pPrev;

    pCurr = *ppOverflowBuckets;
    pPrev = *ppOverflowBuckets;

    while(pCurr!=NULL){
        if(isSame(pCurr->pval,pItem)){
            
            if(pPrev!=pCurr){
                pPrev->pNext = pCurr->pNext;
            }
            else{
                *ppOverflowBuckets = pCurr->pNext;
            }
            free(pCurr); //free the node, don't free the hash item
            return true;
        }
        pPrev = pCurr;
        pCurr = pCurr->pNext;
    }
    return false;
}
/* remove one item from hash table, don't free the item itself
 * if it isn't in hash table, do nothing
 * if it is at overflow buckets, remove it
 * if it is at hash table, with an overflow buckets
 *      remove from hash table, move one item from overflow buckets to hash table
 * if it is at hash table, without an overflow buckets
 *      remove from hash table
 */
void RemoveHashItemFromHashTable(HASHTABLE *pTable, HASHTABLE_ITEM *pItem){
    if(pTable == NULL || pItem == NULL){
        return;
    }
    bool bRemoveFromOverflowBuckets = false;
    HASHTABLE_ITEM *pCurrItem, *pFirstItemInOverflowBuckets;
    
    
    int index = Hash_function(pItem->key,pTable->size);
    pCurrItem = pTable->pHashTableItem[index];

    if(pCurrItem!=NULL){ 
        if(isSame(pItem,pCurrItem)){
            if(pTable->pOverflowBuckets[index]!=NULL){ //remove the first item of overflow buckets to hash table
                pFirstItemInOverflowBuckets = pTable->pOverflowBuckets[index]->pval;                
                bRemoveFromOverflowBuckets = RemoveHashItemFromOverflowBuckets(&(pTable->pOverflowBuckets[index]),pFirstItemInOverflowBuckets);
                
                pTable->pHashTableItem[index] = pFirstItemInOverflowBuckets;
            }
            else{ //overflow buckets is null, remove from hash table only
                pTable->pHashTableItem[index] = NULL;
            }    
        }
        else{
            bRemoveFromOverflowBuckets =RemoveHashItemFromOverflowBuckets(&(pTable->pOverflowBuckets[index]),pItem);
        }
    }
    return;
}

HASHTABLE_ITEM * GetHashItemFromHashTable(HASHTABLE *pTable, int key, int val){
    if(pTable == NULL){
        return NULL;
    }
    HASHTABLE_ITEM *pItem,*pNewItem,*pExistedItem;
    int index = Hash_function(key,pTable->size);
    pNewItem = NewHashItem(key,val);

    pItem = pTable->pHashTableItem[index];
    if(pTable->pHashTableItem[index]!=NULL){ 
        if(isSame(pItem,pNewItem)){
            DeleteHashItem(pNewItem);
            return pItem;
        }
    }
    if(isExistedinBuckets(pTable->pOverflowBuckets[index],pNewItem, &pExistedItem)){
        DeleteHashItem(pNewItem);
        return pExistedItem;   
    }
    return NULL;
}
void GetMaxAndIndex(HASHTABLE_ITEM *pHashTableItem, int *maxFrequence, int *key){
    if(pHashTableItem!=NULL){
        if(*maxFrequence < pHashTableItem->val){
                *maxFrequence = pHashTableItem->val;
                *key = pHashTableItem->key;
        }
    }
    return;
}
void HighestFrequnceItem(HASHTABLE *pHashTable,int *maxFrequence, int *key){
    HASHTABLE_ITEM *pHashTableItem;
    LINKEDLIST_SINGLE *pOverflowBucketsHead,*pOverflowBucketsNode;
    
    if(pHashTable==NULL){
        return ;
    }
        
    for(int index=0;index<pHashTable->size;index++){
        pHashTableItem = pHashTable->pHashTableItem[index];
        
        if(pHashTableItem!=NULL){
            GetMaxAndIndex(pHashTableItem,maxFrequence,key);
        }
    }
        
    for(int index=0;index<pHashTable->size;index++){
        pOverflowBucketsHead = pHashTable->pOverflowBuckets[index];
        while(pOverflowBucketsHead){
            pOverflowBucketsNode = pOverflowBucketsHead->pNext;
            if(pOverflowBucketsHead->pval){
                GetMaxAndIndex(pOverflowBucketsHead->pval,maxFrequence,key);
            }
            pOverflowBucketsHead = pOverflowBucketsNode;
        }      
    }
}
/******/
/*
#ifdef STRING_AS_KEY
HASHTABLE_ITEM *NewHashItem(char *key, char *val){
    HASHTABLE_ITEM *pItem;
    int keylength,vallength;
    pItem = malloc(sizeof(HASHTABLE_ITEM));
    assert(pItem);
    
    keylength = strlen(key);
    vallength = strlen(val);

    pItem->key = (char *)malloc(keylength+1);
    pItem->val = (char *)malloc(vallength+1);
    strcpy(pItem->key,key);
    strcpy(pItem->val,val);
    return pItem;
}

void DeleteHashItem(HASHTABLE_ITEM *pItem){
    if(pItem!=NULL){
        if(pItem->key!=NULL){
            free(pItem->key);
        }
        if(pItem->val!=NULL){
            free(pItem->val);
        }
        free(pItem);
    }
    return;
}
bool isSame(HASHTABLE_ITEM *pItem1, HASHTABLE_ITEM *pItem2){
    int i= strncmp(pItem1->key,pItem2->key,strlen(pItem2->key));
    int j = strncmp(pItem1->val,pItem2->val,strlen(pItem2->val));
    if((i==0) && (j==0) ){
        return true;
    }
    return false;
}

void InsertHashTable(HASHTABLE *pTable, char *key, char *val){
    if(pTable == NULL|| key == NULL || val == NULL){
        return;
    }
    HASHTABLE_ITEM *pItem,*pNewItem;
    LINKEDLIST_SINGLE *pBucketItem;
    int index;

    pNewItem = NewHashItem(key,val);

    index = Hash_function(key);
    pItem = pTable->pHashTableItem[index];
    if(pTable->pHashTableItem[index]==NULL){ // first element to be inserted in this position
        pTable->pHashTableItem[index] = pNewItem;
        pTable->count++;
    }
    else{ //collision happened. append to the head linked list
        if(isSame(pNewItem,pItem)){
            //the pair (key,val) has already been there -scenario 1
            DeleteHashItem(pNewItem);
        }
        else if(isExistedinBuckets(pTable->pOverflowBuckets[index],pNewItem)){
            //the pair (key,val) has already been there -scenario 2
            DeleteHashItem(pNewItem);
        }
        else{ // insert to buckets
            pBucketItem = newOverflowBucketsItem(pItem);
            if(pTable->pOverflowBuckets[index]==NULL){
                pTable->pOverflowBuckets[index] = pBucketItem;
            }
            else{
                pBucketItem->pNext = pTable->pOverflowBuckets[index] 
                pTable->pOverflowBuckets[index] = pBucketItem;
            }
            pTable->count++;
        }
        
    }
}
#endif

 */

/*
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 50000 // Size of the HashTable.

unsigned long hash_function(char *str)
{
    unsigned long i = 0;

    for (int j = 0; str[j]; j++)
        i += str[j];

    return i % CAPACITY;
}

// Defines the HashTable item.
typedef struct Ht_item
{
    char *key;
    char *value;
} Ht_item;

// Defines the LinkedList.
typedef struct LinkedList
{
    Ht_item *item;
    LinkedList *next;
} LinkedList;

// Defines the HashTable.
typedef struct HashTable
{
    // Contains an array of pointers to items.
    Ht_item **items;
    LinkedList **overflow_buckets;
    int size;
    int count;
} HashTable;

LinkedList *allocate_list()
{
    // Allocates memory for a LinkedList pointer.
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    return list;
}

LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item)
{
    // Inserts the item onto the LinkedList.
    if (!list)
    {
        LinkedList *head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }
    else if (list->next == NULL)
    {
        LinkedList *node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList *temp = list;

    while (temp->next->next)
    {
        temp = temp->next;
    }

    LinkedList *node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;
}

Ht_item *linkedlist_remove(LinkedList *list)
{
    // Removes the head from the LinkedList.
    // Returns the item of the popped element.
    if (!list)
        return NULL;

    if (!list->next)
        return NULL;

    LinkedList *node = list->next;
    LinkedList *temp = list;
    temp->next = NULL;
    list = node;
    Ht_item *it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    free(temp->item->key);
    free(temp->item->value);
    free(temp->item);
    free(temp);
    return it;
}

void free_linkedlist(LinkedList *list)
{
    LinkedList *temp = list;

    while (list)
    {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

LinkedList **create_overflow_buckets(HashTable *table)
{
    // Create the overflow buckets; an array of LinkedLists.
    LinkedList **buckets = (LinkedList **)calloc(table->size, sizeof(LinkedList *));

    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;

    return buckets;
}

void free_overflow_buckets(HashTable *table)
{
    // Free all the overflow bucket lists.
    LinkedList **buckets = table->overflow_buckets;

    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);

    free(buckets);
}

Ht_item *create_item(char *key, char *value)
{
    // Creates a pointer to a new HashTable item.
    Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
    item->key = (char *)malloc(strlen(key) + 1);
    item->value = (char *)malloc(strlen(value) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

HashTable *create_table(int size)
{
    // Creates a new HashTable.
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item **)calloc(table->size, sizeof(Ht_item *));

    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;

    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item *item)
{
    // Frees an item.
    free(item->key);
    free(item->value);
    free(item);
}

void free_table(HashTable *table)
{
    // Frees the table.
    for (int i = 0; i < table->size; i++)
    {
        Ht_item *item = table->items[i];

        if (item != NULL)
            free_item(item);
    }

    // Free the overflow bucket lists and its items.
    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable *table, unsigned long index, Ht_item *item)
{
    LinkedList *head = table->overflow_buckets[index];

    if (head == NULL)
    {
        // Creates the list.
        head = allocate_list();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else
    {
        // Insert to the list.
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable *table, char *key, char *value)
{
    // Creates the item.
    Ht_item *item = create_item(key, value);

    // Computes the index.
    int index = hash_function(key);

    Ht_item *current_item = table->items[index];

    if (current_item == NULL)
    {
        // Key does not exist.
        if (table->count == table->size)
        {
            // HashTable is full.
            printf("Insert Error: Hash Table is full\n");
            free_item(item);
            return;
        }

        // Insert directly.
        table->items[index] = item;
        table->count++;
    }
    else
    {
        // Scenario 1: Update the value.
        if (strcmp(current_item->key, key) == 0)
        {
            strcpy(table->items[index]->value, value);
            return;
        }
        else
        {
            // Scenario 2: Handle the collision.
            handle_collision(table, index, item);
            return;
        }
    }
}

char *ht_search(HashTable *table, char *key)
{
    // Searches for the key in the HashTable.
    // Returns NULL if it doesn't exist.
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    // Provide only non-NULL values.
    if (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return item->value;

        if (head == NULL)
            return NULL;

        item = head->item;
        head = head->next;
    }

    return NULL;
}

void ht_delete(HashTable *table, char *key)
{
    // Deletes an item from the table.
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    if (item == NULL)
    {
        // Does not exist.
        return;
    }
    else
    {
        if (head == NULL && strcmp(item->key, key) == 0)
        {
            // Collision chain does not exist.
            // Remove the item.
            // Set table index to NULL.
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            // Collision chain exists.
            if (strcmp(item->key, key) == 0)
            {
                // Remove this item.
                // Set the head of the list as the new item.
                free_item(item);
                LinkedList *node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList *curr = head;
            LinkedList *prev = NULL;

            while (curr)
            {
                if (strcmp(curr->item->key, key) == 0)
                {
                    if (prev == NULL)
                    {
                        // First element of the chain.
                        // Remove the chain.
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else
                    {
                        // This is somewhere in the chain.
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }

                curr = curr->next;
                prev = curr;
            }
        }
    }
}

void print_search(HashTable *table, char *key)
{
    char *val;

    if ((val = ht_search(table, key)) == NULL)
    {
        printf("Key:%s does not exist\n", key);
        return;
    }
    else
    {
        printf("Key:%s, Value:%s\n", key, val);
    }
}

void print_table(HashTable *table)
{
    printf("\nHash Table\n-------------------\n");

    for (int i = 0; i < table -> size; i++)
    {
        if (table -> items[i])
        {
            printf("Index:%d, Key:%s, Value:%s\n", i, table -> items[i] -> key, table -> items[i] -> value);
        }
    }

    printf("-------------------\n\n");
}

int main()
{
    HashTable *ht = create_table(CAPACITY);
    ht_insert(ht, (char *)"1", (char *)"First address");
    ht_insert(ht, (char *)"2", (char *)"Second address");
    ht_insert(ht, (char *)"Hel", (char *)"Third address");
    ht_insert(ht, (char *)"Cau", (char *)"Fourth address");
    print_search(ht, (char *)"1");
    print_search(ht, (char *)"2");
    print_search(ht, (char *)"3");
    print_search(ht, (char *)"Hel");
    print_search(ht, (char *)"Cau"); // Collision!
    print_table(ht);
    ht_delete(ht, (char *)"1");
    ht_delete(ht, (char *)"Cau");
    print_table(ht);
    free_table(ht);
    return 0;
}
#endif
 */
#include "stdlib.h"
#include "my_linkedlist.h"
#include "my_hash.h"

LINKEDLIST_SINGLE *newLinkedListSingleNode(){
    LINKEDLIST_SINGLE *pNode;
    pNode = (LINKEDLIST_SINGLE *)malloc(sizeof(LINKEDLIST_SINGLE));
    pNode ->pval = NULL;
    pNode ->pNext = NULL;
    return pNode;
}

LINKEDLIST_DOUBLE *newLinkedListDoubleNode(){
    LINKEDLIST_DOUBLE *pNode;
    pNode = (LINKEDLIST_DOUBLE *)malloc(sizeof(LINKEDLIST_DOUBLE));
    pNode ->pval = NULL;
    pNode ->pNext = NULL;
    pNode ->pPrev = NULL;
    return pNode;
}

//insert pNew before pCurr
void InsertLinkedListDoubleNode(LINKEDLIST_DOUBLE *pCurr, LINKEDLIST_DOUBLE *pNew){ 
    if((pCurr==NULL) || (pNew==NULL)){
        return;
    }

    pNew->pNext = pCurr;
    pNew->pPrev = pCurr->pPrev;

    (pCurr->pPrev)->pNext = pNew;
    pCurr->pPrev = pNew;
    return;
}

//Remove a node (pCurr) from the double linked list. maybe affect its head (ppHead)
void RemoveLinkedListDoubleNode(LINKEDLIST_DOUBLE *pCurr,LINKEDLIST_DOUBLE **ppHead){
    LINKEDLIST_DOUBLE *pPrev, *pNext;
    if(pCurr==NULL){
        return;
    }
    //assert(*ppHead!=NULL);

    pPrev = pCurr->pPrev;
    pNext = pCurr->pNext;
    if(pNext == pCurr){ 
        *ppHead = NULL;
        return;
    }

    
    if(pNext!=NULL){ 
        pNext->pPrev = pPrev;
    }
    if(pPrev!=NULL){ 
        pPrev->pNext = pNext;
    }
    
    if(*ppHead==pCurr){
        *ppHead = pNext;
    }

    return;
}
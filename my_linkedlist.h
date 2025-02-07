#ifndef _MY_LINKEDLIST_H
#define _MY_LINKEDLIST_H

typedef struct tagLinkedListSingle{
    void *pval;
    struct tagLinkedListSingle *pNext;
} LINKEDLIST_SINGLE;

LINKEDLIST_SINGLE *newLinkedListSingleNode();

typedef struct tagLinkedListDouble{
    void *pval;
    struct tagLinkedListDouble *pNext;
    struct tagLinkedListDouble *pPrev;
} LINKEDLIST_DOUBLE;

LINKEDLIST_DOUBLE *newLinkedListDoubleNode();
//insert pNew befopre pCurr
void InsertLinkedListDoubleNode(LINKEDLIST_DOUBLE *pCurr, LINKEDLIST_DOUBLE *pNew);
//Remove a node (pCurr) from the double linked list. maybe affect its head (ppHead)
void RemoveLinkedListDoubleNode(LINKEDLIST_DOUBLE *pCurr,LINKEDLIST_DOUBLE **ppHead);
#endif
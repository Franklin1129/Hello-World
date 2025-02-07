#include "stddef.h"
#include "stdlib.h"
#include "my_queue.h"
/* as a FIFO queue, will allocate a new node and enter the tail of queue
 * Parameters:
 *  ppHead: always point to the head of queue
 *  ppTail: always point to the tail of queue
 *  pVal: thge value to be enter the tail of queue
 */


void EnQueue(struct Queue **ppHead,struct Queue **ppTail, void *pVal){
    struct Queue *pTemp;
    
    pTemp = (struct Queue *)malloc(sizeof(struct Queue));
    pTemp->pVal = pVal;
    pTemp->pNext = NULL;
    
    
    if(*ppHead == NULL){//first elemnet    
        *ppHead=pTemp;
        *ppTail=pTemp;
    }
    else{
        (*ppTail)->pNext = pTemp;
        *ppTail = pTemp;
    }
    return;
}

/* as a FIFO queue, will get the head node, has its value which will be return later, 
 *.                 free the head node, and update and let it point to new head 
 * Parameters:
 *  ppHead: always point to the head of stack
 *  ppTail: always point to the tail of stack
 *  pVal: thge value to be enter the tail of queue
 */

void * DeQueue(struct Queue **ppHead,struct Queue **ppTail){
    struct Queue *pTemp;
    void *pVal;
    if((*ppHead==NULL)||(*ppTail==NULL)){
        return NULL;
    }
    else{
        pTemp = *ppHead;
        if(*ppHead != *ppTail){
            *ppHead = pTemp->pNext;
            pTemp->pNext =NULL;
        }
        else{
            *ppHead = NULL;
            *ppTail = NULL;
        }
        pVal = pTemp->pVal;
        free(pTemp);
        return pVal;
    }
}

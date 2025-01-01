#include "stddef.h"
#include "stdlib.h"
#include "my_stack.h"
/* push a new node with value pVal to the stack top and update the stack top to the new node*/
void Stack_Push(struct StackNode **ppStackTop, void *pVal){
    struct StackNode *pNode;
    if(pVal==NULL) { //pointer to the target value
        return;
    }

    pNode = (struct StackNode *)malloc(sizeof(struct StackNode));
    pNode->pVal=pVal;
    pNode->prev = NULL;

    if(*ppStackTop==NULL) {
        *ppStackTop = pNode;
    }
    else{
        pNode->prev = *ppStackTop;
        *ppStackTop = pNode;
    }
    return;
}

/* pop the value fromm the top of stack, and free the node, update the stack top */
void *Stack_Pop(struct StackNode **ppStackTop){
    void *pVal;
    struct StackNode *pTop;
    if((ppStackTop==NULL)||(*ppStackTop==NULL)){
        return NULL;
    }
    
    pTop = *ppStackTop;
    *ppStackTop = pTop->prev;
    pVal = pTop->pVal;
    free(pTop);
    return pVal; 
}

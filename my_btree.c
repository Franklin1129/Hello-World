#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "assert.h"
#include "my_stack.h"
#include "my_btree.h"
#include "my_queue.h"

/* allocate a new tree node  */
struct TreeNode *AllocateTreeNode(){
    struct TreeNode *pNode;
    pNode = (struct TreeNode *)malloc( sizeof ( struct TreeNode ) );
    assert(pNode!=NULL);
    pNode->left = NULL;
    pNode->right = NULL;
    return pNode;
}

/* generate a binary tree
 * parameters
 *  pArray: point to array store the description of a tree
 *          [1,null,2,3] : 
 *                      1
 *                          2
 *                        3
 * 
 *  ArraySize: valid elements in the array
 */

struct TreeNode *GenerateBinaryTreebyArray(int *pArray, int ArraySize){
    struct TreeNode *pRoot, *pCurrentNode, *pNextNode; 
    
    struct Queue *pQueHead,*pQueTail;
    pQueHead = NULL;
    pQueTail = NULL;

    pRoot = NULL;
    pCurrentNode = NULL;

    if((pArray==NULL) || ArraySize <=0 ){
        return NULL;
    }

    for(int index = 0; index<ArraySize;index++){

        if(pArray[index]!=NULLNODE_IN_THE_INPUT_ARRAY) {//not NULL;
            pNextNode = AllocateTreeNode();  //new tree node       
            pNextNode->val = pArray[index];
            
            EnQueue(&pQueHead,&pQueTail,pNextNode); //enqueue the tree node
        }
        else{
            pNextNode = NULL;
        }

        if(pRoot == NULL) {
            assert(pNextNode!=NULL);
            pRoot = pNextNode; //the root node of binary tree
        }
        else{ 

            pCurrentNode = DeQueue(&pQueHead,&pQueTail); //add left and right child at same time 
            pCurrentNode ->left = pNextNode; 
            if(index+1<ArraySize){
                if(pArray[index+1] != NULLNODE_IN_THE_INPUT_ARRAY) {//not NULL;
                    pNextNode = AllocateTreeNode();        
                    pNextNode->val = pArray[index+1];
                    EnQueue(&pQueHead,&pQueTail,pNextNode);
                    
                }
                else{
                    pNextNode = NULL;
                }
                pCurrentNode->right = pNextNode;
                index++;
            }
        }
        
    }
    //clear the queue    
    while( DeQueue(&pQueHead,&pQueTail)!=NULL){
        continue;
    }
    return pRoot;
}

/* in order traversal
 * root: point to the root of the binary tree
 * returnSize: valid element numbers in the return array
 * return value : the address of the return array with returnSize valid elements
 *                the whole size of the return array may equal or bigger than  returnSize
 */
int* inorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *pCurrentNode;
    int ArrayIndex=0;
    int *pArray;

    pArray = (int *)malloc(MAX_TREE_NODE*sizeof(int));
    assert(pArray!=NULL);

    struct StackNode *pStack;
    pStack = NULL;
    pCurrentNode = root;
    while(pCurrentNode || pStack!=NULL){
        if(pCurrentNode!=NULL){
            Stack_Push(&pStack,pCurrentNode);
            pCurrentNode = pCurrentNode->left;
        }
        else{
            pCurrentNode = Stack_Pop(&pStack);
            pArray[ArrayIndex] = pCurrentNode->val;
            ArrayIndex++;       
            pCurrentNode = pCurrentNode->right;
        }
    }
    *returnSize=ArrayIndex;
    return pArray;
}

/* pre-order traversal
 * root: point to the root of the binary tree
 * returnSize: valid element numbers in the return array
 * return value : the address of the return array with returnSize valid elements
 *                the whole size of the return array may equal or bigger than  returnSize
 */
int* PreorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *pCurrentNode;
    int ArrayIndex=0;
    int *pArray;

    pArray = (int *)malloc(MAX_TREE_NODE*sizeof(int));
    assert(pArray!=NULL);

    struct StackNode *pStack;
    pStack = NULL;
    pCurrentNode = root;
    while(pCurrentNode || pStack!=NULL){    
        if( pCurrentNode!=NULL ) {
            //root visited first
            pArray[ArrayIndex] = pCurrentNode->val;
            ArrayIndex++;
            
            //push into stack for future right node
            Stack_Push(&pStack,pCurrentNode);
            
            //point to left node for next loop
            pCurrentNode = pCurrentNode->left;
        }
        else{
            pCurrentNode = Stack_Pop(&pStack);       
            pCurrentNode = pCurrentNode->right;
        }
    }
    *returnSize=ArrayIndex;
    return pArray;
}

/* post-order traversal
 * root: point to the root of the binary tree
 * returnSize: valid element numbers in the return array
 * return value : the address of the return array with returnSize valid elements
 *                the whole size of the return array may equal or bigger than  returnSize
 *    1.1 Create an empty stack
 *    2.1 Do following while root is not NULL
 *      a) Push root's right child and then root to stack.
 *      b) Set root as root's left child.
 *    2.2 Pop an item from stack and set it as root.
 *      a) If the popped item has a right child and the right child 
 *         is at top of stack, then remove the right child from stack,
 *         push the root back and set root as root's right child.
 *     b) Else print root's data and set root as NULL.
 * 2.3 Repeat steps 2.1 and 2.2 while stack is not empty.
 * 
 *  
 */
int* PostorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *pCurrentNode, *pRightNode;
    int ArrayIndex=0;
    int *pArray;

    pArray = (int *)malloc(MAX_TREE_NODE*sizeof(int));
    assert(pArray!=NULL);

    struct StackNode *pStack;
    pStack = NULL;
    pCurrentNode = root;
    while(pCurrentNode || pStack!=NULL){
   
        if(pCurrentNode!=NULL){
            if(pCurrentNode->right != NULL){
                Stack_Push(&pStack,pCurrentNode->right);
            }
            Stack_Push(&pStack,pCurrentNode);
           pCurrentNode = pCurrentNode->left;     
        }
        else{
            pCurrentNode = Stack_Pop(&pStack);
            if(pCurrentNode!=NULL){
                if( ( pCurrentNode->right != NULL) &&(pStack!=NULL) ){
                    if(pCurrentNode->right == pStack->pVal ){
                        pRightNode = Stack_Pop(&pStack);
                        Stack_Push(&pStack,pCurrentNode);
                        pCurrentNode = pRightNode;
                        continue;
                    }
                }
                
                pArray[ArrayIndex] = pCurrentNode->val;
                ArrayIndex++;
                pCurrentNode = NULL;                
            }
        }
    }
    *returnSize=ArrayIndex;
    return pArray;
}
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "assert.h"
#include "stdbool.h"
#include "string.h"

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

bool isSameTree(struct TreeNode* p, struct TreeNode* q) {

    if(p==NULL){
        if(q==NULL){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(q==NULL){
            return false;
        }
    }
    if( p->val != q->val ){
        return false;
    }
    return (isSameTree(p->left,q->left) && isSameTree(p->right,q->right));
}
/**
 * Based on levelorderTraversal
 */
void FreeBinaryTree(struct TreeNode* root){
    struct TreeNode *pCurrentNode;
    struct Queue *pQueHead,*pQueTail;
    if( root == NULL){
        return;
    }
    pQueHead = NULL;
    pQueTail = NULL;

    EnQueue(&pQueHead,&pQueTail,root);
    while(pQueHead!=NULL){
        pCurrentNode = DeQueue(&pQueHead,&pQueTail); 
        if(pCurrentNode->left != NULL){
            EnQueue(&pQueHead,&pQueTail,pCurrentNode->left);
        }      
        if(pCurrentNode->right != NULL){
            EnQueue(&pQueHead,&pQueTail,pCurrentNode->right);
        }
        free(pCurrentNode);
    }
    return;
}
/*LeetCode 297. Serialize and Deserialize Binary Tree */

/* the DIFFERENCE between levelorderTraversal2 and levelorderTraversal:
 * levelorderTraversal is return the order visited
 * levelorderTraversal2 is used to reconstruct the tree which means that the null node position 
 * should be noted if there is any valid node at its right side 
 */
int* levelorderTraversal2(struct TreeNode* root, int* returnSize) {
    struct TreeNode *pCurrentNode;
    struct TreeNode NullNode;
    int ArrayIndex=0;
    int Index;
    int *pArray;

    if( root == NULL){
        *returnSize = 0;
        return NULL;
    }
    pArray = (int *)malloc(MAX_TREE_NODE*sizeof(int)*2);
    assert(pArray!=NULL);

    struct Queue *pQueHead,*pQueTail;
    pQueHead = NULL;
    pQueTail = NULL;

    NullNode.val = NULLNODE_IN_THE_INPUT_ARRAY;
    NullNode.left = NULL;
    NullNode.right = NULL;

    EnQueue(&pQueHead,&pQueTail,root);

    while(pQueHead!=NULL){
        pCurrentNode = DeQueue(&pQueHead,&pQueTail);
        pArray[ArrayIndex] = pCurrentNode->val;
        ArrayIndex++;

        if(pCurrentNode->val==NULLNODE_IN_THE_INPUT_ARRAY){
            continue;
        }

        if(pCurrentNode->left != NULL){
            EnQueue(&pQueHead,&pQueTail,pCurrentNode->left);
        }
        else{
            EnQueue(&pQueHead,&pQueTail,&NullNode);
        }  

        if(pCurrentNode->right != NULL){
            EnQueue(&pQueHead,&pQueTail,pCurrentNode->right);
        }
        else{
            EnQueue(&pQueHead,&pQueTail,&NullNode);
        }  
    }
    //
    for(Index=ArrayIndex-1;Index>0;Index--){
        if(pArray[Index]!=NULLNODE_IN_THE_INPUT_ARRAY){
            break;
        }
    }
    ArrayIndex = Index+1;
    //
    *returnSize=ArrayIndex;
    return pArray;
}
/** Encodes a tree to a single string. */
char* serialize(struct TreeNode* root) {
    int *pArray;
    int ArraySize;
    char *String;
    if(root==NULL){
        return NULL;
    }
    
    ArraySize = 0;
    
    pArray = levelorderTraversal2(root,&ArraySize);
    
    String = GetStringByArray(pArray,ArraySize);
    if(pArray!=NULL){
        free(pArray);
    }
    return String;
}
/*The return string: number string,sperated by ','
 *numstring at most has 5 characters ( -1000 )
 */
char *GetStringByArray(int *pArray, int ArraySize){
    char TempString[MAX_NODESTRING+1];
    char *String;

    if((pArray==NULL) || (ArraySize<=0)){
        return NULL;
    }

    String = (char *)malloc(sizeof(char)*ArraySize*(MAX_NODESTRING+1) +1);
    String[0]='\0';
    
    for(int Index = 0; Index < ArraySize; Index++){
        if(pArray[Index] != NULLNODE_IN_THE_INPUT_ARRAY){
            //itoa(pArray[Index],TempString,10);
            sprintf(TempString,"%d",pArray[Index]);
            strcat(String,TempString);
            if(Index+1 < ArraySize) { //has next node
                strcat(String,",");
            }
        }            
        else{ // NULL
            if(Index+1 < ArraySize) { //has next node
                strcat(String,",");
            }
        }
    }
    return String;
}
/** Decodes your encoded data to tree. */
/* 
 * seperate symbol: ','
 */
struct TreeNode* deserialize(char* data){
    int *pArray;
    struct TreeNode *pRoot;
    int ArraySize=0;
    pArray = GetArrayByString(data,&ArraySize);
    pRoot = GenerateBinaryTreebyArray(pArray,ArraySize);
    if(pArray!=NULL){
        free(pArray);
    }
    return pRoot;
}

int *GetArrayByString(char *data, int *ArraySize) {
    char ch;
    char TempString[MAX_NODESTRING+1];
    int InputDataIndex;
    int Index;
    int *pArray;

    *ArraySize = 0;
    if(data ==NULL){
        return NULL;
    }
    pArray = (int *)malloc(sizeof(int)*BINARY_INPUT_ARRAY_LENGTH_MAX);

    InputDataIndex = 0;
    
    while(data[InputDataIndex]!='\0'){ //don't reach end of string
        Index = 0;
        while((data[InputDataIndex]!=',') && (data[InputDataIndex]!='\0')){//find numeric characters
            ch = data[InputDataIndex];
            TempString[Index] = ch;
            Index++;
            InputDataIndex ++;
        }

        if(Index==0) { // no numeric characters found
            if(data[InputDataIndex]=='\0'){ //end of the traversal of input data
                break;
            }
            if(data[InputDataIndex]==','){ //only ',' read this round
                
                pArray[*ArraySize] = NULLNODE_IN_THE_INPUT_ARRAY;
                (*ArraySize) ++ ;        
    
                InputDataIndex ++;
                continue;
            }
            else{ // data[InputDataIndex]=='\0', end of the traversal of input data
                break;
            }
        }

        //numeric characters found    
        assert(Index<= MAX_NODESTRING);
        TempString[Index] = '\0';
    
        pArray[*ArraySize] = atoi(TempString);
        (*ArraySize)++;

        if(data[InputDataIndex]=='\0'){
            break;
        }
    
        InputDataIndex ++;
    
    }
    return pArray;
}

// Your functions will be called as such:
// char* data = serialize(root);
// deserialize(data);


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
    pQueHead = NULL;//0
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

/* level order traversal
 * root: point to the root of the binary tree
 * returnSize: valid element numbers in the return array
 * return value : the address of the return array with returnSize valid elements
 *                the whole size of the return array may equal or bigger than  returnSize
 */

int* levelorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *pCurrentNode;

    int ArrayIndex=0;
    int *pArray;

    if( root == NULL){
        *returnSize = 0;
        return NULL;
    }
    pArray = (int *)malloc(MAX_TREE_NODE*sizeof(int));
    assert(pArray!=NULL);

    struct Queue *pQueHead,*pQueTail;
    pQueHead = NULL;
    pQueTail = NULL;


    EnQueue(&pQueHead,&pQueTail,root);

    while(pQueHead!=NULL){
        pCurrentNode = DeQueue(&pQueHead,&pQueTail);
        pArray[ArrayIndex] = pCurrentNode->val;
        ArrayIndex++; 
        if(pCurrentNode->left != NULL){
            EnQueue(&pQueHead,&pQueTail,pCurrentNode->left);
        }      
        if(pCurrentNode->right != NULL){
            EnQueue(&pQueHead,&pQueTail,pCurrentNode->right);
        }
    }
    *returnSize=ArrayIndex;
    return pArray;
}


///////////////////////////////////////
//leetcode 1028. Recover a Tree From Preorder Traversal
/*
 * Array preoder is the preorder traversal of a binary tree
 *   [preorderStrart,preoderEnd) is the preorder traversal of a binary sub-tree 
 * Array inoder is the inorder traversal of a binary tree
 *   [inorderStrart,inoderEnd) is the inorder traversal of a binary sub-tree
 */
struct TreeNode* buildSubTree(int* preorder, int preoderStart,int preorderEnd, int* inorder, int inorderStart, int inorderEnd) {
    
    struct TreeNode *pRoot;
    
    int preoderStart1,preorderEnd1,preoderStart2,preorderEnd2;
    int inorderStart1,inorderEnd1,inorderStart2,inorderEnd2;
    if(preoderStart>=preorderEnd){
        return NULL;
    }
    if(inorderStart>=inorderEnd){
        return NULL;
    }
    pRoot = (struct TreeNode *)malloc(sizeof(struct TreeNode ));
    assert(pRoot!=NULL);
    pRoot->val = preorder[preoderStart]; //preoder traversal starts from root
    pRoot->left = NULL;
    pRoot->right = NULL;


    for(int index=inorderStart;index<inorderEnd;index++){
        if(inorder[index]==pRoot->val){

            if(index>inorderStart){
                inorderStart1 = inorderStart;
                inorderEnd1   = index;
                preoderStart1 = preoderStart+1;
                preorderEnd1   = preoderStart1 + (inorderEnd1 - inorderStart1);
                pRoot->left  = buildSubTree(preorder,preoderStart1,preorderEnd1,inorder,inorderStart1,inorderEnd1);
            }
            else{
                pRoot->left = NULL;
            }
            if(index+1<inorderEnd){
                inorderStart2 = index+1;
                inorderEnd2   = inorderEnd;
                
                preorderEnd2   = preorderEnd; // inorderEnd2 - inorderStart2 == preorderEnd2 - preoderStart2
                preoderStart2 = preorderEnd2-(inorderEnd2 - inorderStart2);
                pRoot->right = buildSubTree(preorder,preoderStart2,preorderEnd2,inorder,inorderStart2,inorderEnd2);
            }
            else{
                pRoot->right = NULL;
            }
            return pRoot;
        }
    }
   
    return pRoot;
} 
/*
 * Given two integer arrays preorder and inorder where preorder is the preorder traversal of a binary tree and 
 * inorder is the inorder traversal of the same tree, construct and return the binary tree.
 * for example 
 * Input: preorder = [3,9,20,15,7], inorder = [9,3,15,20,7]
 * Output: [3,9,20,null,null,15,7]
 */   
struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize) {    
     if((preorder == NULL) || (inorder ==NULL)) {
        return NULL;
    }
    return buildSubTree(preorder,0,preorderSize,inorder,0,inorderSize);
}



////////////////////////////
//Leetcode 124. Binary Tree Maximum Path Sum
#define INVALID_VALUE_LEETCODE_124 -1000
#define N2max(a,b) ((a>b)?a:b)
#define N3max(a,b,c) N2max(N2max(a,b),c)
#define N4max(a,b,c,d) N2max(N2max(a,b),N2max(c,d))

void maxPathSum2(struct TreeNode* root,int *maxPathSum, int *maxPathRootasEndsum){
    int maxLeftPathSum;  //左子树内的maxPathSum
    int maxLeftPathRootasEndsum; //以root为一个端点，和左子树其他一起构成的特殊路，其的的maxPathSum
    int maxRightPathSum; //右子树内的maxPathSum
    int maxRightPathRootasEndsum;//以root为一个端点，和右子树其他一起构成的特殊路，其的的maxPathSum
    
    maxLeftPathSum = INVALID_VALUE_LEETCODE_124; //not 0
    maxLeftPathRootasEndsum = INVALID_VALUE_LEETCODE_124;
    maxRightPathSum = INVALID_VALUE_LEETCODE_124;
    maxRightPathRootasEndsum = INVALID_VALUE_LEETCODE_124;
    

    if(root->left!=NULL){
        maxPathSum2(root->left,&maxLeftPathSum,&maxLeftPathRootasEndsum);
    }
    else{
        maxLeftPathSum = INVALID_VALUE_LEETCODE_124;
        maxLeftPathRootasEndsum = INVALID_VALUE_LEETCODE_124;
    }

    if(root->right!=NULL){
        maxPathSum2(root->right,&maxRightPathSum,&maxRightPathRootasEndsum);
    }
    else{
        maxRightPathSum = INVALID_VALUE_LEETCODE_124;
        maxRightPathRootasEndsum = INVALID_VALUE_LEETCODE_124;
    }

    *maxPathRootasEndsum = N3max(maxLeftPathRootasEndsum+root->val,maxRightPathRootasEndsum+root->val,root->val);


    *maxPathSum = N4max(maxLeftPathSum,maxRightPathSum,*maxPathRootasEndsum,maxLeftPathRootasEndsum+root->val+maxRightPathRootasEndsum);
    return;
    
}
int maxPathSum(struct TreeNode* root) {
    int maxPathsum;
    int maxPathRootasEndsum;
    if(root == NULL){
        return 0;
    }
    maxPathSum2(root,&maxPathsum,&maxPathRootasEndsum);
    return(N2max(maxPathsum,maxPathRootasEndsum));
}
//end of leetcode 124. Binary Tree Maximum Path Sum
/////////////////////


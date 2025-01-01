#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "my_btree.h"

int main(){
    struct TreeNode *pRoot;
    int returnSize;
    int *pSearch;
    /* The input array is a discription the binary code 
     * it starts level by level, each level start from left to right
     * at each level, using NULLNODE_IN_THE_INPUT_ARRAY indicate it is NULL but there is/are some nodes at its right
     * the array ends with valid number (never ends with NULLNODE_IN_THE_INPUT_ARRAY)
     */

    #ifdef ARRAY12
    int ArraySize = 12;
    int array[] = {1,2,3,4,5,NULLNODE_IN_THE_INPUT_ARRAY,8,NULLNODE_IN_THE_INPUT_ARRAY,NULLNODE_IN_THE_INPUT_ARRAY,6,7,9};
    #else
    int ArraySize = 4;
    int array[] = {1,NULLNODE_IN_THE_INPUT_ARRAY,2,3};
    #endif

    for(int index=0;index<ArraySize;index++){
        printf("%d  ",array[index]);
    }
    printf("\r\n");
    pRoot = GenerateBinaryTreebyArray((int *)&array,ArraySize);

    //pSearch = inorderTraversal(pRoot,&returnSize);
    //pSearch = PreorderTraversal(pRoot,&returnSize);
    pSearch = PostorderTraversal(pRoot,&returnSize);
    for(int index=0;index<returnSize;index++){
        printf("%d  ",pSearch[index]);
    }
    printf("\r\n");
    free(pSearch);
    return 0;
}



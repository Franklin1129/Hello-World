struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};
#define MAX_TREE_NODE 1000

struct TreeNode *AllocateTreeNode();
/* in order traversal
 * root: point to the root of the binary tree
 * returnSize: valid element numbers in the return array
 * return value : the address of the return array with returnSize valid elements
 *                the whole size of the return array may equal or bigger than  returnSize
 */
int* inorderTraversal(struct TreeNode* root, int* returnSize);

#define NULLNODE_IN_THE_INPUT_ARRAY -1
struct TreeNode *GenerateBinaryTreebyArray(int *pArray, int ArraySize);
int* inorderTraversal(struct TreeNode* root, int* returnSize);
int* PreorderTraversal(struct TreeNode* root, int* returnSize);
int* PostorderTraversal(struct TreeNode* root, int* returnSize);
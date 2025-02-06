struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};
#define MAX_NODESTRING 5 // bianry tree's node vale [-1000,1000]
#define MAX_TREE_NODE 1024 //max node number of a binary tree
#define NULLNODE_IN_THE_INPUT_ARRAY -100000 //invalid node value of a binary tree
#define BINARY_INPUT_ARRAY_LENGTH_MAX 2048 //the max length of the array used to store the bianry tree

struct TreeNode *GenerateBinaryTreebyArray(int *pArray, int ArraySize);
int *GetArrayByString(char *data, int *ArraySize);
struct TreeNode* deserialize(char* data);
char *GetStringByArray(int *pArray, int ArraySize);
char* serialize(struct TreeNode* root);

struct TreeNode *AllocateTreeNode();
bool isSameTree(struct TreeNode* p, struct TreeNode* q);
void FreeBinaryTree(struct TreeNode* root);
/* in order traversal
 * root: point to the root of the binary tree
 * returnSize: valid element numbers in the return array
 * return value : the address of the return array with returnSize valid elements
 *                the whole size of the return array may equal or bigger than  returnSize
 */
int* inorderTraversal(struct TreeNode* root, int* returnSize);
int* PreorderTraversal(struct TreeNode* root, int* returnSize);
int* PostorderTraversal(struct TreeNode* root, int* returnSize);
int* levelorderTraversal(struct TreeNode* root, int* returnSize);

struct TreeNode* buildSubTree(int* preorder, int preoderStart,int preorderEnd, int* inorder, int inorderStart, int inorderEnd);
struct TreeNode* buildTree(int* preorder, int preorderSize, int* inorder, int inorderSize);
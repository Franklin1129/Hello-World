
/* stack node*/
struct StackNode{
    void *pVal; //value
    struct StackNode *prev; //pointer to previous node
};

/* push a new node with value pVal to the stack top and update the stack top to the new node*/
void Stack_Push(struct StackNode **ppStackTop, void *pVal);
/* pop the value fromm the top of stack, and free the node, update the stack top */
void *Stack_Pop(struct StackNode **ppStackTop);
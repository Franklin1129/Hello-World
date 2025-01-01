/* my Queue implementation*/
struct Queue{
    void *pVal;
    struct Queue *pNext;
};

/* as a FIFO queue, will allocate a new node and enter the tail of queue
 * Parameters:
 *  ppHead: always point to the head of stack
 *  ppTail: always point to the tail of stack
 *  pVal: thge value to be enter the tail of queue
 */
void EnQueue(struct Queue **ppHead,struct Queue **ppTail, void *pVal);
/* as a FIFO queue, will get the head node, has its value which will be return later, 
 *.                 free the head node, and update and let it point to new head 
 * Parameters:
 *  ppHead: always point to the head of stack
 *  ppTail: always point to the tail of stack
 *  pVal: thge value to be enter the tail of queue
 */

void * DeQueue(struct Queue **ppHead,struct Queue **ppTail);
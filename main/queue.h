#include <stdlib.h>
#include <stdio.h>

/* Queue API */
/*
        _________
    -->|__|__|__|--> 
        
*/

typedef struct Queue {
    int *data;
    int head;
    int tail;
    int size;
    int lastHeadVal;
    int lastTailVal;
    int tmpVal;
} Queue;


void QueueInit(Queue *q, int size);
int QueueIsEmpty(Queue *q);
void QueuePrint(Queue *q);
int QueueSendToHead(Queue *q, int val);
int QueueSendToBack(Queue *q, int val);
int QueueRmHead(Queue *q);
int QueueRmLast(Queue *q);
int QueueReceive(Queue *q, int *msg);
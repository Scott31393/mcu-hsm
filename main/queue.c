#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

/* Queue API */
/*
        _________
    -->|__|__|__|--> 
        
*/

/** 
 * @brief Initialize Queue
 * 
 * @param q Queue pointer
 * @param size Queue data buffer size
 * 
 */
void QueueInit(Queue *q, int size)
{
    q->data = (int *) malloc(size * sizeof(int));
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->lastHeadVal = q->data[q->head];
    q->lastTailVal = q->data[((q->tail - 1) % q->size)];
    q->tmpVal = q->lastTailVal;
}

/**
 * @brief Check if Queue is empty
 * 
 * @param q Queue pointer
 * @return int return -1 if Queue is empty else 0
 */
int QueueIsEmpty(Queue *q)
{
    int ret = 0;

    if(((q->tail + 1) % q->size) != q->head) {
        if(q->head == q->tail){
            ret = -1;
        } else {
            ret = 0;
        } 
    }

    return ret;
}

/**
 * @brief Print Queue data buffer 
 * 
 * @param q Queue pointer
 */
void QueuePrint(Queue *q)
{
    int i;
    if(((q->tail + 1) % q->size) != q->head) {
        for(i = q->head; i != q->tail; i = (i + 1) % q->size ){
            printf("q->data[%d] = %d \n",i ,q->data[i]);
        }
    }
        

}

/**
 * @brief Pre insert value into Queue
 * 
 * @param q Queue pointer
 * @param val Value
 * @return int return 0 if operation end fine else -1
 */
int QueueSendToHead(Queue *q, int val)
{
    int ret = 0;

    if(((q->tail + 1) % q->size) != q->head) {
        q->lastHeadVal = q->data[q->head];
        q->head = (q->head + q->size -1) % q->size;
        q->data[q->head] = val;
        ret = 0;

    } else {

        ret = -1;
    }

    return ret;
}

/**
 * @brief Suf Insert value to Queue
 * 
 * @param q Queue pointer
 * @param val Value
 * @return int return 0 if operation end fine else -1
 */
int QueueSendToBack(Queue *q, int val)
{
    int ret = 0;

    if(((q->tail + 1) % q->size) != q->head) {

        q->lastTailVal = q->data[((q->tail - 1) % q->size)];
        q->data[q->tail] = val;
        q->tail = (q->tail + 1) % q->size;
        ret = 0;

    } else {

        ret = -1;
    }

    return ret;
}

/**
 * @brief Remove Head
 * 
 * @param q Queue pointer
 * @return int return 0 if operation end fine else -1
 */
int QueueRmHead(Queue *q)
{
    int ret = 0;

    if(((q->tail + 1) % q->size) != q->head) {
        
        q->lastHeadVal = q->data[q->head];
        q->data[q->head] = 0;
        q->head = (q->head + 1) % q->size;
        ret = 0;

    } else {
        ret = -1;
    }

    return ret;
}

/**
 * @brief Remove last value into Queue
 * 
 * @param q Queue pointer
 * @return int return 0 if operation end fine else -1
 */
int QueueRmLast(Queue *q)
{
    int ret = 0;

    if(((q->tail + 1) % q->size) != q->head) {
        
        q->lastTailVal = q->data[((q->tail - 1) % q->size)];
        q->data[((q->tail - 1) % q->size)] = 0;
        q->tail = (q->tail - 1) % q->size;
        ret = 0;
    } else {
        ret = -1;
    }

    return ret;
}

/* */
/**
 * @brief Pop operation on the Queue last item, store last value into msg
 * 
 * @param q Queue pointer
 * @param msg Msg pointer
 * @return int Return 0 if Queue is empty else -1 and pop last item
 */
int QueueReceive(Queue *q, int *msg)
{
    int ret = 0;
    int err = 0;

    if(((q->tail + 1) % q->size) != q->head) {
        if(q->head == q->tail){
            ret = 0;
        } else {
            *msg = q->data[((q->tail -1) % q->size)];
            err = QueueRmLast(q);
            if(err)
                printf("Fail to rm tail");
            ret = -1;
            
        }
    }    

    return ret;

}
#include "priority_queue.h"


bool print_errors_PQueue = false;




int errorno_PQueue = 0;
long time_tracker;

void createEmptyPQueue(PriorityQueue **pp_queue, int capacity){
    Data empty;
    *pp_queue = createElementPQ(pp_queue, empty, 0);
    (*pp_queue)->capacity = capacity;
    (*pp_queue)->member_count = 0;
}

bool isEmptyPQueue(PriorityQueue *p_queue){
    if (p_queue->member_count == 0 || p_queue == NULL){
        return true;
    }
    return false;
}

bool isFullPQueue(PriorityQueue *p_queue){
    PriorityQueue *temp = malloc(sizeof(PriorityQueue));
    
    IF_PRINT_ERROR((temp == NULL && p_queue->capacity > p_queue->member_count), ERROR6, 4);

    if (temp == NULL || p_queue->member_count == p_queue->capacity){
        return true;
    }
    free(temp);

    return false;
}

void insertPQueue(PriorityQueue **pp_queue, Data value, int priority){
    if (isEmptyPQueue(*pp_queue)){
        (*pp_queue)->p_next = createElementPQ(&((*pp_queue)->p_next), value, priority);
        (*pp_queue)->member_count++;
        return ;
    }

    
    

    IF_PRINT_ERROR((isFullPQueue(*pp_queue)), ERROR4, 4);
    if (errorno_PQueue == 4)
    {
        return ;
    }

    (*pp_queue)->member_count++;

    PriorityQueue *p_element = createElementPQ(&p_element, value, priority);

    PriorityQueue *temp = (*pp_queue)->p_next;

    if (temp->priority < priority){
        p_element->p_next = temp;
        (*pp_queue)->p_next = p_element;

        return ;
    }

    // Finding an element in PriorityQueue after which the new element will be inserted.

    while (temp->priority >= priority){
        if (temp->p_next == NULL || temp->p_next->priority < priority){
            break ;
        }
        temp = temp->p_next;
    }


    // Inserting new element
    p_element->p_next = temp->p_next;
    temp->p_next = p_element;
}

void removePQueue(PriorityQueue **pp_queue){
    IF_PRINT_ERROR(isEmptyPQueue(*pp_queue), ERROR1, 1);
    if (errorno_PQueue == 1){
        return ;
    }

    PriorityQueue *new_head = (*pp_queue)->p_next->p_next;

    free((*pp_queue)->p_next);
    (*pp_queue)->p_next = new_head;

    (*pp_queue)->member_count--;
    
    return ;
}
Data getPQueue(PriorityQueue *p_queue){
    IF_PRINT_ERROR(isEmptyPQueue(p_queue), ERROR2, 2);
    if (errorno_PQueue == 2){
        Data garbage;
        return garbage;
    }

    return p_queue->p_next->value;
}

void deletePQueue(PriorityQueue **pp_queue){
    while ((*pp_queue)->p_next){
        PriorityQueue *new_head = (*pp_queue)->p_next->p_next;

        free((*pp_queue)->p_next);
        (*pp_queue)->p_next = new_head;
    }

    (*pp_queue)->member_count = 0;
    free(*pp_queue);
    *pp_queue == NULL;
}


void joinPQueue(PriorityQueue **pp_queue1, PriorityQueue **pp_queue2){

    PriorityQueue *p_head1 = (*pp_queue1)->p_next;
    PriorityQueue *p_head2 = (*pp_queue2)->p_next;

    IF_PRINT_ERROR((p_head1 == p_head2 && p_head1 != NULL), ERROR3, 3);
    
    if (errorno_PQueue == 3){
        return ;
    }
    
    //Sprintf("%d\n%d\n",(*pp_queue1)->member_count, (*pp_queue2)->member_count);

    int new_member_count = (*pp_queue1)->member_count + (*pp_queue2)->member_count;



    IF_PRINT_ERROR((new_member_count > (*pp_queue1)->capacity), ERROR5, 5);
    if (errorno_PQueue == 5){
        return ;
    }

    (*pp_queue1)->member_count = new_member_count;

    if (p_head1 == NULL){
        (*pp_queue1)->p_next = (*pp_queue2)->p_next;
        free(*pp_queue2);
        *pp_queue2 == NULL;
        return ;
    }
    else if(p_head2 == NULL){
        return ;
    }

    PriorityQueue *t1 = p_head1;
    PriorityQueue *t2 = p_head2;

    PriorityQueue *new_head = NULL;
    PriorityQueue *new_curr;



    if (comparePQ(t1, t2)){
        new_head = copyPQNULL(t1);
        new_curr = new_head;
        t1 = makeChildPQ(&t1);
    }
    else{
        new_head = copyPQNULL(t2);
        new_curr = new_head;
        t2 = makeChildPQ(&t2);
    }



    while (t1 != NULL || t2 != NULL){
        
        switch(comparePQ(t1, t2)){
            case 1:
                new_curr = new_curr->p_next = copyPQNULL(t1);
                t1 = makeChildPQ(&t1);                
            break;

            case 0:
                new_curr = new_curr->p_next = copyPQNULL(t2);
                t2 = makeChildPQ(&t2);  
            break;
        }
    }
    // Makes second p-queue clear, as it may point to elements from pp_queue1.
    free(*pp_queue2);
    *pp_queue2 == NULL;


    (*pp_queue1)->p_next = new_head;
}



static char comparePQ(PriorityQueue *p_queue1, PriorityQueue *p_queue2){
    // Returns 1 if p_queue1 has larger priority. Otherwise 0.
    if (p_queue1 == NULL){
        return 0;
    }
    if (p_queue2 == NULL){
        return 1;
    }


    if (p_queue1->priority > p_queue2->priority){
        return 1;
    }
    else if (p_queue1->priority < p_queue2->priority){
        return 0;
    }
    else if (p_queue1->priority == p_queue2->priority && p_queue1->time < p_queue2->time){
        return 1;
    }
    else{
        return 0;
    }
}

static PriorityQueue *copyPQNULL(PriorityQueue *p_queue){
    PriorityQueue *p_element = copyPQ(p_queue);
    p_element->p_next = NULL;
    return p_element;
}


static PriorityQueue *copyPQ(PriorityQueue *p_queue){
    PriorityQueue *p_element = (PriorityQueue *)malloc(sizeof(PriorityQueue));

    IF_PRINT_ERROR((p_element == NULL), ERROR4, 4);
    if (errorno_PQueue == 4){
        return NULL;
    }

    p_element->priority = p_queue->priority;
    p_element->value = p_queue->value;

    p_element->time = p_queue->time;
    p_element->p_next = p_queue->p_next;


    return p_element;
}

static PriorityQueue *makeChildPQ(PriorityQueue **pp_queue){
    PriorityQueue *child = (*pp_queue)->p_next;

    free(*pp_queue);
    *pp_queue = NULL;

    return child;
}

static PriorityQueue *createElementPQ(PriorityQueue **pp_queue, Data value, int priority){
    *pp_queue = (PriorityQueue *)malloc(sizeof(PriorityQueue));
    IF_PRINT_ERROR((*pp_queue == NULL), ERROR4, 4);
    if (errorno_PQueue == 4){
        return NULL;
    }

    (*pp_queue)->p_next = NULL;
    (*pp_queue)->value = value;
    (*pp_queue)->priority = priority;
    (*pp_queue)->member_count = 0;
    (*pp_queue)->capacity = 0;

    (*pp_queue)->time = time_tracker++;
    return (*pp_queue);
}



/* The print statements commented out are just to demonstrate actual merging and working of the algorithm properly */

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#define MAX_HEAP_SIZE 262152//size of 256 kilobytes + threshold
typedef enum{NTINIT,INIT} status;
typedef enum{FALSE,TRUE} boolean;
static status sc =  NTINIT;
typedef struct heapmetadata
{
    size_t size;
    struct heapmetadata* next;
}MetaData;

void print(MetaData *ptr){
    printf("\nAddressofBlock = %p\n",ptr);
    printf("size = %d\n",ptr->size);
}

void DisplayLists(MetaData* head){ //this has been made just to verify whether all allocation and deallocation calls are being done correctly or not 
    if(head == NULL){
        printf("\nNULL\n");
    }
    else{
        MetaData *ptr = head;
        while(ptr){
            print(ptr);
            ptr = ptr->next;
        }
    }
}

#define threshold 8
char simulatedHeap[MAX_HEAP_SIZE];
MetaData* freelistHead = (void *)simulatedHeap;

void initializeHead(){
    freelistHead->size = MAX_HEAP_SIZE - threshold;
    freelistHead->next = NULL;
    printf("\n----Heap simulation Successfull----\n");
}   

MetaData *allocatedhead = NULL;

void insertAllocated(MetaData* block){
    if(allocatedhead== NULL){
        allocatedhead= block;
        block->next = NULL;
    }
    else{
        MetaData *itr = allocatedhead;
        while(itr->next){
            itr = itr->next;
        }
        itr->next = block;
        block->next = NULL;
    }
}

void *Allocate(size_t sizerequest){
    if(sc == NTINIT){
        initializeHead();
        sc = INIT;
    }
    if(sizerequest == 0){
        printf("\nCouldn't Allocate Block of zero size \n");
        printf("\nAllocate() Failed\n");
        return NULL;
    }
    sizerequest = (sizerequest < threshold) ? threshold : sizerequest;
    void *ret_val = NULL;
    MetaData *allocatedblock;
    sizerequest += threshold;
    MetaData *itr = freelistHead;
    MetaData *prev = NULL;
    boolean found = FALSE;
    while(itr && !found){
        if(itr->size >= sizerequest){
            found = TRUE;
        }
        else{
            prev = itr;
            itr = itr->next;
        }
    }
    if(itr == NULL){
        ret_val =  NULL;
        printf("\nUnable to allocate Memory \n");
        printf("Memory is full (or) External Fragmentation\n\n");
        return ret_val;
    }
    else if(itr && (itr->size - sizerequest <= threshold)){
        allocatedblock = itr;
        if(prev)
            prev = prev->next;
        else
            freelistHead = NULL;
        printf("\nMemory Allocation successfull\n");
    }
    else if(itr && (itr->size - sizerequest > threshold)){
        allocatedblock = itr;
        void *newhead = (void *)((void *)itr + sizerequest);
        MetaData* next = itr->next;
        int prevsize = itr->size;
        itr = newhead;
        itr->size = prevsize - sizerequest;
        itr->next = next;
        allocatedblock->size = sizerequest - threshold;
        if(prev == NULL)
            freelistHead = itr;
        else
            prev->next = itr;
        printf("\nMemory Allocation successfull\n");
    }
    insertAllocated(allocatedblock);
    ret_val = (void *)(allocatedblock + 1);
    return ret_val;
}

void Deallocate(void *p){
    if(p == NULL){
        printf("\nProvide a valid pointer to Deallocate\n");
        printf("\nDeallocation failed");
        return;
    }
    void *startaddress = p - threshold;
    MetaData* itr = allocatedhead;
    MetaData *ptr = NULL;
    while(itr && (void *)itr != startaddress){
        ptr = itr;
        itr = itr->next;
    }
    if(itr){
        size_t size = itr->size;
        void *endaddress = startaddress + size + threshold;

        //removing the required deallocated block from allocated list
        if(ptr == NULL){
            allocatedhead = itr->next;
        }
        else{
            ptr->next = itr->next;
        }
        itr->next = NULL;
        //done removing

        MetaData *freeitr = freelistHead;
        MetaData *freeprev = NULL;
        while(endaddress > (void *)freeitr){
            freeprev = freeitr;
            freeitr = freeitr->next;
        }
        void *prevend = NULL;
        if(!freeprev && freeitr){
            if(endaddress == (void *)freeitr){
                int temp = freeitr->size;
                MetaData *next = freeitr->next;
                freelistHead = (MetaData *)startaddress;
                freelistHead->size = temp + threshold + size;
                freelistHead->next = next;
            }
            else{
                itr->next = freelistHead;
                freelistHead = itr;         
            }
        }
        else if(freeprev && !freeitr){
            void *add = (void *)freeprev + freeprev->size + threshold;
            if(add == startaddress){
                freeprev->size += size + threshold;
            }
            else{
                freeprev->next = itr;
                itr->next = NULL;
            }
        }
        else{
            void *left = (void *)freeprev + freeprev->size + threshold;
            if(startaddress != left && endaddress != freeitr){
                itr->next = freeitr;
                freeprev->next = itr;
            }
            else if(startaddress == left && endaddress != freeitr){
                freeprev->size += itr->size + threshold;
            }
            else if(startaddress != left && endaddress == freeitr){
                MetaData *next = freeitr->next;
                int t = freeitr->size;
                freeitr = (MetaData *)endaddress;
                freeitr->size = t + size + threshold;
                freeitr->next = next;
            }
            else{
                freeprev->size += itr->size + freeitr->size + (2 * threshold);
                freeprev->next = freeitr->next;
                freeitr->next = NULL;
            }
        }
        printf("\nMemory successfully Freed Up\n");
    }
    else{
        printf("\nFree up a valid element from a heap \n");
        printf("\n Deallocate(void *p) failed \n");
    }
}

int main()
{
    int *p = (int *)Allocate(sizeof(int) * 100);
    int *q = (int *)Allocate(sizeof(int) * 700);
    float *r = (float *)Allocate(sizeof(float) * 200);
    char *s = (char *)Allocate(sizeof(char) * 30);
    // printf("\nFree List = \n");
    // DisplayLists(freelistHead);
    // printf("\nAllocated List = \n");
    // DisplayLists(allocatedhead);

    Deallocate(p);

    // printf("\nFree List = \n");
    // DisplayLists(freelistHead);
    // printf("\nAllocated List = \n");
    // DisplayLists(allocatedhead);

    Deallocate(r);

    // printf("\nFree List = \n");
    // DisplayLists(freelistHead);
    // printf("\nAllocated List = \n");
    // DisplayLists(allocatedhead);

    Deallocate(q);

    // printf("\nFree List = \n");
    // DisplayLists(freelistHead);
    // printf("\nAllocated List = \n");
    // DisplayLists(allocatedhead);

    Deallocate(s);

    // printf("\nFree List = \n");
    // DisplayLists(freelistHead);
    // printf("\nAllocated List = \n");
    // DisplayLists(allocatedhead);

    printf("\n\n\n-------Heap allocation and deallocation done successfully-------\n\n\n");
    return 0;
}

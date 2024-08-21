//==========================================================<Include Libs>
#include "Dynamic_Allocation_APIs.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Extern Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
extern char  MyHeap[];
extern char* MyProgram_Break;
//========================================================================

//----------------------------------------------<Global struct user define>
struct Temp
{
    int arr[100];
    
} sTemp;

void main()
{
    int* ptr1;
    int* ptr2;
    struct Temp* pTemp;
    
    /*Test Case 1: First allocate 4 byte */
    printf("                     In Init: MyHeap:%x  MyProgram_Break:%x\n\n", &MyHeap, MyProgram_Break);
    ptr1 = Hmm_Alloc(4);
    printf("After allocate first block:  MyHeap:%x  MyProgram_Break:%x Number of pages:%d size:%d\n", &MyHeap, MyProgram_Break, ( (unsigned long)MyProgram_Break - (unsigned long)&MyHeap ) / PageSize , (unsigned long)MyProgram_Break - (unsigned long)ptr1 );
    //===========================================================================================================================================================================================================================================================

    /*Test Case 2: Allocate again 4 byte in sequence with the first*/ 
    ptr2 = Hmm_Alloc(4);
    printf("After allocate second block: MyHeap:%x  MyProgram_Break:%x Number of pages:%d size:%d\n", &MyHeap, MyProgram_Break, ( (unsigned long)MyProgram_Break - (unsigned long)&MyHeap ) / PageSize , (unsigned long)ptr1 - (unsigned long)ptr2 - 8 * 3 );
    //===========================================================================================================================================================================================================================================================

    /*Test Case 3: Allocate 400 which is bigger than any free space in my FreeBlock Nodes ,so
     
      1. it will change MyProgram_Break after checking the added pages isn't execute MyHeap size
      2. check if there is before it free space to integrate together.
    
    */
    pTemp = Hmm_Alloc(sizeof(sTemp));
    printf("After allocate third block:  MyHeap:%x  MyProgram_Break:%x Number of pages:%d size:%d\n\n", &MyHeap, MyProgram_Break, ( (unsigned long)MyProgram_Break - (unsigned long)&MyHeap ) / PageSize , (unsigned long)MyProgram_Break - (unsigned long)pTemp );
    //===========================================================================================================================================================================================================================================================
   
    /*Test Case 4: free allocated memory before it also free memory*/
    Hmm_free(ptr2);
    printf("After free the second allocated block:   MyProgram_Break:%x Number of pages:%d\n", MyProgram_Break, ( (unsigned long)MyProgram_Break - (unsigned long)&MyHeap ) / PageSize);

    /*Test Case 5: free allocated memory before it and after free memory*/
    Hmm_free(ptr1);
    printf("After free the first allocated block:    MyProgram_Break:%x Number of pages:%d\n", MyProgram_Break, ( (unsigned long)MyProgram_Break - (unsigned long)&MyHeap ) / PageSize);
    //===========================================================================================================================================================================================================================================================
   
    /*Test Case 6: Free allocated memory before it a big free memory and wait if program break will decrease or not*/ 
    Hmm_free(pTemp);
    printf("After free the third allocated block:    MyProgram_Break:%x Number of pages:%d\n", MyProgram_Break, ( (unsigned long)MyProgram_Break - (unsigned long)&MyHeap ) / PageSize);
    //===========================================================================================================================================================================================================================================================
}
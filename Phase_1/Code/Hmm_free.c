//===============================<Include Libs>
#include "Dynamic_Allocation_APIs.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Extern Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
extern char  MyHeap[];
extern char* MyProgram_Break;
extern FreeBlockNode_t* HeadNode;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        Services function                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void TryToChangeMyProgramBreak()
{
    FreeBlockNode_t* Temp = HeadNode;
    unsigned long  NumOfFreePages = 0;

    if ( HeadNode != NULL)
    {
        while (Temp->pNext_FreeBlock != NULL)
        {
            Temp = Temp->pNext_FreeBlock;
        }

        NumOfFreePages = Temp->LengthOfBlock / (size_t)PageSize;

        if (NumOfFreePages > 4)
        {
            MyProgram_Break = MyProgram_Break - (NumOfFreePages - 4) * PageSize;

            Temp->LengthOfBlock = Temp->LengthOfBlock - (NumOfFreePages - 4) * PageSize;
        }
    }
}

int IsAllocateBlockExist(char* ptr)
{
    if ( (unsigned long)ptr >= (unsigned long)&MyHeap && ( (unsigned long)MyProgram_Break ) >= (unsigned long)ptr )
    {
        return 1;

    } else
    {
        return 0;
    }
}
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function implementation                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void Hmm_free(void* ptr)
{
    ptr = (char*)ptr - 8 - ( PointerSize * 2 );

    if ( IsAllocateBlockExist(ptr) == 1 )
    {
        /*Create free Node*/
        Create_FreeNode(ptr);

        /*if there ability to change MyProgramBreak*/
        TryToChangeMyProgramBreak();

    } else
    {
        if ( write(STDOUT_FILENO, "Allocated block is not exit\n", strlen("Allocated block is not exit\n") ) == -1 )
        {
            perror("write");
        }

        return NULL;
    }
}
//===============================<Include Libs>
#include "Dynamic_Allocation_APIs.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Extern Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
extern char* MyProgram_Break;
extern FreeBlockNode_t* HeadNode;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        Services function                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
static void TryToChangeMyProgramBreak()
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
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function implementation                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void Hmm_free(void* ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    ptr = (char*)ptr - 8 - ( PointerSize * 2 );

    //Create free Node
    Create_FreeNode(ptr);

    //if there ability to change MyProgramBreak
    TryToChangeMyProgramBreak();
}
//==========================================================<Include Libs>
#include "FreeBlocks_LinkedLists.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Global Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
FreeBlockNode_t* HeadNode = NULL;
FreeBlockNode_t* CurrentNode = NULL;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Extern Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
extern char  MyHeap[];
extern char* MyProgram_Break;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        Services function                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
static int FreeFreedBlock(char* ptr)
{
    FreeBlockNode_t* Temp1 = HeadNode;

    while (Temp1 != NULL)
    {
        if (Temp1 == ptr)
        {
            return 1;
        }

        Temp1 = Temp1->pNext_FreeBlock;
    }
    
    return 0;
}

static void Connect_Node(char* ptr)
{
    FreeBlockNode_t* TEMP1 = HeadNode;
    FreeBlockNode_t* TEMP2 = ptr;
    FreeBlockNode_t* TEMP3 = HeadNode;

    if ( ( TEMP3 = HeadNode->pNext_FreeBlock ) != NULL)
    {
        while (TEMP3 != NULL)
        {
            if ( (size_t)TEMP1 < (size_t)ptr && (size_t)TEMP3 > (size_t)ptr)
            {
                TEMP2->pNext_FreeBlock = TEMP3;
                TEMP2->pPrevious_FreeBlock = TEMP1;
                TEMP1->pNext_FreeBlock = TEMP2;
                TEMP3->pPrevious_FreeBlock = TEMP2;
                return;
            }

            TEMP1 = TEMP1->pNext_FreeBlock;
            TEMP3 = TEMP3->pNext_FreeBlock;
        } 

        TEMP1->pNext_FreeBlock = TEMP2;
        TEMP2->pNext_FreeBlock = NULL;
        TEMP2->pPrevious_FreeBlock = TEMP1;

    } else
    {
        TEMP1->pNext_FreeBlock = TEMP2;
        TEMP2->pPrevious_FreeBlock = TEMP1;
        TEMP2->pNext_FreeBlock = NULL;
    }
}

static void MakeFreeNode(char* ptr)
{
    FreeBlockNode_t* Temp1 = HeadNode;
    
    if ( HeadNode == NULL )
    {
        HeadNode = (FreeBlockNode_t*)ptr;
        HeadNode->pPrevious_FreeBlock = NULL;
        HeadNode->pNext_FreeBlock = NULL;

    } else if ( ptr > HeadNode )
    {
        Connect_Node(ptr);

    } else if ( ptr < HeadNode )
    {
        Temp1 = HeadNode;
        HeadNode = (FreeBlockNode_t*)ptr;
        HeadNode->pNext_FreeBlock = Temp1;
        HeadNode->pPrevious_FreeBlock = NULL;
    }
}

static void Integrate_Sequence_FreeBlocks()
{
    FreeBlockNode_t* Temp1 = NULL;
    FreeBlockNode_t* Temp2 = NULL;
    FreeBlockNode_t* Temp3 = NULL;

    Temp1 = HeadNode;
    Temp2 = HeadNode;

    while (Temp2->pNext_FreeBlock != NULL)
    {
        Temp2 = Temp2->pNext_FreeBlock;
    }

    Temp1 = Temp2->pPrevious_FreeBlock;

    while (Temp1 != NULL)
    {
        if ( ( (unsigned long)Temp1 + Temp1->LengthOfBlock + 8 + PointerSize * 2 ) == (unsigned long)Temp2 )
        {
            Temp1->LengthOfBlock = Temp2->LengthOfBlock + Temp1->LengthOfBlock + (8 + PointerSize * 2);
            Temp1->pNext_FreeBlock = Temp2->pNext_FreeBlock;

            if (Temp2->pNext_FreeBlock != NULL)
            {
                Temp3 = Temp2->pNext_FreeBlock;
                Temp3->pPrevious_FreeBlock = Temp1;
            }
        }
        
        Temp1 = Temp1->pPrevious_FreeBlock;
        Temp2 = Temp2->pPrevious_FreeBlock;
    }

    //Another algorithm
    //  Temp1 = HeadNode;
    //  Temp2 = HeadNode->pNext_FreeBlock;

    //  while (Temp2 != NULL)
    //  {
    //      if ( ( (unsigned long)Temp1 + Temp1->LengthOfBlock + 8 + PointerSize * 2 ) == (unsigned long)Temp2 )
    //      {
    //          Temp1->LengthOfBlock = Temp2->LengthOfBlock + Temp1->LengthOfBlock + (8 + PointerSize * 2);
    //          Temp1->pNext_FreeBlock = Temp2->pNext_FreeBlock;

    //          if (Temp2->pNext_FreeBlock != NULL)
    //          {
    //              Temp3 = Temp2->pNext_FreeBlock;
    //              Temp3->pPrevious_FreeBlock = Temp1;
    //          }
    //      }

    //      Temp1 = Temp1->pNext_FreeBlock;
    //      Temp2 = Temp2->pNext_FreeBlock;
    //  }
    
}
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function implementation                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void Create_FreeNode(char* ptr)
{

    /*Check if the user try to Free block are freeed before*/
    if ( FreeFreedBlock(ptr) == 1 )
    {
        if ( write(STDOUT_FILENO, "You try to free this block again", strlen("You try to free this block again") ) == -1 )
        {
            perror("write");
        }
        return;
    }

    /*Make the free node*/
    MakeFreeNode(ptr);

    /*if there is two sequence free nodes the function will merge then to one free block*/
    Integrate_Sequence_FreeBlocks();
}


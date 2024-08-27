//==========================================================<Include Libs>
#include "Dynamic_Allocation_APIs.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Global Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
char* MyProgram_Break = NULL;
extern FreeBlockNode_t* HeadNode;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        Services function                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
static char* Spilt_Block(FreeBlockNode_t* FreeBlock, size_t Length)
{
    FreeBlock->LengthOfBlock = (size_t)( FreeBlock->LengthOfBlock - (Length) - 8 - PointerSize * 2 );

    FreeBlock = ( (char*)FreeBlock + (8 + PointerSize * 2) + FreeBlock->LengthOfBlock );

    FreeBlock->LengthOfBlock = Length;
    FreeBlock = ( (char*)FreeBlock + (8 + PointerSize * 2) );

    return FreeBlock;
}

static char* FirstFit(size_t Length)
{
    FreeBlockNode_t* Temp = HeadNode;

    while (Temp != NULL)
    {
        if ( Temp->LengthOfBlock > ( Length + 2 * PointerSize + 8 ) )
        {
            return Temp;

        } else
        {
            Temp = Temp->pNext_FreeBlock;
        }
    }

    return Temp;
}

static align(size_t Num)
{
    if ( Num < 8 )
    {
        Num = 8;

    } else
    {
        //equation to align the number
        Num = Num + ( AligningNum - ( Num % AligningNum ) );  
    }

    return Num;
}

//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function implementation                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void* Hmm_Alloc(size_t size)
{
    FreeBlockNode_t* Temp = NULL;
    size_t i = 1;

    
    if ( size == 0 )
    {
        size = 8;
    }

    //Align the size to 8
    if ( ( size % 8 ) != 0 )
    {
         size = align(size);
    }
    
    if ( MyProgram_Break == NULL )
    {
        MyProgram_Break = sbrk(PageSize);

        *( (size_t*)MyProgram_Break ) = (size_t)( PageSize - (PointerSize * 2 + 8) );

        Create_FreeNode(MyProgram_Break);
    }

    if ( ( Temp = FirstFit(size) ) == NULL )
    {
        while ( PageSize * i < (size + 8 + PointerSize * 2) )
        {
            i += 1;
        }

        MyProgram_Break = sbrk(PageSize * i);

        *( (size_t*)MyProgram_Break ) = (size_t)( PageSize * i - (PointerSize * 2 + 8) );

        Create_FreeNode(MyProgram_Break);

        return (void*)Spilt_Block(MyProgram_Break, size);

    } else
    {
        return (void*)Spilt_Block(Temp, size);
    }
}


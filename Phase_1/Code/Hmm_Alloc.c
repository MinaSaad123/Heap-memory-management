//==========================================================<Include Libs>
#include "Dynamic_Allocation_APIs.h"

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           Global Vars                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
char  MyHeap[MyHeapSize];
char* MyProgram_Break = MyHeap;
extern FreeBlockNode_t* HeadNode;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        Services function                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
static char* Spilt_Block(char* FreeBlock, size_t Length)
{
    FreeBlockNode_t* Temp = (FreeBlockNode_t*)FreeBlock;

    Temp->LengthOfBlock = (size_t)( Temp->LengthOfBlock - (Length) - 8 - PointerSize * 2 );

    Temp = ( (char*)Temp + (8 + PointerSize * 2) + Temp->LengthOfBlock );

    Temp->LengthOfBlock = Length;
    Temp = ( (char*)Temp + (8 + PointerSize * 2) );

    return Temp;
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
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function implementation                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void* Hmm_Alloc(size_t size)
{
    char* Old_MyProgram_Break = NULL, *Temp = NULL;
    int i = 2;

    while ( (size + ( 8 + PointerSize * 2 ) ) > ( (PageSize * i) - 8 - PointerSize * 2) ) 
    {
        i += 2;
    }

    if ( MyProgram_Break == MyHeap )
    {
        if ( ( MyProgram_Break + ( PageSize * i ) ) <= ( MyHeap  + MyHeapSize ) &&
           (unsigned long)( (unsigned long)MyProgram_Break + (unsigned long)( PageSize * i ) ) <= (unsigned long)( (unsigned long)MyHeap  + (unsigned long)MyHeapSize ) && (unsigned long)MyProgram_Break >= (unsigned long)&MyHeap  )
        {
            MyProgram_Break += ( PageSize * i ); /* increase static array Instead of sbrk*/

            *( (size_t*)(MyHeap) ) = (size_t)( (PageSize * i) - 8 - PointerSize * 2);
            Create_FreeNode(MyHeap);

        } else
        {
            if ( write(STDOUT_FILENO, "MyHeap size is not enough\n", strlen("MyHeap size is not enough\n") ) == -1 )
            {
                perror("write");
            }

            return NULL;
        }
    } 

    if ( ( Temp = FirstFit(size) ) == NULL )
    {
        Old_MyProgram_Break = MyProgram_Break;

        if ( (unsigned long)( (unsigned long)MyProgram_Break + (unsigned long)( PageSize * i ) ) <= (unsigned long)( (unsigned long)MyHeap  + (unsigned long)MyHeapSize ) && (unsigned long)MyProgram_Break >= (unsigned long)&MyHeap )
        {
            MyProgram_Break += (unsigned long)( PageSize * i ); /* increase static array Instead of sbrk*/

        } else
        {
            if ( write(STDOUT_FILENO, "MyHeap size is not enough\n", strlen("MyHeap size is not enough\n") ) == -1 )
            {
                perror("write");
            }

            return NULL;
        }

        *( (size_t*)(Old_MyProgram_Break) ) = (size_t)( (size_t)(PageSize * i) - 8 - PointerSize * 2);

        Create_FreeNode(Old_MyProgram_Break);

        return (void*)Spilt_Block(Old_MyProgram_Break, size);

    } else
    {
        return (void*)Spilt_Block(Temp, size);
    }
}
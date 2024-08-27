//==========================================================<Include Libs>
#include "Dynamic_Allocation_APIs.h"
int i = 0;

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        Services function                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
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
//                         function implementation                          
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void* malloc(size_t size)
{
    // if (i == 0)
    // {
    //     i = 1;
    //     sleep(20);
    // }
    return Hmm_Alloc(size);
}

void free(void* ptr)
{
    Hmm_free(ptr);
}

void* calloc(size_t nmemb, size_t size)
{
    char* ptr = NULL;
    size_t calloc_size = 0;
    unsigned int i = 0;

    if ( ( nmemb > MaxNum / size ) || ( size > MaxNum / nmemb ) ) /*this state is Overflow*/
    {
        if ( write(STDOUT_FILENO, "error: you excceed the max size", strlen("error: you excceed the max size")) == -1 )
        {
            perror("calloc");
        }
        return NULL;

    } else if ( size == 0 || nmemb == 0 ) /*this state is not allowed*/
    {
        return NULL;
    }

    //Allocate the wanted size.
    ptr = Hmm_Alloc(nmemb * size); 

    //Align the size.
    if ( ( size % 8 ) == 0 )
    {
        calloc_size = align(nmemb * size);
    }

    //assign all the allocated memory to zero.
    for (i = 0; i < calloc_size; ++i)
    {
        ptr[i] = (char)0;
    }

    return ptr;
}

void* realloc(void* ptr, size_t size)
{
    if ( ptr == NULL && size != 0  || ptr == NULL && size == 0)
    {
        return malloc(size);

    } else if ( ptr != NULL && size == 0 )
    {
        free(ptr);
        return NULL;

    } else 
    {
        FreeBlockNode_t* Temp = (FreeBlockNode_t*)( (char*)ptr - 8 - 2 * PointerSize );

        if ( Temp->LengthOfBlock > size )
        {
            FreeBlockNode_t* Freeing_Block = (FreeBlockNode_t*)( (char*)ptr + size );

            if ( Temp->LengthOfBlock - size >= ( PointerSize * 2 + 8 ) )  //in case: the the change of block will be big enough for free block frame
            {
                Freeing_Block->LengthOfBlock = Temp->LengthOfBlock - size - ( PointerSize * 2 + 8 );
                Temp->LengthOfBlock = size;
                free(Freeing_Block);

            } else //in case: it is small so we will ignore it
            {
                Temp->LengthOfBlock = size;
            }

            return ptr;

        } else if ( Temp->LengthOfBlock < size ) /*in case: new size bigger than old size*/
        { 
            //---->Suppouse there is not free block after it to make it easier
            char* New_ptr = NULL;
            unsigned int i = 0;
            size_t OldSize = Temp->LengthOfBlock;

            //Allocate the new block
            New_ptr = (char*)malloc(size);

            //Allocate old block's data in new block.
            Temp = (char*)Temp + 8 + 2 * PointerSize;

            for (i = 0; i < OldSize; ++i)
            {
                New_ptr[i] = *( (char*)Temp + i );
            }

            return New_ptr;

        } else if ( Temp->LengthOfBlock == size ) /*in case: the size is the same*/
        {
            return ptr;
        }
    }
}
//========================================================================
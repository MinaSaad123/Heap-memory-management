#ifndef FREEBLOCKS_LINKEDLISTS_H
#define FREEBLOCKS_LINKEDLISTS_H

//==========================================================<Include Libs>
#include "Dynamic_Allocation_APIs.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                           User defined                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
struct FreeBlockNode
{
    size_t LengthOfBlock;
    struct FreeBlockNode* pPrevious_FreeBlock;
    struct FreeBlockNode* pNext_FreeBlock;

};

typedef struct FreeBlockNode FreeBlockNode_t;
//========================================================================

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function Prototype                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void Create_FreeNode(char* ptr);

#endif
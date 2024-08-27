#ifndef DYNAMIC_ALLOCATION_APIS_H
#define DYNAMIC_ALLOCATION_APIS_H

//=========================<Include Libs>
#include "FreeBlocks_LinkedLists.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                             Macros                         
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
#define PageSize (134000*1024)
#define PointerSize 8
#define MaxNum 0xffffffffffffffff
#define AligningNum 8

//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                       function Prototype                           
//=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
void* Hmm_Alloc(size_t size);
void  Hmm_free(void* ptr);
#endif
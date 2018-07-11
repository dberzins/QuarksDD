#if !defined(QTESTBUFFER_CPP)

#include <QuarksDD.h>
#include <Memory.h>
#include <Buffer.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestBuffer
{

internal uint32 BufferInit()
{
    struct BufElemType {
        uint32 x;
        uint32 y;
    };

    BufElemType* elements = NULL;

    BufPush(elements, BufElemType, BufElemType({1, 2}));
    _Assert(elements[0].x == 1 && elements[0].y == 2, "elements[0]->x == 1 && elements[0]->y == 2");

    BufPush(elements, BufElemType, BufElemType({3, 4}));
    _Assert(elements[1].x == 3 && elements[1].y == 4, "elements[0]->x == 1 && elements[0]->y == 2");

    BufferHdr* hdr = BufHdr(elements);
    _Assert(hdr->count == 2, "hdr->count == 2");

    BufClear(elements);
    _Assert(hdr->count == 0 && BufCount(elements) == 0, "hdr->count == 0 && BufCount(elements) == 0");

    BufFree(elements);
    _Assert(elements == NULL, "elements == NULL");
    
    // _Assert(a.items != NULL, "a.items != NULL");
    return true;
}

internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Strechy Buffer test group\n");
    printf("===========================================\n");
    _RunTest(BufferInit);
}

} // namespace

#define QTESTBUFFER_CPP
#endif
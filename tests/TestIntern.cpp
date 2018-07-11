#if !defined(QTESTINTERN_CPP)

#include <QuarksDD.h>
#include <Memory.h>
#include <Intern.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestIntern
{

internal uint32 InternInit()
{

    InternTable table;
    table = {};
    table.Init();

    char a[] = "Hello World";
    char b[] = "Hello World";
    char c[] = "Hello";
    char d[] = "HelloX";

    char* aa = (char*)table.InternStr(a);
    char* bb = (char*)table.InternStr(b);
    char* cc = (char*)table.InternStr(c);
    char* dd = (char*)table.InternStr(d);
    _Assert(table.interns.count == 3, "table.interns.count == 3");
    _Assert(aa != (char*)a, "aa != (char*)a");
    _Assert(bb != (char*)b, "bb != (char*)b");
    _Assert(cc != (char*)c, "cc != (char*)c");
    _Assert(dd != (char*)d, "dd != (char*)d");

    _Assert(aa == bb, "&aa == &bb");



    // struct BufElemType {
    //     uint32 x;
    //     uint32 y;
    // };

    // BufElemType* elements = NULL;

    // BufPush(elements, BufElemType, BufElemType({1, 2}));
    // _Assert(elements[0].x == 1 && elements[0].y == 2, "elements[0]->x == 1 && elements[0]->y == 2");

    // BufPush(elements, BufElemType, BufElemType({3, 4}));
    // _Assert(elements[1].x == 3 && elements[1].y == 4, "elements[0]->x == 1 && elements[0]->y == 2");

    // BufferHdr* hdr = BufHdr(elements);
    // _Assert(hdr->count == 2, "hdr->count == 2");

    // BufClear(elements);
    // _Assert(hdr->count == 0 && BufCount(elements) == 0, "hdr->count == 0 && BufCount(elements) == 0");

    // BufFree(elements);
    // _Assert(elements == NULL, "elements == NULL");
    
    // _Assert(a.items != NULL, "a.items != NULL");
    return true;
}

internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Intern test group\n");
    printf("===========================================\n");
    _RunTest(InternInit);
}

} // namespace

#define QTESTINTERN_CPP
#endif
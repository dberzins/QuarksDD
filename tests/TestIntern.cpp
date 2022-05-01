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

    return true;
}

internal uint32 InternInit2()
{

    InternTable table;
    table = {};
    table.Init();
    const char* a;
    const char* b;
    const char* c;
    const char* d;

    a = table.InternStr("IcoSphere");
    b = table.InternStr("Sphere");
    c = table.InternStr("Circle");
    d = table.InternStr("Cylinder");
    
    return true;
}

internal uint32 InternInit3()
{

    InternTable table;
    table = {};
    table.Init();
    char str[] = "IcoSphere";
    const char* s1 = table.InternRange(str, str + strlen(str));
    const char* s2 = table.InternStr("IcoSphere");

    _Assert(s1==s2, "s1 == s2");

    return true;
}

internal void Run()
{
    printf("\n===========================================\n");
    printf("Start Intern test group\n");
    printf("===========================================\n");
    _RunTest(InternInit);
    _RunTest(InternInit2);
    _RunTest(InternInit3);
}

} // namespace

#define QTESTINTERN_CPP
#endif
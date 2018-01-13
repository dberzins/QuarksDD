#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"
#include "TestStack.cpp"
#include "TestArray.cpp"
#include "TestList.cpp"
#include "TestHashtable.cpp"
#include "TestMemory.cpp"
#include "TestJson.cpp"

using namespace QuarksDD;
TestStats stats = {};

namespace QuarksDD {
    MemoryStats memStats = {};
}


int32 main(int32 argc, char **argv)
{
    TestMemory::Run();
    TestStack::Run();
    TestArray::Run();
    TestList::Run();
    TestHashtable::Run();
    TestJson::Run();
    
    // bool32 result = stats.failed == 0;
    // if ( result) {
    //     printf("ALL TESTS PASSED\n");
    // }

    printf("\n-------------------------------------------\n");
    printf("Tests run: %d\n", stats.total);
    printf("Tests passed: %d\n", stats.passed);
    printf("Tests failed: %d\n", stats.failed);

    return 0;
}

// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>

#include "Common.h"
#include "Memory.h"
#include "Array.h"
#include "Hashtable.h"
#include "Json\JsonParser.h"

using namespace QuarksDD;

namespace QuarksDD {
    MemoryStats memStats = {};
}

int32 main(int32 argc, char **argv)
{
    printf("Current dir: %s\n", argv[0]);

    JsonParser parser = {};
    parser.Init("../../tests/data/SampleData.json");
    JValue* root = parser.Parse();
    parser.Free();

    return 0;
}
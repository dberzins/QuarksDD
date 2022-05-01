// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QGRAPH_H)

#include "QuarksDD.h"
#include "Memory.h"
#include "Hashtable.h"
#include "List.h"
#include "Array.h"
#include <stdlib.h>

namespace QuarksDD {
// https://www.softwaretestinghelp.com/graph-implementation-cpp/

struct Edge {
    uint32 start;
    uint32 end;
    real32 weight;
};

struct Graph 
{
    // Data
    bool32 initialized;
    bool32 useLocalArena;
    MemoryArena *arena;

    uint32 edgeCount;
    uint32 nodeCount;

    Edge* edges; 
    Hashtable* adjencies;

    bool32  Init(Edge* edges, uint32 count);
    bool32  Init(MemoryArena *arena, Edge* edges, uint32 count);
    void Free();

    void Build();
    void Print();
    void DepthSearch(uint32 start, Array* result);
    void BreadthSearch(uint32 start, Array* result);
};

} // namespace
#define QGRAPH_H
#endif
// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QLIST_H)

#include "Common.h"
#include "Memory.h"

#include <stdlib.h>

namespace QuarksDD {

// TODO(dainis):
struct ListItem 
{
    // Data 
    void* data;

    ListItem* next;
    ListItem* prev;
};

// TODO(dainis):
struct List
{
    // Data
    bool32 initialized;
    ListItem* head;
    ListItem* tail;
};

} // namespace
#define QLIST_H
#endif
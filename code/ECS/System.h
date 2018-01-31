// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QSYSTEM_H)

#include "../Common.h"
#include "../Array.h"
#include "../FlatArray.h"
#include "../Hashtable.h"

namespace QuarksDD {

struct System;

typedef bool32 (*StartupFn)(System* s, void* context);
typedef bool32 (*ActionFn)(System* s, void* context);
typedef bool32 (*FinishFn)(System* s, void* context);

#define GET_SYSTEM(s, Type) (Type*) ContainerOf(s, Type, system)

struct System {
    // Data 
    uint32 id;
    uint32 type;
    uint64 componentFlags;
    bool32 initialized;
    bool32 useLocalArena;
    MemoryArena* arena;
    
    // Callbacks
    StartupFn StartupCallback;
    ActionFn ActionCallback;
    FinishFn FinishCallback;

    struct {
        FlatArray ids;
        Hashtable componentMap;
    };

    // Operations
    bool32 Init(MemoryArena* arena, uint32 systemId, uint32 systemType);
    void Free();
    void ClearComponentFlags();
    void AddComponentType(uint32 componentType);
    void AddComponentTypes(uint64 flags);
    void RemoveComponentType(uint32 componentType);
    bool32 HasComponent(uint32 componentType);
    bool32 AddComponent(Component* component);
    bool32 RemoveComponent(Component* component);
    bool32 RemoveComponents(ComponentStatus status);
    Array* GetComponents(uint32 componentType);
};

} // namespace
#define QSYSTEM_H
#endif


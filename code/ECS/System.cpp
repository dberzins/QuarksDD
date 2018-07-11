// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Component.h"
#include "Entity.h"
#include "System.h"
#include "EntityManager.h"


namespace QuarksDD {

internal int32 CompareComponentId(Component* c1, Component* c2)
{
    int32 result = -1;

        if (!c1)
            result = 1;
        if (!c2)
            result = -1;

        if (c1->id == c2->id)
            result = 0;
        if (c1->id < c2->id)
            result = -1;
        if (c1->id > c2->id)
            result = 1;

    return result;
}    

internal bool32 MatchComponentStatus(ComponentStatus* status, Component* component)
{
    bool32 result = false;
    if (status && component) {
        result = component->status == *status;
    }
    return result;
}

// internal bool32 MatchComponentType(uint32* type, Component* component)
// {
//     bool32 result = false;
//     if (type && component) {
//         result = component->type == *type;
//     }
//     return result;
// }

internal bool32 MatchIds(uint32* id1, uint32* id2)
{
    bool32 result = false;
    if (id1 && id2) {
        result = *id1 == *id2;
    }
    return result;
}

bool32 System::Init(MemoryArena* arena, uint32 systemId, uint32 systemType) {
    bool32 result = false;
    if (!initialized) {
        id = systemId;
        type = systemType;
        Assert(arena);
        this->arena = arena;
        useLocalArena = false;

         // Total ids array size ARRAY_ITEM_SIZE * (EntityManager::maxComponents + ARRAY_EXTEND)
        // MemorySize idsSize = sizeof(ArrayItem) * (EntityManager::maxComponents + 5) + sizeof(id)* EntityManager::maxComponents;
        // MemoryArena* idsArena = arena->CreateChildArena(idsSize);
        ids = {};
        ids.Init(arena, EntityManager::maxComponents, SizeOf(Component, type), false);

        // Total component type map hastable size 
        // MemorySize mapSize = sizeof(HashItem) * 2 * EntityManager::maxComponents;
        // MemoryArena* mapArena = arena->CreateChildArena(mapSize);
        componentMap = {};
        componentMap.Init(arena, EntityManager::maxComponents);
        
        if (arena) {
            for (uint32 i = 0; i< EntityManager::maxComponents; i++) {
                if (HasComponent(i)) {
                    Array* components = ArenaPushStruct(arena, Array);
                    
                    // Total components array size
                    // MemorySize componentsSize = sizeof(ArrayItem) * (EntityManager::maxEntities + 5);
                    // MemoryArena* componentsArena = arena->CreateChildArena(componentsSize);

                    *components = {};
                    components->Init(arena, EntityManager::maxEntities, 0, true, (CompareFn)CompareComponentId, SortOrder::Asc);
                    componentMap.Add(i, components);

                    ids.Add(&i);
                }
            }
        }

        initialized = true;
        result = true;
    }

    return result;
}

void System::Free() {
    if (initialized) {
        if (useLocalArena) {
            if (arena) {
                arena->Free();
                Deallocate(arena);
                arena = NULL;
            }
        }
    }

    *this = {};
}

void System::ClearComponentFlags() {
    componentFlags &= ~componentFlags;
}

void System::AddComponentType(uint32 componentType) {
    
    if (arena && !HasComponent(componentType) && !componentMap.HasIndex(componentType)) {
        Array* components = ArenaPushStruct(arena, Array);
        
        // Total components array size
        // MemorySize componentsSize = sizeof(ArrayItem) * (EntityManager::maxEntities + 5);
        // MemoryArena* componentsArena = arena->CreateChildArena(componentsSize);

        *components = {};
        components->Init(arena, EntityManager::maxEntities, 0, true, (CompareFn)CompareComponentId, SortOrder::Asc);
        componentMap.Add(componentType, components);
        
        ids.Add(&componentType);
    }
    componentFlags |= (1ULL << componentType);

}

void System::AddComponentTypes(uint64 flags) {
    if (arena) {
        for (uint32 i = 0; i < EntityManager::maxComponents; i++) {
            if (!HasComponent(i) && !componentMap.HasIndex(i)) {
                Array* components = ArenaPushStruct(arena, Array);
                
                // Total components array size
                // MemorySize componentsSize = sizeof(ArrayItem) * (EntityManager::maxEntities + 5);
                // MemoryArena* componentsArena = arena->CreateChildArena(componentsSize);

                *components = {};
                components->Init(arena, EntityManager::maxEntities, 0, true, (CompareFn)CompareComponentId, SortOrder::Asc);
                componentMap.Add(i, components);

                ids.Add(&i);
            }
        }
    }
    componentFlags |= flags;
}

void System::RemoveComponentType(uint32 componentType) {
    componentFlags &= ~(1ULL << componentType);

    if (arena && componentMap.HasIndex(componentType)) {
        Array* components = (Array*)componentMap.GetItem(componentType)->data;
        components->Clear();

        ids.Remove(&componentType, (MatchFn)MatchIds);
    }
}

bool32 System::HasComponent(uint32 componentType) {
    bool32 result = (componentFlags & (1ULL << componentType)) != 0;
    return result;
}

bool32 System::AddComponent(Component* component) {
    bool32 result = false;
    Assert(arena);

    if (arena && component && HasComponent(component->type)) {
        Array* components = GetComponents(component->type);
        if (components) {
            result = components->AddSorted(component) != NULL;
        }
    }
    return result;
}

bool32 System::RemoveComponent(Component* component) {
    bool32 result = false;

    Array* components = GetComponents(component->type);
    int32 pos = components->Find(component);
    if (pos >= 0) {
       result = components->Remove(pos);  
    }

    return result;
}

Array* System::GetComponents(uint32 componentType) {
    Array* result = NULL;

    HashItem* item = componentMap.GetItem(componentType);
    if (item) {
        result = (Array*)item->data;
    }

    return result;
}

bool32 System::RemoveComponents(ComponentStatus status) {
    bool32 result = false;

    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);
        HashItem* item = componentMap.GetItem(type);
        if (item && item->data) {
            Array* components = GetComponents(i);
            if (components) {
                components->Remove(&status, (MatchFn)MatchComponentStatus);
                result = true;
            }
        }
    }

    return result;
}


} // namespace
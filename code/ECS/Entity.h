// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QENTITY_H)

#include "../QuarksDD.h"
#include "../Hashtable.h"
#include "../FlatArray.h"

#include "Component.h"

namespace QuarksDD {

struct EntityManager;
struct Entity;
struct System;
struct Component;




// #define CREATE_COMPONENT(entity, Type) \
//     (Type*) ArenaPushSize(((EntityManager*)(Entity*)entity->ecs)->componentArena, sizeof(Type)) \

// #define CREATE_COMPONENT(arena, Type) \
//     (Type*) ArenaPushSize(((MemoryArena*)(arena), sizeof(Type)) \

// #define ADD_COMPONENT(entity, componentType, c) \
//     (bool32) entity->AddComponent(entity, (uint32)componentType, &c->component)

#define MAX_ENTITY_NAME 30
typedef uint64 EntityId;

enum class EntityStatus {
    Ghost = 0,
    Alive,
    Dead
};

struct Entity {
    // Data 
    EntityId id;
    const char* name;
    // char name[MAX_ENTITY_NAME];
    EntityStatus status;
    uint64 flags;

    struct {
        FlatArray ids;
        Hashtable components;
    };
    
    EntityManager* ecs;
    MemoryArena* arena;
    bool32 initialized;

    // Operations
    bool32 Init(MemoryArena* arena, EntityManager* entityManager, EntityId entityId, const char* name, uint32 nameLen);
    bool32 Init(MemoryArena* arena, EntityManager* entityManager, EntityId entityId, const char* name);
    void Free();

    bool32 HasComponent(uint32 type);
    Component* GetComponent(uint32 type);
    // bool32 AddComponent(Entity* entity, uint32 componentType, Component* component);
    // NOTE: add new component
    bool32 AddComponent(uint32 componentType, Component* component);
    // NOTE: add existing component
    bool32 AddComponent(Component* component);
    bool32 RemoveComponent(Component* component);
    bool32 RemoveComponent(uint32 type);
    void SetComponentsStatus(ComponentStatus status);
    void SetStatus(EntityStatus status);
};

} // namespace
#define QENTITY_H
#endif


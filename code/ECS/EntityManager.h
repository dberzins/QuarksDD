// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QENTITYMANAGER_H)

#include "../QuarksDD.h"
#include "../Memory.h"
#include "../Hashtable.h"
#include "../Array.h"
#include "../FlatArray.h"
#include "Component.h"
#include "Entity.h"
#include "../Intern.h"

namespace QuarksDD {
struct System;

#define MAX_SYSTEMS 32
#define MAX_ENTITIES 4096
// #define MAX_ENTITIES 100000
#define MAX_COMPONENTS 64

#define SYSTEM_DEF(base, systmeType, componentFlags) do { \
    base->system = {}; \
    base->system.type = (u32)systmeType; \
    base->system.AddComponentTypes(componentFlags); \
    base->system.StartupCallback = &base->Startup; \
    base->system.ActionCallback = &base->Action; \
    base->system.FinishCallback = &base->Finish; \
} while(0) 

#define CREATE_SYSTEM(ecs, Type) \
    (Type*) ((Type*)ArenaPushSize(((EntityManager*)ecs)->systemArena, sizeof(Type)))->Init() \

#define ADD_SYSTEM(ecs, sys) \
    (System*) (((EntityManager*)ecs)->AddSystem((System*)&sys->system)) \

#define REGISTER_SYSTEM(ecs, Type) \
    (System*) (((EntityManager*)ecs)->AddSystem( \
        (System*)&(((Type*)CREATE_SYSTEM(ecs, Type))->system) ) \
    ) \

struct EntityManager {
    // Data 
    bool32 initialized;
    InternTable symbols;
    
    local_persist uint32 maxComponents;
    local_persist uint32 maxEntities;
    local_persist uint32 maxSystems;
    EntityId maxEntityId;
    ComponentId maxComponentId;
    uint32 maxSystemId;
    uint32 systemFlags;

    bool32 useLocalArena;
    MemoryArena* systemArena;
    MemoryArena* entityArena;
    MemoryArena* componentArena;

    struct {
        FlatArray ids;
        Hashtable systems;
    };
    
    Array entities; 


    // Operations
    bool32 Init();
    bool32 Init(MemoryArena* systemArena, MemoryArena* entityArena, MemoryArena* componentArena);
    void Free();
    // Systems
    bool32 HasSystem(uint32 type);
    System* GetSystem(uint32 type);
    System* AddSystem(uint32 systemType, System* sys);
    System* AddSystem(System* sys);
    bool32 RemoveSystem(System* sys);

    // System Actions
    bool32 Startup();
    bool32 Startup(uint32 systemFlags, void* context);
    bool32 Run();
    bool32 Run(uint32 systemFlags, void* context);
    void Cleanup();
    bool32 Finish();

    // Entities
    EntityId NextEntityId();
    Entity* CreateEntity(const char* name);
    Entity* CreateEntity(EntityId id, const char* name);
    Entity* CreateEntity(EntityId id, const char* name, uint32 nameLen);
    Entity* FindEntity(EntityId id);
    bool32 HasEntity(EntityId id);
    bool32 AddEntity(Entity* entity);
    bool32 RemoveEntity(Entity* entity);
    // Component
    bool32 AddComponent(Component* component);
    bool32 RemoveComponent(Component* component);
};


} // namespace
#define QENTITYMANAGER_H
#endif


// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Entity.h"
#include "System.h"
#include "EntityManager.h"

namespace QuarksDD {

// internal bool32 MatchSystemType(uint32* type, System* sys)
// {
//     bool32 result = false;
//     if (type && sys) {
//         result = sys->type == *type;
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

internal int32 CompareEntityId(Entity* e1, Entity* e2)
{
    int32 result = -1;

    if (!e1)
        result = 1;
    if (!e2)
        result = -1;

    if (e1->id == e2->id)
        result = 0;
    if (e1->id < e2->id)
        result = -1;
    if (e1->id > e2->id)
        result = 1;

    return result;
}    

internal bool32 MatchEntityStatus(EntityStatus* status, Entity* entity)
{
    bool32 result = false;
    if (status && entity) {
        result = entity->status == *status;
    }
    return result;
}    

uint32 EntityManager::maxComponents;
uint32 EntityManager::maxEntities;
uint32 EntityManager::maxSystems;

bool32 EntityManager::Init() {
    bool32 result = false;
    if (!initialized) {

        symbols = {};
        symbols.Init();

        if (!maxSystems) {
            maxSystems = MAX_SYSTEMS;
        }
        Assert(maxSystems <= (CHAR_BIT*SizeOf(EntityManager, systemFlags)));

        if (!maxEntities) {
            maxEntities = MAX_ENTITIES;
        }
        if (!maxComponents) {
            maxComponents = MAX_COMPONENTS;
        }
        Assert(SizeOf(Entity, flags) == SizeOf(System, componentFlags));
        Assert(maxComponents <= (CHAR_BIT*SizeOf(Entity, flags)));
        Assert(maxComponents <= (CHAR_BIT*SizeOf(System, componentFlags)));

        useLocalArena = true;;

        // TODO(dainis): Calculate precize default sizes for ECS arenas
        systemArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *systemArena = {};
        // systemArena = Sisze of Systems + Size of SystemTypes
        systemArena->Init((64 + sizeof(System)) * EntityManager::maxSystems + EntityManager::maxSystems * SizeOf(System, type));

        entityArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *entityArena = {};
        entityArena->Init(sizeof(Entity) * EntityManager::maxEntities);

        componentArena = (MemoryArena*)Allocate(sizeof(MemoryArena));
        *componentArena = {};
        componentArena->Init((128 + sizeof(Component)) * EntityManager::maxEntities * EntityManager::maxComponents);

        ids = {};
        ids.Init(systemArena, EntityManager::maxSystems, SizeOf(System, type), false);

        systems = {};
        systems.Init(systemArena, EntityManager::maxSystems);

        entities = {};
        entities.Init(entityArena, EntityManager::maxEntities, 0, true, (CompareFn)CompareEntityId, SortOrder::Asc);

        initialized = true;
        result = true;
    }

    return result;
}

bool32 EntityManager::Init(MemoryArena* systemArena, MemoryArena* entityArena, MemoryArena* componentArena) {
    bool32 result = false;
    if (!initialized) {

        symbols = {};
        symbols.Init();

        if (!maxSystems) {
            maxSystems = MAX_SYSTEMS;
        }
        Assert(maxSystems <= (CHAR_BIT*SizeOf(EntityManager, systemFlags)));

        if (!maxEntities) {
            maxEntities = MAX_ENTITIES;
        }
        if (!maxComponents) {
            maxComponents = MAX_COMPONENTS;
        }
        Assert(SizeOf(Entity, flags) == SizeOf(System, componentFlags));
        Assert(maxComponents <= (CHAR_BIT*SizeOf(Entity, flags)));
        Assert(maxComponents <= (CHAR_BIT*SizeOf(System, componentFlags)));

        if (systemArena && entityArena && componentArena) {
            useLocalArena = false;;

            this->systemArena = systemArena;
            this->entityArena = entityArena;
            this->componentArena = componentArena;

            ids.Init(systemArena, EntityManager::maxSystems, SizeOf(System, type), false);

            systems = {};
            systems.Init(systemArena, EntityManager::maxSystems);

            entities = {};
            entities.Init(entityArena, EntityManager::maxEntities, 0, true, (CompareFn)CompareEntityId, SortOrder::Asc);

            initialized = true;
            result = true;
        }
    }

    return result;
}

void EntityManager::Free() {
    if (initialized) {
        if (useLocalArena) {
            if (systemArena) {
                systemArena->Free();
                Deallocate(systemArena);
                systemArena = NULL;
            }
            if (entityArena) {
                entityArena->Free();
                Deallocate(entityArena);
                entityArena = NULL;
            }
            if (componentArena) {
                componentArena->Free();
                Deallocate(componentArena);
                componentArena = NULL;
            }
        }

        symbols.Free();
    }

    *this = {};
}

bool32 EntityManager::HasSystem(uint32 type) {
    bool32 result = (systemFlags & (1UL << type)) != 0;
    return result;
}

System* EntityManager::GetSystem(uint32 type) {
    System* result = NULL;
    if (HasSystem(type) && type < EntityManager::maxSystems) {
        HashItem* item = systems.GetItem(type);
        if (item) {
            result = (System*)item->data;
        }
    }
    return result;
}

System* EntityManager::AddSystem(uint32 systemType, System* sys) {
    System* result = NULL;
    Assert(systemType < EntityManager::maxSystems);

    if (!HasSystem(systemType)) {
        if (sys) {
            sys->Init(systemArena, ++maxSystemId, systemType);
            systems.Add(sys->type, sys);
            systemFlags |= 1UL << sys->type;

            ids.Add(&systemType);
            result = sys;
        }
    }

    return result;
}

System* EntityManager::AddSystem(System* sys) {
    System* result = NULL;
    Assert(sys->type < EntityManager::maxSystems);

    if (!HasSystem(sys->type)) {
        if (sys) {
            sys->Init(systemArena, ++maxSystemId, sys->type);
            systems.Add(sys->type, sys);
            systemFlags |= 1UL << sys->type;

            ids.Add(&sys->type);
            result = sys;
        }
    }

    return result;
}

bool32 EntityManager::RemoveSystem(System* sys) {
    bool32 result = false;
    Assert(sys->type < EntityManager::maxSystems);

    if (HasSystem(sys->type)) {

        HashItem* item = systems.GetItem(sys->type);
        if (item) {
            System* s = (System*)item->data;
            if (s->id == sys->id) {
                systems.Remove(sys->type);
                systemFlags &= ~(1UL << sys->type);

                ids.Remove(&sys->type, (MatchFn)MatchIds);
                result = true;
            }
        }
    }

    return result;
}

bool32 EntityManager::Startup() {
    bool32 result = false;

    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);
        HashItem* item = systems.GetItem(type);
        if (item && item->data) {
            System* s = (System*)item->data;

            if (s->StartupCallback) {
                s->StartupCallback(s, NULL);
            }
        }
    }

    return result;
}

bool32 EntityManager::Startup(uint32 systemFlags, void* context) {
    bool32 result = false;

    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);

        if ((systemFlags & (1UL << type)) != 0) {
            HashItem* item = systems.GetItem(type);
            if (item && item->data) {
                System* s = (System*)item->data;

                if (s->StartupCallback) {
                    s->StartupCallback(s, context);
                    result = true;
                }
            }
        }

    }

    return result;
}

bool32 EntityManager::Run() {
    bool32 result = false;

    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);
        HashItem* item = systems.GetItem(type);
        if (item && item->data) {
            System* s = (System*)item->data;

            if (s->ActionCallback) {
                s->ActionCallback(s, NULL);
                result = true;
            }
        }
    }

    return result;
}

bool32 EntityManager::Run(uint32 systemFlags, void* context) {
    bool32 result = false;

    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);

        if ((systemFlags & (1UL << type)) != 0) {
            HashItem* item = systems.GetItem(type);
            if (item && item->data) {
                System* s = (System*)item->data;

                if (s->ActionCallback) {
                    s->ActionCallback(s, context);
                    result = true;
                }
            }
        }

    }

    return result;
}


void EntityManager::Cleanup()
{
    if (entities.initialized && entities.count) {
        //  Remove DEAD components from systems
        for (uint32 i = 0; i < ids.count; i++) {
            uint32 type = *(uint32*)ids.GetItem(i);
            HashItem* item = systems.GetItem(type);
            if (item && item->data) {
                System* s = (System*)item->data;

                s->RemoveComponents(ComponentStatus::Dead);
            }
        }

        // Remove DEAD entities
        EntityStatus status = EntityStatus::Dead;
        entities.Remove(&status, (MatchFn)MatchEntityStatus);
    }
}

bool32 EntityManager::Finish() {
    bool32 result = false;

    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);
        HashItem* item = systems.GetItem(type);
        if (item && item->data) {
            System* s = (System*)item->data;

            if (s->FinishCallback) {
                s->FinishCallback(s, NULL);
            }
        }
    }

    return result;
}

EntityId EntityManager::NextEntityId() {
    return ++maxEntityId;
}

Entity* EntityManager::CreateEntity(const char* name) {
    return this->CreateEntity(++maxEntityId, name, strlen(name));
}

Entity* EntityManager::CreateEntity(EntityId id, const char* name) {
    return this->CreateEntity(id, name, strlen(name));
}

Entity* EntityManager::CreateEntity(EntityId id, const char* name, uint32 nameLen) {
    Entity* result = NULL;
    Entity* entity = ArenaPushStruct(entityArena, Entity); 
    if (entity) { 
        *entity = {};
        entity->Init(entityArena, this, id, name, nameLen);
        result = entity;
    } 

    return result;
}

Entity* EntityManager::FindEntity(EntityId id) {
    Entity* result = NULL;

    Entity e = {};
    e.id = id;
    int32 pos = entities.Find(&e);
    if (pos >= 0) {
        result = (Entity*)entities.GetValue(pos);
    }

    return result;
}

bool32 EntityManager::HasEntity(EntityId id) {
    bool32 result;

    Entity e = {};
    e.id = id;
    int32 pos = entities.Find(&e);
    result = pos >= 0;

    return result;
}

bool32 EntityManager::AddEntity(Entity* entity) {
    bool32 result = false;

    Assert(entity);
    Assert(entity->initialized);
    Assert(entity->status != EntityStatus::Dead);
    
    if (entity && entity->initialized) { 
        entity->status = EntityStatus::Alive;

        // Add entity components to systems
        for (uint32 i = 0; i < entity->ids.count; i++) {
            uint32 type = *(uint32*)entity->ids.GetItem(i);
            HashItem* item = entity->components.GetItem(type);
            if (item && item->data) {
                Component* component = (Component*)item->data;
                component->status = ComponentStatus::Alive;
                AddComponent(component);

                // if (component->status == ComponentStatus::Alive) {
                //     AddComponent(component);
                // }
            }
        }

        entities.AddSorted(entity);
        // entities.Add(entity);
    } 

    return result;
}

bool32 EntityManager::RemoveEntity(Entity* entity) {
    bool32 result = false;

    Assert(entity);
    Assert(entity->initialized);
    if (entity && entity->initialized) { 

        // Remove entity components from systems
        for (uint32 i = 0; i < entity->ids.count; i++) {
            uint32 type = *(uint32*)entity->ids.GetItem(i);
            HashItem* item = entity->components.GetItem(type);
            if (item && item->data) {
                Component* component = (Component*)item->data;
                if (component) {
                    RemoveComponent(component);
                }
            }
        }

        // Remove entity
        int32 pos = entities.Find(entity);
        if (pos >= 0) {
            entities.Remove(pos);
        }
    } 

    return result;
}

bool32 EntityManager::AddComponent(Component* component) {
    bool32 result = false;

    // Add component to systems
    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);
        HashItem* item = systems.GetItem(type);
        if (item && item->data) {
            System* s = (System*)item->data;

            if (s->HasComponent(component->type)) {
                result = s->AddComponent(component);
            }
        }
    }

    return result;
}

bool32 EntityManager::RemoveComponent(Component* component) {
    bool32 result = false;

    // Remove component from systems
    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)ids.GetItem(i);
        HashItem* item = systems.GetItem(type);
        if (item && item->data) {
            System* s = (System*)item->data;

            if (s->HasComponent(component->type)) {
                result = s->RemoveComponent(component);
            }
        }
    }

    return result;
}



} // namespace
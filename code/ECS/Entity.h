// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QENTITY_H)

#include "../Common.h"
#include "../Memory.h"
#include "../Hashtable.h"
#include "../Array.h"
#include "../List.h"

#include <stdlib.h>

namespace QuarksDD {

#if !defined(REDEFINE_ECS_TYPINGS)
    enum class SystemType {
        System_Count
    };

    enum class ComponentType {
        Component_Count
    };
#endif


struct Entity;
struct System;
typedef bool32 (*InitFn)(System* s);
typedef bool32 (*UpdateFn)();
typedef bool32 (*RenderFn)();

#define MAX_SYSTEMS 32
#define ENTITIES 4096
#define MAX_COMPONENTS 64

struct ComponentTypeId {
    uint32 value;
};

struct ComponentFlags {
    uint64 value;
};

struct ComponentId {
    uint32 value;

    // ++prefix, return reference of this
    ComponentId& operator++() {
        ++value;
        return *this;
    }
    
    // postfix++, return old value by value
    const ComponentId operator++(int32 dummy) {
        ComponentId temp = *this;
        ++value;  
        return temp;  
    }

    bool32 operator==(const ComponentId &other) const {
        bool32 result = value == other.value;
        return result;
    }

    bool32 operator!=(const ComponentId &other) const {
        bool32 result = !(*this == other);
        return result;
    }
};

struct EntityId {
    uint32 value;

    // ++prefix, return reference of this
    EntityId& operator++() {
        ++value;
        return *this;
    }
    
    // postfix++, return old value by value
    const EntityId operator++(int32 dummy) {
        EntityId temp = *this;
        ++value;  
        return temp;  
    }

    bool32 operator==(const EntityId &other) const {
        bool32 result = value == other.value;
        return result;
    }

    bool32 operator!=(const EntityId &other) const {
        bool32 result = !(*this == other);
        return result;
    }
};

#define ADD_SYSTEM(ecs, sys, Type, systemType) do { \
    sys = ArenaPushStruct((MemoryArena*)(&ecs.arena), Type); \
    if (sys) { \
        *sys = {}; \
        sys->system = {};\
        sys->system.Init(++ecs.maxSystemId, (uint32)systemType); \
        sys->system.InitCallback = &Type::Init; \
        Assert(sys->system.initialized); \
        ecs.AddSystem(&sys->system); \
    } \
} while (0)

// #define CREATE_ENTITY(ecs, entity, Type) do { \
//     entity = ArenaPushStruct((MemoryArena*)(&ecs.arena), Type); \
//     if (entity) { \
//         *entity = {}; \
//         entity->Init(++ecs.maxEntityId); \
//     } \
// } while (0)

// #define ADD_ENTITY(ecs, entity) do { \
//     Assert(entity->initialized); \
//     if (entity && entity->initialized) { \
//         ecs.entities.Add(entity); \
//     } \
// } while (0)


// #define ADD_ENTITY(ecs, entity, Type) do { \
//     entity = ArenaPushStruct((MemoryArena*)(&ecs.arena), Type); \
//     if (entity) { \
//         *entity = {}; \
//         entity->Init(++ecs.maxEntityId); \
//         Assert(entity->initialized); \
//         ecs.entities.Add(entity); \
//     } \
// } while (0)

#define CREATE_COMPONENT(ecs, comp, Type, componentType) do { \
    comp = ArenaPushStruct((MemoryArena*)(&ecs.arena), Type); \
    if (comp) {\
        *comp = {}; \
        comp->component = {}; \
        comp->component.Init(++ecs.maxComponentId, (uint32)componentType); \
    } \
} while (0)

#define ADD_COMPONENT(entity, comp) do { \
    if (entity && comp ) {\
        entity->AddComponent(&comp->component); \
    } \
} while (0)

// #define ADD_COMPONENT(ecs, entity, comp, Type, componentType) do { \
//     comp = ArenaPushStruct((MemoryArena*)(&ecs.arena), Type); \
//     if (comp) {\
//         *comp = {}; \
//         comp->component = {}; \
//         comp->component.Init(++ecs.maxComponentId, (uint32)componentType); \
//         Assert(comp->component.initialized); \
//         entity->AddComponent(&comp->component); \
//     } \
// } while (0)

#define GET_SYSTEM(s, Type) (Type*) ContainerOf(s, Type, system)
#define GET_COMPONENT(c, Type) (Type*) ContainerOf(c, Type, component)

struct System {
    uint32 id;
    uint32 type;
    // NOTE: flags specify what type of components will be handled by system 
    uint32 flags;
    bool32 initialized;

    InitFn InitCallback;

    // Hashtable map [type][List of Enitites]
    // Hashtable map;

    bool32 Init(uint32 systemId, uint32 systemType) {
        bool32 result = false;
        if (!initialized) {
            id = systemId;
            type = systemType;

            initialized = true;
            result = true;
        }

        return result;
    }

    void ClearComponentFlags() {
        flags &= ~flags;
    }

    void AddComponentType(uint32 componentType) {
        flags |= (1UL << componentType);
    }

    void RemoveComponentType(uint32 componentType) {
        flags &= ~(1UL << componentType);
    }
};

struct Component {
    ComponentId id;
    uint32 type;

    bool32 initialized;

    bool32 Init( uint32 componentType) {
        bool32 result = false;
        if (!initialized) {
            type = componentType;

            initialized = true;
            result = true;
        }
        return result;
    }

    bool32 Init(ComponentId componentId, uint32 componentType) {
        bool32 result = false;
        if (!initialized) {
            id = componentId;
            type = componentType;

            initialized = true;
            result = true;
        }
        return result;
    }
};

struct Entity {
    EntityId id;
    Hashtable components;
    uint64 flags;
    // ComponentFlags flags;

    bool32 initialized;

    bool32 Init(EntityId entityId) {
        bool32 result = false;
        if (!initialized) {
            components = {};
            components.Init(MAX_COMPONENTS);

            id = entityId;
            initialized = true;
            result = true;
        }
        return result;
    }

    void Free() {
        if (initialized) {
            components.Free();
        }
        *this = {};
    }

    bool32 HasComponent(uint32 type) {
        bool32 result = (flags & (1ULL << type)) != 0;
        return result;
    }

    Component* GetComponent(uint32 type) {
        Component* result = NULL;
        if (HasComponent(type) && type < MAX_COMPONENTS) {
            HashItem* item = components.GetItem(type);
            if (item) {
                result = (Component*)item->data;
            }
        }
        return result;
    }

    bool32 AddComponent(Component* component) {
        bool32 result = false;
        Assert(component->type < MAX_COMPONENTS);

        if (!HasComponent(component->type)) {
            components[component->type] = component;
            flags |= 1ULL << component->type;
            result = true;
        }
        return result;
    }

    bool32 RemoveComponent(Component* component) {
        bool32 result = false;
        Assert(component->type < MAX_COMPONENTS);

        if (HasComponent(component->type)) {

            HashItem* item = components.GetItem(component->type);
            if (item) {
                Component* c = (Component*)item->data;
                if (c->id == component->id) {
                    components.Remove(component->type);
                    flags &= ~(1ULL << component->type);
                    result = true;
                }
            }
        }

        return result;
    }

};

struct EntityManager {
    bool32 initialized;
    
    EntityId maxEntityId;
    ComponentId maxComponentId;
    uint32 maxSystemId;
    uint32 systemFlags;

    MemoryArena arena;

    Hashtable systems;
    Array entities; 

    bool32 Init() {
        bool32 result = false;
        if (!initialized) {

            arena = {};
            arena.Init(4096);

            systems = {};
            systems.Init(MAX_SYSTEMS);

            entities = {};
            entities.Init(ENTITIES);

            initialized = true;
            result = true;
        }

        return result;
    }

    void Free() {
        if (initialized) {
            systems.Free();
            entities.Free();
            arena.Free();
        }

        *this = {};
    }


    bool32 HasSystem(uint32 type) {
        bool32 result = (systemFlags & (1UL << type)) != 0;
        return result;
    }

    System* GetSystem(uint32 type) {
        System* result = NULL;
        if (HasSystem(type) && type < MAX_SYSTEMS) {
            HashItem* item = systems.GetItem(type);
            if (item) {
                result = (System*)item->data;
            }
        }
        return result;
    }

    bool32 AddSystem(System* sys) {
        bool32 result = false;
        Assert(sys->type < MAX_SYSTEMS);

        if (!HasSystem(sys->type)) {
            systems[sys->type] = sys;
            systemFlags |= 1UL << sys->type;
            result = true;
        }
        return result;
    }

    bool32 RemoveSystem(System* sys) {
        bool32 result = false;
        Assert(sys->type < MAX_SYSTEMS);

        if (HasSystem(sys->type)) {

            HashItem* item = systems.GetItem(sys->type);
            if (item) {
                System* s = (System*)item->data;
                if (s->id == sys->id) {
                    systems.Remove(sys->type);
                    systemFlags &= ~(1UL << sys->type);
                    result = true;
                }
            }
        }

        return result;
    }

    bool32 InitSystems() {
        bool32 result = false;

        HashtableIterator* iter = systems.GetIteratorSorted();

        while (iter) {
            HashItem* item = systems.GetItem(iter);
            if (item) {
                System* s = (System*)item->data;
                if (s->InitCallback) {
                    s->InitCallback(s);
                }
            }
            iter = iter->next;
        }

        return result;
    }

    void Update() {
        // for (uint32 i = 0; i < systems.count; i++) {
        //     ((System*)systems[i].data)->Update();
        // }
    }
    
    void Render() {
        // for (uint32 i = 0; i < systems.count; i++) {
        //     ((System*)systems[i].data)->Render();
        // }        
    }

    Entity* CreateEntity() {
        Entity* result;
        result = ArenaPushStruct((MemoryArena*)(&arena), Entity); 
        if (result) { 
            *result = {};
            result->Init(++maxEntityId);
        } 

        return result;
    }

    bool32 AddEntity(Entity* entity) {
        bool32 result = false;

        Assert(entity);
        Assert(entity->initialized);
        if (entity && entity->initialized) { 
            entities.Add(entity);
        } 

        return result;
    }
};

} // namespace
#define QENTITY_H
#endif


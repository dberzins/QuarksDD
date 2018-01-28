// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "Entity.h"
#include "EntityManager.h"

namespace QuarksDD {

internal bool32 MatchComponentType(uint32* type, Component* component)
{
    bool32 result = false;
    if (type && component) {
        result = component->type == *type;
    }
    return result;
}

bool32 Entity::Init(MemoryArena* arena, EntityManager* entityManager, EntityId entityId, const char* name) {
    bool32 result = false;
    if (!initialized && arena) {
        id = entityId;
        this->name = name;
        ecs = entityManager;
        this->arena = arena;

         // Total ids array size ARRAY_ITEM_SIZE * (EntityManager::maxComponents + ARRAY_EXTEND)
        // MemorySize idsSize = sizeof(ArrayItem) * (EntityManager::maxComponents + 5) + sizeof(id)* EntityManager::maxComponents;
        // MemoryArena* idsArena = arena->CreateChildArena(idsSize);
        ids = {};
        ids.Init(arena, EntityManager::maxComponents);

        // Total component type map hastable size 
        // MemorySize componentsSize = sizeof(HashItem) * 2 * EntityManager::maxComponents;
        // MemoryArena* componentsArena = arena->CreateChildArena(componentsSize);
        components = {};
        components.Init(arena, EntityManager::maxComponents);


        initialized = true;
        result = true;
    }
    return result;
}

void Entity::Free() {
    if (initialized) {
        ids.Free();
        components.Free();
    }
    *this = {};
}

bool32 Entity::HasComponent(uint32 type) {
    bool32 result = (flags & (1ULL << type)) != 0;
    return result;
}

Component* Entity::GetComponent(uint32 type) {
    Component* result = NULL;
    if (HasComponent(type) && type < EntityManager::maxComponents) {
        HashItem* item = components.GetItem(type);
        if (item) {
            result = (Component*)item->data;
        }
    }
    return result;
}

bool32 Entity::AddComponent(Entity* entity, uint32 componentType, Component* component) {
    bool32 result = false;
    Assert(componentType < EntityManager::maxComponents);

    if (!HasComponent(componentType)) {
        if (component) {
            component->Init(entity, ++ecs->maxComponentId, componentType);

            components[componentType] = component;
            flags |= 1ULL << componentType;
            

            if (component->status == ComponentStatus::Alive) {
                ecs->AddComponent(component);
            }
            
            ids.AddCopy(componentType);

            result = true;
        }
    }

    return result;
}


bool32 Entity::AddComponent(Component* component) {
    bool32 result = false;
    Assert(component->type < EntityManager::maxComponents);

    if (!HasComponent(component->type)) {
        components[component->type] = component;
        flags |= 1ULL << component->type;

        if (component->status == ComponentStatus::Alive) {
            ecs->AddComponent(component);
        }

        ids.AddCopy(component->type);

        result = true;
    }
    return result;
}

bool32 Entity::RemoveComponent(uint32 type) {
    bool32 result = false;

    Component* component = GetComponent(type);
    if (component) {
        ecs->RemoveComponent(component);
        result = RemoveComponent(component);
    }

    return result;
}

bool32 Entity::RemoveComponent(Component* component) {
    bool32 result = false;
    Assert(component->type < EntityManager::maxComponents);

    if (HasComponent(component->type)) {

        HashItem* item = components.GetItem(component->type);
        if (item) {
            Component* c = (Component*)item->data;
            if (c->id == component->id) {
                components.Remove(component->type);
                flags &= ~(1ULL << component->type);

                ids.Remove(&component->type, (MatchFn)MatchComponentType);
                result = true;
            }
        }
    }

    return result;
}

void Entity::SetComponentsStatus(ComponentStatus status) {
    for (uint32 i = 0; i < ids.count; i++) {
        uint32 type = *(uint32*)(ids.items + i)->data;
        HashItem* item = components.GetItem(type);
        if (item && item->data) {
            Component* component = (Component*)item->data;
            if (component) {
                component->status = status;
            }
        }
    }
}

void Entity::SetStatus(EntityStatus s) {
    switch (s) {
        case EntityStatus::Alive : {
            SetComponentsStatus(ComponentStatus::Alive);
            break;
        }
        case EntityStatus::Dead : {
            SetComponentsStatus(ComponentStatus::Dead);
            break;
        }
    }
    status = s;
}
   
} // namespace
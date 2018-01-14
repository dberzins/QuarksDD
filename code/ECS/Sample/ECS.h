#if !defined(QECSTYPES_H)
#define REDEFINE_ECS_TYPINGS
#include "..\Entity.h"

enum class SystemType {
    SampleSystem = 0,
    SampleRenderSystem,
    System_Count
};

enum class ComponentType {
    Component1 = 0,
    Component2,
    Component3,
    Component_Count
};

#define QECSTYPES_H
#endif
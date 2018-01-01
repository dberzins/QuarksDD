#include "../Common.h"
#include "Json.h"

namespace QuarksDD {

bool32 JValue::Init() {
    bool32 result = false;
    if (!initialized) {
        initialized = true;
        result = true;
    }
    return result;
}

void* JValue::GetValue()
{
    switch (type) {
        case JValueType::Integer : {
            return (void*)&iValue;
        }
        case JValueType::Float : {
            return (void*)&fValue;
        }
        case JValueType::String : {
            return (void*)&sValue;
        }
        case JValueType::Object : {
            return (void*)&oValue;
        }
        case JValueType::Array : {
            return (void*)&aValue;
        }
    }

    return NULL;
}

bool32 JValue::GetValue(int32** value)
{
    if (this == NULL)
        return false;

    if (type == JValueType::Integer) {
        *value = &iValue;
        return true;
    }

    value = NULL;
    return false;
}

bool32 JValue::GetValue(int32& value)
{
    if (this == NULL)
        return false;

    if (type == JValueType::Integer) {
        value = iValue;
        return true;
    }

    return false;
}

bool32 JValue::GetValue(real32** value)
{
    if (this == NULL)
        return false;

    if (type == JValueType::Float) {
        *value = &fValue;
        return true;
    }

    value = NULL;
    return false;
}

bool32 JValue::GetValue(real32& value) 
{
    if (this == NULL)
        return false;

    if (type == JValueType::Float) {
        value = fValue;
        return true;
    }

    return false;
}

bool32 JValue::GetValue(JString** value)
{
    if (this == NULL)
        return false;

    if (type == JValueType::String) {
        *value = &sValue;
        return true;
    }

    value = NULL;
    return false;
}

bool32 JValue::GetValue(JObject** value)
{
    if (this == NULL)
        return false;

    if (type == JValueType::Object) {
        *value = &oValue;
        return true;
    }

    value = NULL;
    return false;
}

bool32 JValue::GetValue(JArray** value)
{
    if (this == NULL)
        return false;

    if (type == JValueType::Array) {
        *value = &aValue;
        return true;
    }

    value = NULL;
    return false;
}

} // namspace
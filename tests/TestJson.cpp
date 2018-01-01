#if !defined(QTESTJSON_CPP)

#include <Common.h>
#include <Memory.h>
#include <Json\JsonParser.h>
#include <stdio.h>
#include <stdlib.h>
#include "Tests.h"

using namespace QuarksDD;
namespace TestJson
{

internal uint32 JsonObject()
{
    JsonParser parser = {};
    parser.Init("../../tests/data/Object.json");
    JValue* root = parser.Parse();

    _Assert(root != NULL, "root != NULL");
    _Assert(root->type == JValueType::Object, "root->type == JValueType::Object");

    JValue* o = root->oValue["Object1"];
    _Assert(o != NULL, "o != NULL");

    JValue* f = NULL;

    f = o->oValue["field1"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 1111, "f->iValue == 1111");

    f = o->oValue["field2"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 2222, "f->iValue == 2222");

    f = o->oValue["field3"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 3333, "f->iValue == 3333");

    parser.Free();
    return true;
}

internal uint32 JsonNestedObject()
{
    JsonParser parser = {};
    parser.Init("../../tests/data/NestedObject.json");
    JValue* root = parser.Parse();

    _Assert(root != NULL, "root != NULL");
    _Assert(root->type == JValueType::Object, "root->type == JValueType::Object");

    JValue* o = root->oValue["Object1"];
    _Assert(o != NULL, "o != NULL");

    JValue* n = o->oValue["NestedObject1"];
    _Assert(n != NULL, "n != NULL");
    _Assert(n->type == JValueType::Object, "n->type == JValueType::Object");

    n = n->oValue["NestedObject2"];
    _Assert(n != NULL, "n != NULL");
    _Assert(n->type == JValueType::Object, "n->type == JValueType::Object");

    JValue* f = n->oValue["field1"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 1111, "f->iValue == 1111");

    f = n->oValue["field2"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 2222, "f->iValue == 2222");

    f = n->oValue["field3"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 3333, "f->iValue == 3333");

    parser.Free();
    return true;
}

internal uint32 JsonScalars()
{
    JsonParser parser = {};
    parser.Init("../../tests/data/Scalars.json");
    JValue* root = parser.Parse();
    _Assert(root != NULL, "root != NULL");
    _Assert(root->type == JValueType::Object, "root->type == JValueType::Object");

    JValue* f = NULL;

    f = root->oValue["field1"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == -1, "f->iValue == -1");

    f = root->oValue["field2"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == 0, "f->iValue == 0");
    uint32 t = (uint32)I32Max;

    f = root->oValue["field3"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue != U32Max, "f->iValue != U32Max");
    _Assert(f->iValue == I32Max, "f->iValue == U16Max");

    f = root->oValue["field4"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
    _Assert(f->iValue == I32Max, "f->iValue == U16Max");

    f = root->oValue["field5"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::Float, "f->type == JValueType::Float");
    _Assert(f->fValue == 1.001f, "f->iValue == 1.001f");

    f = root->oValue["field6"];
    _Assert(f != NULL, "f != NULL");
    _Assert(f->type == JValueType::String, "f->type == JValueType::String");
    _Assert(strncmp(f->sValue.str, "Hello World", f->sValue.len) == 0, "f->sValue.str == \"Hello World\"");

    parser.Free();
    return true;
}

internal uint32 JsonArray()
{
    JsonParser parser = {};
    parser.Init("../../tests/data/Array.json");
    JValue* root = parser.Parse();

    _Assert(root != NULL, "root != NULL");
    _Assert(root->type == JValueType::Object, "root->type == JValueType::Object");

    JValue* a = root->oValue["Array1"];
    _Assert(a != NULL, "a != NULL");
    _Assert(a->type == JValueType::Array, "a->type == JValueType::Array");
    _Assert(a->aValue.values.count == 3, "a->aValue.values.count == 3");

    JValue* v = NULL;
    
    v = (JValue*)a->aValue.values[0].data;
    _Assert(v->iValue == 1, "v->iValue == 1");

    v = (JValue*)a->aValue.values[1].data;
    _Assert(v->iValue == 2, "v->iValue == 2");

    v = (JValue*)a->aValue.values[2].data;
    _Assert(v->iValue == 3, "v->iValue == 3");

    parser.Free();
    return true;
}

internal uint32 JsonNestedArray()
{
    JsonParser parser = {};
    parser.Init("../../tests/data/NestedArray.json");
    JValue* root = parser.Parse();

    _Assert(root != NULL, "root != NULL");
    _Assert(root->type == JValueType::Array, "root->type == JValueType::Array");
    _Assert(root->aValue.values.count == 1, "root->aValue.values.count == 1");

    JValue* a = NULL;

    a = (JValue*) root->aValue.values[0].data;
    _Assert(a != NULL, "a != NULL");
    _Assert(a->type == JValueType::Array, "a->type == JValueType::Array");
    _Assert(a->aValue.values.count == 1, "a->aValue.values.count == 1");
    
    a = (JValue*) a->aValue.values[0].data;
    _Assert(a != NULL, "a != NULL");
    _Assert(a->type == JValueType::Array, "a->type == JValueType::Array");
    _Assert(a->aValue.values.count == 3, "a->aValue.values.count == 3");

    JValue* v = NULL;
    
    v = (JValue*)a->aValue.values[0].data;
    _Assert(v->iValue == 1, "v->iValue == 1");

    v = (JValue*)a->aValue.values[1].data;
    _Assert(v->iValue == 2, "v->iValue == 2");

    v = (JValue*)a->aValue.values[2].data;
    _Assert(v->iValue == 3, "v->iValue == 3");

    parser.Free();
    return true;
}

internal uint32 JsonObjectArray()
{
    JsonParser parser = {};
    parser.Init("../../tests/data/ObjectArray.json");
    JValue* root = parser.Parse();

    _Assert(root != NULL, "root != NULL");
    _Assert(root->type == JValueType::Array, "root->type == JValueType::Array");
    _Assert(root->aValue.values.count == 3, "root->aValue.values.count == 3");

    for (uint32 i = 0; i< root->aValue.values.count; i++) {
        JValue* o = NULL;

        o = (JValue*) root->aValue.values[i].data;
        _Assert(o != NULL, "o != NULL");
        _Assert(o->type == JValueType::Object, "o->type == JValueType::Object");

        JValue* f = NULL;

        HashtableIterator* iter = o->oValue.fields.GetIteratorSorted(SortOrder::Asc);

        uint32 j = 0;
        while (iter) {
            j++;

            JValue* f = (JValue*)iter->item->data;
            _Assert(f != NULL, "f != NULL");
            _Assert(f->type == JValueType::Integer, "f->type == JValueType::Integer");
           
            int32 testValue = i * root->aValue.values.count + j;
            _Assert(f->iValue == testValue, "f->iValue == testValue");

            iter = iter->next;
        }
    }

    parser.Free();
    return true;
}

internal void Run()
{
    printf("===========================================\n");
    printf("Start Json test group\n");
    printf("===========================================\n");
    _RunTest(JsonObject);
    _RunTest(JsonNestedObject);
    _RunTest(JsonScalars);
    _RunTest(JsonArray);
    _RunTest(JsonNestedArray);
    _RunTest(JsonObjectArray);
}

} // namespace

#define QTESTJSON_CPP
#endif

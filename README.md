<div style="text-align:left">
<img src="misc/QuarksDD.png" alt="QuarksDD logo" height="200">
</div>

# QuarksDD

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Simple educational library for generic data structures written in C and C++: 
* [Memory managment](#memory-managment)
* [Array](#array)
* [List](#list)
* [Stack](#stack)
* [Hashtable](#hashtable)
* [Json data structures and parser](#json)
* etc ...

## Project goal

Goal of the project is to have educational generic data structures library with ability to have control over memory usage and managment.
Library is used as part for other side project (custom game engine) which will be main driver force for library evolution. 

Library development tends to fallow [Handmade philosophy](https://handmade.network/manifesto).

Intention is to avoid major C++ code constructions and dependency libraries and keep C++ code base out of the code as much as possible.

## Build

### Requirments

* [CMake >= 3.6](https://cmake.org/)
* C/C++ compiler (MSVC 2015, gcc, clang, etc...)

### Project build

Generate build project using cmake for environment, compiler of your choise.

To generate Visual Studio 2015 project execute from project directory:
```
.\cmake_generate.bat
``` 

For other develpment environments see `CMakeLists.txt` and reference CMake documantation for more details:
```
mkdir build
cd build
cmake ..
make
```

To build QuarksDD library using Visual Studio 2015 execute:
```
.\build_libs.bat
```

Build results (headers, build artifacts) will be placed in `build\Results` directory.

## Tests

To build QuarksDD library tests using Visual Studio 2015 execute:
```
.\build_tests.bat
```
Tests executable will be placed in `build\Debug\` directory.

To run tests execute:
```
cd build\Debug\
.\Tests.exe > Tests.log
cat Tests.log
```
## API

### Classes vs Structs

Structs are used instead of classes as in major cases we want to be `public` to the whole world from the beginig, but still can choose to be `private` and hide some secrets if needed.   


### Construcors/Destructors

No C++ class costructors/destructors are used as we are not using any of new/delete operators and we hate C++ implicit constructor/destructor calls. Instead we are relaying on good old `structs` and explicit initializtion/deinitilaization calls. 

Template struct:
```c
struct SomeStruct 
{
    // Data
    bool32 initialized;

    // Operations
    bool32 Init() {
        bool32 result = false;
        if (!initialized) {
            // Do some init staff if needed
            initilaized = true;
            result = true;
        }

        return result;
    }

    void Free() {
         if (initialized) {
            // Do some deinitialization staff if needed
        }
        *this = {};       
    }
}
```
Complex struct initializtion/deinitializtion:

```c
    SomeStruct foo = {};
    foo.Init();

    // ... Do some staff with foo

    foo.Free();

```
Simple struct initializtion:
```c
    SimpleStruct bar = {};
    
    // ... Do some staff with bar

```

### Memory managment

#### Generic API
Memory managment wrapper macros for `stdlib.h` API used through whole code base. Feel free to redefine those to your taste:
```
Allocate - malloc(...) wrapper
Deallocate - free(...) wrapper 
Reallocate - realloc(...) wraper
```

#### Memory arena
Memory arena provides more controlled way for memory handling and provides tools for allocating and tracing chunks of large memory blocks, thus simplifying and speeding up allocation and release of memory resources.

Helper macros:
```
ArenaPushStruct(arena, Type) - Push some type (struct/class) to arena
ArenaPushArray(arena, count, Type) - Push array of some type to arena
ArenaPushSize(arena, size) - Push some arbitary size of bytes to arena
```

Usage example 1:
```c
    // Initialization
    MemoryArena arena = {};
    // Init arena with 1024 byte block size
    arena.Init(1024);

    // Push and reserves 100 bytes of memory to arena
    char* data = (char*)ArenaPushSize(&arena, 100);

    // Push some type (struct/class) to arena
    SomeItem* item = (SomeItem*) ArenaPushStruct(&arena, SomeItem);

    // Push array of some type to arena
    SomeItem* itemArray = (SomeItem*) ArenaPushArray(&arena, 10, SomeItem);
 
     // If push size is greater than free space in initalized block then new block with 
     // needed size will be allocated and chained in memory arena
    char* bigdata = (char*)ArenaPushSize(&arena, 4096);

    // Free all allocated resources
    arena.Free();
```

Usage example 2:
```c
    // Initialize arena with undefined block size
    MemoryArena arena = {};
    arena.Init();
    
    // Create and reserves arena memory block with 4096 byte block size  
    char* data = (char*)ArenaPushSize(&arena, 4096);

    // Free all allocated resources
    arena.Free();
```
### Array

Array represents collection of `ArrayItem` elements which holds pointer to arbitary data:

```c
struct ArrayItem 
{
    // Data 
    void* data;

    ...
}
```

Usage example:

```c
    // Create array
    Array a = {};
    // By default inits to array of 5 elments with 5 elements extend factor
    a.Init();

    // Add some values to array
    int32 value1 = 1;
    a.Add(&value1);

    int32 value2 = 2;
    a.Add(&value2);

    // Insert value3 in position 1
    int32 value3 = 3;
    a.Insert(1, &value3);

    // Remove value1
    a.Remove(0);

    // Print array content
    for (uint32 i = 0; i < a.count; i++) {
        int32* value = (int32*)a[i].data;
        printf("%d, ", *value);
    }
    // Result: 3, 2

    a.Free();

```
Array intialization with defined size:

```c
    Array a = {};
    // Inits to array of 10 elments with 5 elements extend factor
    a.Init(10);
    ...
```
Array with external memory arena:

```c
    MemoryArena arena = {};
    arena.Init(4096);

    Array a = {};
    // Inits to array of 10 elments with 5 elements extend factor
    a.Init(&arena, 10);
    ...

    a.Free();
    arena.Free();
```

Sorted array:
```c

int32 CompareValues(void* v1, void* v2)
{
   int32 result = 0;

   int32 value1 = *(Int32*)v1;
   int32 value2 = *(Int32*)v2;

   if (value1 == value2)
       result = 0;
   if (value1 < value2)
       result = -1;
   if (value1 > value2)
       result = 1;

   return result;
}

void TestSortedArray()
{
    Array a = {};
    // Inits sorted array with descending sort order
    a.Init(5, true, CompareValues, SortOrder::Desc);

    // Unsorted values
    int32 values[] = {3, 5, 2, 1, 4};

    for (uint32 i = 0; i < 5; i++>) {
        a.AddSorted(&values[i]);
    }

    // Print array content
    for (uint32 i = 0; i < a.count; i++) {
        int32* value = (int32*)a[i].data;
        printf("%d, ", *value);
    }
    // Result: 5, 4, 3, 2, 1

    // Sort array with ascending sort order
    a.Sort(CompareValues, SortOrder::Asc, SortMethod::QuickSort);

    // Print array content
    for (uint32 i = 0; i < a.count; i++) {
        int32* value = (int32*)a[i].data;
        printf("%d, ", *value);
    }
    // Result: 1, 2, 3, 4, 5

    a.Free();

}
```

### List

List represents double linked list of `ListItem` elements which holds pointer to arbitary data:

```c
struct ListItem 
{
    // Data 
    void* data;

    ListIterator iterator;
}
```

Usage example:

```c
    // Create list
    List l = {};
    // By default reserves meomry for 5 elments with 5 elements extend factor
    l.Init();

    // Add some values to array
    int32 value1 = 1;
    l.Add(&value1);

    int32 value2 = 2;
    ListItem* item2 = l.Add(&value2);

    // Insert value3 in front of list
    int32 value3 = 3;
    l.Insert(NULL, &value3);

    // Insert value4 in end of list
    int32 value4 = 4;
    l.Insert(l.tail, &value4);

    // Insert value5 after value2
    int32 value5 = 5;
    l.Insert(&item2->iterator, &value5);

    // Remove value1
    l.Remove(l.head);

    // Remove value2
    l.Remove(&item2->iterator);

    // Print list content
    ListIterator* iter = l.head;
    while (iter) {
        int32* value = (int32*)l.GetItem(iter)->data;
        printf("%d, ", *value);

        iter = iter->next;
    }
    // Result: 1, 5, 4

    l.Free();

```
List intialization with start size:

```c
    List l = {};
    // Inits to list of 10 elments with 5 elements extend factor
    l.Init(10);
    ...
```
List with external memory arena:

```c
    MemoryArena arena = {};
    // Arena with starting memory for 100 list items
    arena.Init(100*sizeof(ListItem));

    List l = {};
    l.Init(&arena);
    ...

    l.Free();
    arena.Free();
```

List sorting:
```c

int32 CompareValues(void* v1, void* v2)
{
   int32 result = 0;

   int32 value1 = *(Int32*)v1;
   int32 value2 = *(Int32*)v2;

   if (value1 == value2)
       result = 0;
   if (value1 < value2)
       result = -1;
   if (value1 > value2)
       result = 1;

   return result;
}

void TestListSorting()
{
    Array l = {};
    l.Init();

    // Unsorted values
    int32 values[] = {3, 5, 2, 1, 4};

    for (uint32 i = 0; i < 5; i++>) {
        l.Add(&values[i]);
    }

    // Sort list with ascending sort order
    l.Sort(CompareValues, SortOrder::Asc, SortMethod::QuickSort);

    // Print list content
    ListIterator* iter = l.head;
    while (iter) {
        int32* value = (int32*)l.GetItem(iter)->data;
        printf("%d, ", *value);

        iter = iter->next;
    }
    // Result: 1, 2, 3, 4, 5

    l.Free();
}
```
### Stack

Collection of LIFO 'StackItem' elements which holds pointer to arbitary data:

```c
    struct StackItem
    {
        // Data 
        void* data;
        ...
    };

```

Usage example: 
```c
    int32 values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Stack s = {};
    s.Init(10);

    for(uint32 i = 0; i < 10; i++) {
        s.Push(&values[i]);
    }

    while(!s.IsEmpty()) {
        int32* value = (int32*)s.Pop();
        printf("%d, ", *value);
    }
    // Print result: 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    
    s.Free();
```

### Hashtable

Hashtable of `HashItem` elements which holds pointer to arbitary data:

```c
    struct HashItem
    {
        // Data 
        uint32 index;
        char* key;

        void* data;
        ...
    };

```
Usage example:
```c
    Hashtable table = {};
    // Table size defaults to 4096
    table.Init();

    int32 a = 1;
    int32 b = 2;
    int32 c = 3;

    table["a"] = &a;
    table["b"] = &b;
    table["c"] = &c;

    int32 d = *((int32*)table["a"].data);
    int32 e = *((int32*)table["b"].data);
    int32 f = *((int32*)table["c"].data);

    table.Free();
```
Hashtable intialization with defined size:
```c
    Hashtable table = {};
    // Table size will be rounded to higher power of two 512
    table.Init(500);

    // Do staff with table
    // ...

    table.Free();
```

Hashtable with external memory arena:
```c
    MemoryArena arena = {};
    arena.Init();

    Hashtable table = {};
    table.Init(&arena);

    // Do staff with table
    // ...

    table.Free();
    arena.Free();

```
Hashtable items undefined order iterator:
```c
    Hashtable table = {};
    table.Init(4);

    uint32 values[5];

    for (uint32 i=0; i < 5; i++) {
        values[i] = i;
        table[i].data = &values[i]; 
    }

    HashtableIterator* iter;
    // Returns items undefined order iterator
    iter = table.GetIterator();

    while(iter) {
        uint32 value = *(uint32*)iter->item->data;
        printf("%d, ", value);
        iter = iter->next;
    }

    // Print result: 0, 4, 1, 2, 3,

    table.Free();

```
Hashtable items sorted order iterator:
```c
    Hashtable table = {};
    table.Init(4);

    uint32 values[5];

    for (uint32 i=0; i < 5; i++) {
        values[i] = i;
        table[i].data = &values[i]; 
    }

    HashtableIterator* iter;
    // Returns items sorted order iterator - order in which items added to table
    iter = table.GetIteratorSorted();

    while(iter) {
        uint32 value = *(uint32*)iter->item->data;
        printf("%d, ", value);
        iter = iter->next;
    }

    // Print result: 0, 1, 2, 3, 4

    table.Free();

```
### Json

#### JValue
JValue represents parsed Json result generic data type.

```c
    struct JValue 
    {
        // Data 
        bool32 initialized;
        JValueType type;
        union {
            int32   iValue;
            real32  fValue;
            JString sValue;
            JObject oValue;
            JArray  aValue;
        };
        ...
    }
```

JValue can be roselved to one of matching subtype:
* Integer
* Float
* String
    ```c
    struct JString {
        // Data
        char* str;
        uint32 len;
    };
    ```
* Object
    ```c
    struct JObject {
        // Data 
        Hashtable fields;
        ...
    }
    ```
* Array
    ```c
    struct JArray {
        // Data
        Array values;
    };
    ```

#### Json Parser
Basic `Json` like content parser. 
Currently it does not fallow any standarts as well UTF8 support is not yet implementd!

Usage example:

```json
// SampleData.json
{
    "Integer1": 1,
    "Float1": 1.001,
    "String1": "Hello some string",
    "Objec1": {
        "field1" : 1111, 
        "field2" : 2222, 
        "field3" : 2333
    },
    "Array1": ["1", "2", "3"]
}
```

```c
    JsonParser parser = {};
    parser.Init("SampleData.json");
    JValue* root = parser.Parse();

    if (root && root->type == JValueType::Object) {
        JValue* fieldValue = NULL;

        // Read 'Integer1' field value
        fieldValue = root->oValue["Integer1"];
        if (fieldValue && fieldValue->type == JValueType::Integer) {
            printf("'Integer1': %d\n", fieldValue->iValue);
        }

        // Read 'Float1' field value
        fieldValue = root->oValue["Float1"];
        if (fieldValue && fieldValue->type == JValueType::Float) {
            printf("'Float1': %.3f\n", fieldValue->fValue);
        }

        // Read 'String1' field value
        fieldValue = root->oValue["String1"];
        if (fieldValue && fieldValue->type == JValueType::String) {
            printf("'String1': '%s'\n", fieldValue->sValue.str);
        }

        // Read 'Objec1' field value
        fieldValue = root->oValue["Objec1"];
        if (fieldValue && fieldValue->type == JValueType::Object) {
            printf("'Objec1':\n");

            // Iterate over object fields
            HashtableIterator* iter = fieldValue->oValue.fields.GetIteratorSorted(SortOrder::Asc);

            while (iter) {
                char* key = iter->item->key;
                JValue* value = (JValue*)iter->item->data;

                if (value && value->type == JValueType::Integer) {
                    printf("\t'%s': %d\n", key, value->iValue);
                }

                iter = iter->next;
            }
        }

        // Read 'Array1' field value
        fieldValue = root->oValue["Array1"];
        if (fieldValue && fieldValue->type == JValueType::Array) {
            printf("'Array1': ");

            // Iterate over array values
            for (uint32 i = 0; i< fieldValue->aValue.values.count; i++) {
                JValue* value = (JValue*)fieldValue->aValue.values[i].data;

                if (value && value->type == JValueType::String) {
                    printf("'%s', ", value->sValue.str);
                }
            }

        }
    }

    // Print result:
    //    'Integer1': 1
    //    'Float1': 1.001
    //    'String1': 'Hello some string'
    //    'Objec1':
    //            'field1': 1111
    //            'field2': 2222
    //            'field3': 2333
    //    'Array1': '1', '2', '3',

    parser.Free();
```

## Roadmap

Support of other data structure types will fallow:
* Lists
* Queues
* Trees
* Graphs
* ECS
* ... 




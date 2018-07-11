// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QQUARKSDD_H)

#include <stdint.h>
#include <float.h>
#include <stddef.h>
#include <limits.h>

#include <stdlib.h>
#include <stdio.h>

namespace QuarksDD {

    // Typedefs/Defines from: Handmade Hero
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;
    typedef int32 bool32;

    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef float real32;
    typedef double real64;

    typedef int8 s8;
    typedef int8 s08;
    typedef int16 s16;
    typedef int32 s32;
    typedef int64 s64;
    typedef bool32 b32;

    typedef uint8 u8;
    typedef uint8 u08;
    typedef uint16 u16;
    typedef uint32 u32;
    typedef uint64 u64;

    typedef real32 r32;
    typedef real64 r64;
    typedef real32 f32;
    typedef real64 f64;

    typedef size_t SizeType;
    typedef uintptr_t uintptr;

    // Code from: stdint.h
    #define I8Min   (-127i8 - 1)
    #define I16Min  (-32767i16 - 1)
    #define I32Min  (-2147483647i32 - 1)
    #define I64Min  (-9223372036854775807i64 - 1)
    #define I8Max   127i8
    #define I16Max  32767i16
    #define I32Max  2147483647i32
    #define I64Max  9223372036854775807i64
    #define U8Max   0xffui8
    #define U16Max  0xffffui16
    #define U32Max  0xffffffffui32
    #define U64Max  0xffffffffffffffffui64

    #define F32Max FLT_MAX
    #define F32Min -FLT_MAX

    #if !defined(internal)
    #define internal static
    #endif
    #define local_persist static
    #define global_variable static

    #define Pi32 3.14159265359f
    #define Tauint32 6.28318530717958647692f

    #define Minimum(a, b) ((a < b) ? (a) : (b))
    #define Maximum(a, b) ((a > b) ? (a) : (b))

    #define ClampMax(a, max) Minimum(a, max)
    #define ClampMin(a, min) Maximum(a, min)

    // #define AlingnDown(n, a) ((n) & ~((a) - 1))
    // #define AlignUp(n, a) AlignDown((n) + (a) - 1, (a))
    // #define AlignDownPtr(p, a) ((void*)AlignDown((uintptr)(p), (a)))
    // #define AlignUpPtr(p, a) ((void*)AlignUp((uintptr)(p), (a)))

    #define Flag32(value) (u32) 1UL << (u32)value
    #define Flag64(value) (u64) 1ULL << (u32)value

    #define SizeOf(Type, member) ((SizeType) sizeof(((Type *)0)->member))
    #define OffsetOf(Type, member) ((SizeType)(&((Type *)0)->member))
    #define ContainerOf(pointer, Type, member) (Type *)((uint8 *)(pointer) - OffsetOf(Type, member))
    // http://www.drdobbs.com/a-portable-typeof-operator/184401310
    // http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2000/0011/gibbons/list1.htm
    // #define TypePtrOfType(Type) decltype((Type *)0)
    // #define TypeOf(expression) decltype(expression)

    // NOTE: Error handling
    #if QUARKSDD_SLOW
    #define Assert(Expression) if(!(Expression)) {*(int32 *)0 = 0;}
    #else
    #define Assert(Expression)
    #endif

    #define InvalidCodePath Assert(!"InvalidCodePath");

    // NOTE: Sorting
    typedef int32 (*CompareFn)(void* item1, void* item2);
    typedef bool32 (*MatchFn)(void* item1, void* item2);
    
    enum class SortOrder {
        Asc,
        Desc
    };

    enum class SortMethod {
        None,
        BubbleSort,
        QuickSort
    };
    

    #define IsPowOf2(x) (((x) != 0) && ((x) & ((x)-1)) == 0)
    // NOTE: Rounding functions
    // Code from: https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    inline uint32 RoundUpToPowerOf2(uint32 v) {
        v--;    
        for (SizeType i = 1; i < sizeof(v) * CHAR_BIT; i *= 2) {
            v |= v >> i;
        }
        return ++v;
    }

} // namespace
#define QQUARKSDD_H
#endif
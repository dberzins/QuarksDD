#if !defined(QCOMMON_H)

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

    typedef size_t TypeSize;

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

    #define Minimum(A, B) ((A < B) ? (A) : (B))
    #define Maximum(A, B) ((A > B) ? (A) : (B))


    // NOTE: Error handling
    #if QUARKSDD_SLOW
    #define Assert(Expression) if(!(Expression)) {*(int32 *)0 = 0;}
    #else
    #define Assert(Expression)
    #endif

    #define InvalidCodePath Assert(!"InvalidCodePath");

    // NOTE: Sorting
    typedef int32 (*CompareFn)(void* item1, void* item2);
    
    enum class SortOrder {
        Asc,
        Desc
    };

    enum class SortMethod {
        None,
        BubbleSort,
        QuickSort
    };

    // NOTE: Rounding functions
    // Code from: https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
    inline uint32 RoundUpToPowerOf2(uint32 v) {
        v--;
        for (size_t i = 1; i < sizeof(v) * CHAR_BIT; i *= 2) {
            v |= v >> i;
        }
        return ++v;
    }

    // NOTE: Hash functions
    // Code from: https://en.wikipedia.org/wiki/MurmurHash
    inline uint32 Murmur3x32(const char *key, size_t len, uint32 seed)
    {
        uint32 h = seed;
        if (len > 3) {
            const uint32 *key_x4 = (const uint32 *)key;
            size_t i = len >> 2;
            do {
                uint32 k = *key_x4++;
                k *= 0xcc9e2d51;
                k = (k << 15) | (k >> 17);
                k *= 0x1b873593;
                h ^= k;
                h = (h << 13) | (h >> 19);
                h = (h * 5) + 0xe6546b64;
            } while (--i);
            key = (const char *)key_x4;
        }
        if (len & 3) {
            size_t i = len & 3;
            uint32 k = 0;
            key = &key[i - 1];
            do {
                k <<= 8;
                k |= *key--;
            } while (--i);
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
        }
        h ^= len;
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h;
    }
} // namespace
#define QCOMMON_H
#endif
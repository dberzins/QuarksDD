// Murmur Hash functions
// Code from: https://en.wikipedia.org/wiki/MurmurHash

#if !defined(QMURMUR_H)

#include "Common.h"

namespace QuarksDD {

    inline uint32 Murmur3x32(const char *key, SizeType len, uint32 seed)
    {
        uint32 h = seed;
        if (len > 3) {
            const uint32 *key_x4 = (const uint32 *)key;
            SizeType i = len >> 2;
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
            SizeType i = len & 3;
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
#define QMURMUR_H
#endif
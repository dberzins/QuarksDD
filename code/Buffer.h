// Stretchy buffers, invented (?) by Sean Barrett 
// Derived implementation from Per Vognsen. 
//      Code From: https://github.com/pervognsen/bitwise/blob/master/ion/common.c
// NOTICE: This code is related to Bitwise and is released into the public domain.
//      If you have derived benefit from Bitwise or have incorporated parts of Bitwise directly into your own 
//      work and products,  Bitwise's author, Per Vognsen, would appreciate an email describing how and where it was used, 
//      but you are under no legal obligation to do so.

#if !defined(QBUFFER_H)

#include "QuarksDD.h"
#include "Memory.h"
#include <stdarg.h>

namespace QuarksDD {

struct BufferHdr {
    SizeType size;  // Total capacity in elements
    SizeType count; // Used count in elements
    char buffer[1];
};

#define BufHdr(b) ((BufferHdr *)((char *)(b) - OffsetOf(BufferHdr, buffer)))

#define BufCount(b) ((b) ? BufHdr(b)->count : 0)
#define BufSize(b) ((b) ? BufHdr(b)->size : 0)
#define BufEnd(b) ((b) + BufCount(b))
#define BufSizeOf(b) ((b) ? BufCount(b) * sizeof(*b) : 0)

#define BufFree(b) ((b) ? (Deallocate(BufHdr(b)), (b) = NULL) : 0)
#define BufFit(b, Type, n) ((n) <= BufSize(b) ? 0 : ((b) = (Type*)BufExtend_((b), (n), sizeof(*(b)))))
#define BufPush(b, Type, ...) (BufFit((b), Type, 1 + BufCount(b)), (b)[BufHdr(b)->count++] = (__VA_ARGS__))
#define BufPrintf(b, ...) ((b) = BufPrintf_((b), __VA_ARGS__))
#define BufClear(b) ((b) ? BufHdr(b)->count = 0 : 0)

internal void* BufExtend_(const void* buf, SizeType newCount, SizeType elemSize) {
    Assert(BufSize(buf) <= (SIZE_MAX - 1) / 2);
    SizeType newSize = ClampMin(2 * BufSize(buf), Maximum(newCount, 16));
    Assert(newCount <= newSize);
    Assert(newSize <= (SIZE_MAX - OffsetOf(BufferHdr, buffer)) / elemSize);
    SizeType newMemSize = OffsetOf(BufferHdr, buffer) + newSize * elemSize;
    BufferHdr *newHdr;
    if (buf) {
        newHdr = (BufferHdr*)XReallocate(BufHdr(buf), newMemSize);
    } else {
        newHdr = (BufferHdr*)XAllocate(newMemSize);
        newHdr->count = 0;
    }
    newHdr->size = newSize;
    return newHdr->buffer;
}

internal char* BufPrintf_(char* buf, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    SizeType size = BufSize(buf) - BufCount(buf);
    SizeType n = 1 + vsnprintf(BufEnd(buf), size, fmt, args);
    va_end(args);
    if (n > size) {
        BufFit(buf, char, n + BufCount(buf));
        va_start(args, fmt);
        SizeType newSize = BufSize(buf) - BufCount(buf);
        n = 1 + vsnprintf(BufEnd(buf), newSize, fmt, args);
        Assert(n <= newSize);
        va_end(args);
    }

    BufHdr(buf)->count += n - 1;
    return buf;
}

} // namespace
#define QBUFFER_H
#endif
// Copyright (c) 2018 Dainis Berzins, All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#if !defined(QTESTS_H)
#include <stdio.h>      
#include <stdarg.h>  

struct TestStats {
    unsigned int total;
    unsigned int passed;
    unsigned int failed;
};
extern struct TestStats stats; 

#define _Assert(cond, ...) do { \
    if (!(cond)) { \
        printf("[FAIL] "); \
        printf("%s() line %d : [ ", __func__, __LINE__); \
        printf(__VA_ARGS__); \
        printf(" ]\n"); \
        return 0; \
    } \
    else { \
        printf("[PASS] "); \
        printf("%s() line %d : [ ", __func__, __LINE__); \
        printf(__VA_ARGS__); \
        printf(" ]\n"); \
    } \
} while (0)

#define _RunTest(test) do { \
    unsigned int pass = (unsigned int)test(); \
    stats.passed += pass; \
    stats.failed += !pass; \
    stats.total++; \
} while (0)

 #define QTESTS_H
 #endif
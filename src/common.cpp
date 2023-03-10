#pragma once
/* This file is a part of pdfcook program, which is GNU GPLv2 licensed */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE// for asprintf()
#endif

#include <string>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstddef>
#include "common.h"
extern bool repair_mode;

typedef unsigned int uint;

// M_PI is not available in mingw32, so using and defining PI
#define PI 3.14159265358979323846

// check if string s1 starts with s2
#define starts(s1, s2)  (strncmp(s1,s2,strlen(s2)) == 0)

// define MAX and MIN macros if not already defined
#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b) ? (a):(b))
#endif

// read a big endian integer provided as char array
int arr2int(char *arr, int len)
{
    unsigned char tmp[4] = {};
    for (int i=0; i<len; i++) {
        tmp[4-len+i] = arr[i];
    }
    // network byte order is big endian, eg. int 16 is stored as 0x000010 in 3 bytes
    return ((uint)tmp[0]<<24 | (uint)tmp[1]<<16 | (uint)tmp[2]<<8 | (uint)tmp[3] );
}

// implementation of asprintf for platforms that do not have it
#if (!HAVE_ASPRINTF)
#include <stdarg.h>
int asprintf(char **strp, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int size = vsnprintf(nullptr, 0, fmt, ap) + 1; // get the required size
    va_end(ap);
    *strp = (char*)malloc2(size); // use malloc2 to allocate memory
    va_start(ap, fmt);
    int ret = vsnprintf(*strp, size, fmt, ap);
    va_end(ap);
    return ret;
}
#endif

// like %f but strips trailing zeros
std::string double2str(double real)
{
    char buf[32]; // allocate a fixed-size buffer
    int len = snprintf(buf, sizeof(buf), "%.15g", real);
    if (len <= 0) {
        return ""; // return empty string if snprintf failed
    }
    if (buf[0] == '-') { // check if the number is negative
        len++; // add one for the negative sign
    }
    if (buf[len-1] == '.') { // check if there are trailing zeros
        len--;
    }
    while (len > 1 && buf[len-1] == '0' && buf[len-2] != '.') { // strip trailing zeros
        len--;
    }
    return std::string(buf, len);
}

// wrapper for malloc that exits the program on failure
inline void* malloc2(size_t size)
{
    void *ptr = malloc(size);
    if (size != 0 && !ptr) {
        fprintf(stdout, "error: malloc() failed!\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

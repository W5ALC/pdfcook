#pragma once
/* This file is a part of pdfcook program, which is GNU GPLv2 licensed */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE // for asprintf()
#endif
#include "config.h"
#include <string>
#include <cstdio>
#include <cstring> // memcpy and other string func
#include <cstdlib> // exit()
#include <cassert>
#include <cmath>

extern bool repair_mode;

// M_PI is not available in mingw32, so using and defining PI
#define PI 3.14159265358979323846

#if (!HAVE_ASPRINTF)
int asprintf(char **strp, const char *fmt, ...);
#endif

// read a big endian integer provided as char array
int arr2int(char *arr, int len);

// like %f but strips trailing zeros
std::string double2str(const double& num);

// like malloc() but exits program when fails. use this where little memory
// is needed, and where we cannot ignore the allocation failure
inline void* malloc2(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stdout, "error: malloc() failed!\n");
        exit(1);
    }
    return ptr;
}

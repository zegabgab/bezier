#ifndef _ARRAYS_H
#define _ARRAYS_H

#include <stdlib.h>
#include <errno.h>

#define ARRAY_INITIAL_CAPACITY 8

#define DOUBLE_CAPACITY(array, capacity, error) {   \
    typeof(capacity) new_c =                        \
        capacity == 0                               \
            ? ARRAY_INITIAL_CAPACITY                \
            : (capacity << 1);                      \
    typeof(array) bigger = realloc(                 \
            array,                                  \
            new_c * sizeof(typeof(*array)));        \
    int success = bigger != NULL;                   \
    error = success ? 0 : errno;                    \
    array = success ? bigger : array;               \
    capacity = success ? new_c : capacity;          \
}

#endif

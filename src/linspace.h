#ifndef _LINSPACE_H
#define _LINSPACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define for_linspace(start, end, count, index, variable)                                \
    typeof(start) (variable) = (start);                                                 \
    size_t (index) = 0;                                                                 \
    for ( ; i < (count);                                                                \
        (variable) = (start) + ((end) - (start)) * (++i / ((double) (count) - 1.)))

#ifdef __cplusplus
}
#endif

#endif

#ifndef _LINSPACE_H
#define _LINSPACE_H

#ifdef __cplusplus
extern "C" {
#endif

#define for_linspace(start, end, count, variable) (variable) = (start);                 \
    for (int i = 0;                                                                     \
        i < (count);                                                                    \
        (variable) = (start) + ((end) - (start)) * (++i / ((double) (count) - 1.)))

#ifdef __cplusplus
}
#endif

#endif

#pragma once
#include <stdlib.h>
#include <sys/time.h>

typedef struct {
    struct timeval startTime;
} ElapsedTime;

inline
void elapsed_start(ElapsedTime* t) {
    gettimeofday(&(t->startTime), NULL);
}

inline
double elapsed_time(ElapsedTime* t) {
    struct timeval endTime;
    gettimeofday(&endTime, NULL);
    return (endTime.tv_sec + endTime.tv_usec * 1E-6)
           - (t->startTime.tv_sec + t->startTime.tv_usec * 1E-6);
}

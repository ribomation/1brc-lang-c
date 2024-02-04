#pragma once
#include <stdlib.h>
#include <math.h>

enum {
    STATION_MAX_LENGTH = 32,
    MAX_KEY_STEPS = 7,
};

typedef struct sEntry {
    char station[STATION_MAX_LENGTH];
    unsigned count;
    double sum;
    double min;
    double max;
    unsigned long key;
    struct sEntry* next;
} Entry;

typedef struct {
    char* station;
    double temperature;
} ParseResult;

unsigned long   key_of(const char* station);
int             entry_comparator(const void* a, const void* b);
void            entry_dump(Entry* self);
void            entry_print(Entry* self);

inline
void entry_parse(ParseResult* self, char* CSV) {
    char* semicolon = strchr(CSV, ';');
    if (semicolon == NULL) {
        fprintf(stderr, "missing ; in '%s'", CSV);
        exit(1);
    }
    *semicolon = '\0';

    self->station = CSV;
    self->temperature = atof(semicolon + 1);
}

inline
Entry* entry_init(Entry* self, const char* station, double temperature) {
    strcpy(self->station, station);
    self->count = 1;
    self->sum = self->min = self->max = temperature;
    self->key = 0;
    self->next = NULL;
    return self;
}

inline
Entry* entry_new(const char* station, double temperature) {
    return entry_init((Entry*) malloc(sizeof(Entry)), station, temperature);
}

inline
Entry* entry_update(Entry* self, double temperature) {
    self->count++;
    self->sum += temperature;
    self->min = fmin(temperature, self->min);
    self->max = fmax(temperature, self->max);
    return self;
}




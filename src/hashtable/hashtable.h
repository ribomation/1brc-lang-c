#pragma once
#include "entity.h"

enum {
    MAX_BUCKETS = 997,
};

typedef struct {
    Entry*      buckets[MAX_BUCKETS];
    unsigned    count;
} HashTable;

void    hashtable_dispose(HashTable* self);
Entry*  hashtable_get(HashTable* self,const char* station);
void    hashtable_stats(HashTable* self);

Entry** hashtable_entries_alloc(HashTable* self);
Entry** hashtable_entries(HashTable* self, Entry** entries);


inline
void hashtable_insert(HashTable* self, Entry* entry) {
    unsigned long key = key_of(entry->station);
    unsigned int index = key % MAX_BUCKETS;
    entry->key = key;
    entry->next = self->buckets[index];
    self->buckets[index] = entry;
    self->count++;
}




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void hashtable_insert(HashTable* self, Entry* entry);

Entry* hashtable_get(HashTable* self, const char* station) {
    unsigned long key = key_of(station);
    unsigned int index = key % MAX_BUCKETS;
    Entry* bucket = self->buckets[index];
    for (Entry* e = bucket; e != NULL; e = e->next) {
        if (e->key == key) return e;
    }
    return NULL;
}

Entry** hashtable_entries(HashTable* self, Entry** entries) {
    unsigned index = 0;
    for (unsigned k = 0; k < MAX_BUCKETS; ++k) {
        if (self->buckets[k] != NULL) {
            for (Entry* e = self->buckets[k]; e != NULL; e = e->next) {
                entries[index++] = e;
            }
        }
    }
    qsort(entries, self->count, sizeof(Entry*), entry_comparator);
    return entries;
}

Entry** hashtable_entries_alloc(HashTable* self) {
    Entry** entries = (Entry**) calloc(self->count, sizeof(Entry*));
    return hashtable_entries(self, entries);
}

void hashtable_stats(HashTable* self) {
    printf("-- HashTable Stats --\n");
    printf("count=%d, #buckets=%d, util=%.1f%%\n",
           self->count, MAX_BUCKETS, 100.0 * self->count / MAX_BUCKETS);

    unsigned max_bucket_size = 0;
    unsigned sum_bucket_size = 0;
    for (unsigned k = 0; k < MAX_BUCKETS; ++k) {
        if (self->buckets[k] != NULL) {
            unsigned cnt = 0;
            for (Entry* e = self->buckets[k]; e != NULL; e = e->next) ++cnt;
            if (cnt > max_bucket_size) max_bucket_size = cnt;
            sum_bucket_size += cnt;
        }
    }
    printf("max bucket size=%d, avg bkt siz=%.1f\n",
           max_bucket_size, (double) sum_bucket_size / self->count);
    printf("-- END --\n");
}

static void entry_dispose(Entry* e) {
    if (e == NULL) return;
    entry_dispose(e->next);
    free(e);
}

void hashtable_dispose(HashTable* self) {
    for (unsigned k = 0; k < MAX_BUCKETS; ++k) {
        if (self->buckets[k] != NULL) {
            entry_dispose(self->buckets[k]);
        }
    }
}


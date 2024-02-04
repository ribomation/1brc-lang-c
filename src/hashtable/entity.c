#include <stdio.h>
#include <string.h>
#include "entity.h"

void entry_parse(ParseResult* self, char* CSV);
Entry* entry_init(Entry* self, const char* station, double temperature);
Entry* entry_new(const char* station, double temperature);
Entry* entry_update(Entry* self, double temperature);


void entry_print(Entry* self) {
    printf("%s: %.2f C, %.1f/%.1f (%d)\n",
           self->station,
           self->sum / self->count,
           self->min,
           self->max,
           self->count
    );
}

int entry_comparator(const void* a, const void* b) {
    const Entry* lhs = *(const Entry**) a;
    const Entry* rhs = *(const Entry**) b;
    return strcmp(lhs->station, rhs->station);
}

unsigned long key_of(const char* station) {
    //https://stackoverflow.com/questions/7666509/hash-function-for-string
    //http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    unsigned k = 0;
    for (unsigned char ch = *station; ch != '\0' && k < MAX_KEY_STEPS; ch = *++station, ++k) {
        hash = ((hash << 5) + hash) + ch;
    }
    return hash;
}

void entry_dump(Entry* self) {
    printf("Entry{key=%lu, cnt=%d, sum=%.1f, min=%.1f, max=%.1f, station=%s}\n",
           self->key,
           self->count,
           self->sum,
           self->min,
           self->max,
           self->station
    );
}


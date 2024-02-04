#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "elapsed/elapsed-time.h"


int main(int argc, char** argv) {
    ElapsedTime t;
    elapsed_start(&t);

    char* filename = "../../../data/weather-data-1M.csv";
    if (argc > 1) {
        filename = argv[1];
    }
    printf("filename: %s\n----\n", filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "cannot open '%s'", filename);
        exit(1);
    }

    // Entry heap
    enum { ENTRY_STORAGE_SIZE = 512 };
    Entry   entry_storage[ENTRY_STORAGE_SIZE] = {0};
    Entry*  entry_storage_next = &entry_storage[0];

    // Hash/table
    HashTable data = {0};

    char CSV[128] = {};
    while (fgets(CSV, sizeof CSV, file) != NULL) {
        ParseResult parseResult;
        entry_parse(&parseResult, CSV);
        Entry* e = hashtable_get(&data, parseResult.station);
        if (e == NULL) {
            e = entry_storage_next++;
            entry_init(e, parseResult.station, parseResult.temperature);
            hashtable_insert(&data, e);
        } else {
            entry_update(e, parseResult.temperature);
        }
    }
    fclose(file);

    // Entries list
    Entry* entries[ENTRY_STORAGE_SIZE] = {0};
    hashtable_entries(&data, entries);
    for (unsigned k = 0; k < data.count; ++k) {
        entry_print(entries[k]);
    }
    printf("----\n");

    fprintf(stderr, "[C w/o malloc] elapsed %.3f seconds, %s\n", elapsed_time(&t), filename);
}

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

    HashTable data = {};
    char CSV[128] = {};
    while (fgets(CSV, sizeof CSV, file) != NULL) {
        ParseResult parseResult;
        entry_parse(&parseResult, CSV);
        Entry* e = hashtable_get(&data, parseResult.station);
        if (e == NULL) {
            e = entry_new(parseResult.station, parseResult.temperature);
            hashtable_insert(&data, e);
        } else {
            entry_update(e, parseResult.temperature);
        }
    }
    fclose(file);

    Entry** entries = hashtable_entries_alloc(&data);
    for (unsigned k = 0; k < data.count; ++k) {
        entry_print(entries[k]);
    }
    free(entries);
    hashtable_dispose(&data);
    printf("----\n");

    fprintf(stderr, "[C w malloc] elapsed %.3f seconds, %s\n", elapsed_time(&t), filename);
}

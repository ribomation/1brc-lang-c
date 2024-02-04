#include "catch2/catch_test_macros.hpp"
#include <cstdlib>

extern "C" {
#include "hashtable.h"
}
#define ARR_SIZE(arr)   (sizeof(arr) / sizeof(arr[0]))

#include <string>

using namespace std::string_literals;

typedef struct {
    char name[STATION_MAX_LENGTH];
    double temperature;
} Measurement;

Measurement data[] = {
        {"Uppsala",   10.0},
        {"Stockholm", -3.0},
        {"Göteborg",  -1.0},
        {"Umeå",      -15.0},
        {"Uppsala",   5.0},
        {"Stockholm", 6.0},
        {"Uppsala",   -3.0},
};
constexpr unsigned data_size = ARR_SIZE(data);


SCENARIO("simple hashtable usage") {
    GIVEN("a hash-table") {
        HashTable tbl{};

        THEN("is should start as empty") {
            REQUIRE(tbl.count == 0);
        }

        WHEN("inserting a single entry") {
            hashtable_insert(&tbl, entry_new("Uppsala", -5.5));

            THEN("the count == 1") {
                REQUIRE(tbl.count == 1);
            }

            AND_WHEN("retrieving the entry") {
                Entry* e = hashtable_get(&tbl, "Uppsala");

                THEN("it should be found") {
                    REQUIRE_FALSE(e == NULL);
                    REQUIRE(e->count == 1);
                    REQUIRE(e->sum == -5.5);
                }
            }

            AND_WHEN("looking for something else") {
                Entry* e = hashtable_get(&tbl, "Sundsvall");

                THEN("it should NOT be found") {
                    REQUIRE(e == NULL);
                }
            }
        }

        hashtable_dispose(&tbl);
    }
}


SCENARIO("typical hashtable usage") {
    GIVEN("a hash-table and a bunch of data") {
        HashTable tbl{};

        WHEN("inserting all data") {
            for (unsigned k = 0; k < data_size; ++k) {
                Entry* e = hashtable_get(&tbl, data[k].name);
                if (e == NULL) {
                    e = entry_new(data[k].name, data[k].temperature);
                    hashtable_insert(&tbl, e);
                } else {
                    entry_update(e, data[k].temperature);
                }
            }

            THEN("there should be 4 items") {
                REQUIRE(tbl.count == 4);
            }

            AND_WHEN("fetching Uppsala") {
                Entry* e = hashtable_get(&tbl, "Uppsala");

                THEN("its count == 3 and sum == 12") {
                    REQUIRE_FALSE(e == NULL);
                    REQUIRE(e->count == 3);
                    REQUIRE(e->sum == 12.0);
                    REQUIRE(e->min == -3.0);
                    REQUIRE(e->max == 10.0);
                }
            }
        }

        hashtable_dispose(&tbl);
    }
}


SCENARIO("hashtable entries") {
    GIVEN("a hash-table and a bunch of data") {
        HashTable tbl{};

        WHEN("inserting all data") {
            for (unsigned k = 0; k < data_size; ++k) {
                Entry* e = hashtable_get(&tbl, data[k].name);
                if (e == NULL) {
                    e = entry_new(data[k].name, data[k].temperature);
                    hashtable_insert(&tbl, e);
                } else {
                    entry_update(e, data[k].temperature);
                }
            }

            THEN("there should be 4 items, as before") {
                REQUIRE(tbl.count == 4);
            }

            AND_WHEN("getting all entries, as an array of pointers") {
                Entry** entries = hashtable_entries_alloc(&tbl);

                THEN("it should be sorted, indeed") {
                    std::string names[] = {
                            "Göteborg", "Stockholm", "Umeå", "Uppsala"
                    };
                    constexpr unsigned names_size = ARR_SIZE(names);
                    for (unsigned k = 0; k < names_size; ++k) {
                        REQUIRE(names[k] == entries[k]->station);
                    }
                }

                free(entries);
            }
        }

        hashtable_dispose(&tbl);
    }
}


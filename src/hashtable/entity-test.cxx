#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

using Catch::Matchers::StartsWith;
using Catch::Matchers::EndsWith;
using Catch::Matchers::ContainsSubstring;
using Catch::Matchers::Equals;

extern "C" {
#include "entity.h"
}

#include <cstdio>
#include <string>
#include <functional>

using namespace std::string_literals;


TEST_CASE("should be possible to parse a CSV string") {
    char CSV[] = "Uppsala City;123.4\n";

    ParseResult r;
    entry_parse(&r, CSV);

    REQUIRE(r.station == "Uppsala City"s);
    REQUIRE(r.temperature == 123.4);
}

TEST_CASE("should be possible to initialize an entry object") {
    Entry e;
    entry_init(&e, "Stockholm City", 7);
    REQUIRE(e.count == 1);
    REQUIRE(e.sum == 7);
    REQUIRE(e.station == "Stockholm City"s);
}

TEST_CASE("updating an entry object twice, should set count=3") {
    Entry e;
    entry_init(&e, "Uppsala", 0);
    entry_update(&e, +1);
    entry_update(&e, -1);

    REQUIRE(e.count == 3);
    REQUIRE(e.sum == 0);
    REQUIRE(e.min == -1);
    REQUIRE(e.max == +1);
}

TEST_CASE("should be possible to generate hash keys") {
    unsigned long key = key_of("Stockholm City");
    REQUIRE(key == 229441708521312);

    unsigned long key2 = key_of("Stockholm City");
    REQUIRE(key == key2);

    unsigned long key3 = key_of("Stocksund");
    REQUIRE_FALSE(key == key3);
    REQUIRE(key3 == 229441708521681);
}

auto capture_stdout(std::function<void()> stmts) -> std::string {
    char buf[10'000] = {};
    freopen("/dev/null", "a", stdout);
    setvbuf(stdout, buf, _IOFBF, sizeof(buf));
    stmts();
    fflush(stdout);
    freopen("/dev/tty", "a", stdout);
    return buf;
}

TEST_CASE("should be possible to print an entry object") {
    Entry e;
    entry_init(&e, "Stockholm City", 7);

    auto content = capture_stdout([&e]() {
        entry_print(&e);
    });
    REQUIRE_THAT(content, StartsWith("Stockholm City"));
    REQUIRE_THAT(content, ContainsSubstring("7.00 C"));
    REQUIRE_THAT(content, EndsWith("(1)\n"));
    REQUIRE_THAT(content, Equals("Stockholm City: 7.00 C, 7.0/7.0 (1)\n"));
}

TEST_CASE("should be possible to dump one entry") {
    Entry e;
    entry_init(&e, "Uppsala", 0);
    entry_update(&e, 1);
    entry_update(&e, 2);
    entry_update(&e, 3);
    auto content = capture_stdout([&e]() {
        entry_dump(&e);
    });
    REQUIRE_THAT(content, StartsWith("Entry{key="));
    REQUIRE_THAT(content, EndsWith("station=Uppsala}\n"));
    REQUIRE_THAT(content, ContainsSubstring("cnt=4"));
}


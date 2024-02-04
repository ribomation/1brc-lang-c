cmake_minimum_required(VERSION 3.27)
project(c C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 23)
set(WARN -Wall -Wextra -Werror -Wfatal-errors)


Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)


add_library(hashtable OBJECT
        src/hashtable/entity.h
        src/hashtable/entity.c
        src/hashtable/hashtable.h
        src/hashtable/hashtable.c
)
target_include_directories(hashtable PUBLIC src/hashtable)
target_link_libraries(hashtable PUBLIC m)


add_library(elapsed OBJECT
        src/elapsed/elapsed-time.h
        src/elapsed/elapsed-time.c
)
target_include_directories(elapsed PUBLIC src/elapsed)


add_executable(calc-with-malloc
        src/calc-avg-with-malloc.c)
target_compile_options(calc-with-malloc PRIVATE ${WARN})
target_link_libraries(calc-with-malloc hashtable elapsed)


add_executable(calc-without-malloc
        src/calc-avg-without-malloc.c)
target_compile_options(calc-without-malloc PRIVATE ${WARN})
target_link_libraries(calc-without-malloc hashtable elapsed)


add_executable(unit-test EXCLUDE_FROM_ALL
        src/hashtable/entity-test.cxx
        src/hashtable/hashtable-test.cxx
)
target_compile_options(unit-test PRIVATE ${WARN} -Wno-unused-result)
target_link_libraries(unit-test
        PRIVATE Catch2::Catch2WithMain
        PRIVATE hashtable
)


find_program(VALGRIND name valgrind)
if (VALGRIND-NOTFOUND)
    message(WARNING "valgrind not found. install with 'sudo apt install valgrind'")
else ()
    set(MEMCHK --leak-check=full --show-leak-kinds=all )
    set(DATAFILE ../../../data/weather-data-100K.csv)

    add_custom_target(mem-check-1
            COMMAND valgrind ${MEMCHK} $<TARGET_FILE:calc-with-malloc> ${DATAFILE} > /dev/null
    )
    add_custom_target(mem-check-2
            COMMAND valgrind ${MEMCHK} $<TARGET_FILE:calc-without-malloc> ${DATAFILE} > /dev/null
    )

    add_custom_target(mem-check)
    add_dependencies(mem-check mem-check-1 mem-check-2)
endif ()



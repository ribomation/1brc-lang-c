# 1BRC - Språk: C
Källkod till artikeln om 1BRC i språket C
https://www.ribomation.se/blog/2024/1brc-c-med-eller-utan-malloc/

## Applikations-Byggnation
Du kan enkelt bygga projektet med `cmake`.

    cmake -S . -B bld
    cmake --build bld 

## Exekvering
För att kunna köra programmet behöver du först geneera en eller flera indata-filer. 
Program för att göra det finns på annan plats. Kika i början av denna artikel-serie
https://www.ribomation.se/blog/tags/1brc/

Kör programmen på följande vis

    ./bld/calc-with-malloc data/file.csv
    ./bld/calc-without-malloc data/file.csv




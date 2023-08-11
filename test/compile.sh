#!/bin/sh
# GCC 8.0 and use the filesystem experimental TS library, so the stdc++fs should be added
ARGS="-g -O2 -DNDEBUG -fPIC -fno-semantic-interposition -Wall -std=gnu++17 -c"

echo "compile test1"
g++ ${ARGS} test1.cc -I../src/
g++ test1.o -L../lib/ -o test1 -lnsim2023 -lstdc++fs -lpthread
echo "compile test1 done"

echo "compile test2"
g++ ${ARGS} test2.cc -I../src/
g++ test2.o -L../lib/ -o test2 -lnsim2023 -lstdc++fs -lpthread
echo "compile test2 done"

echo "compile test3"
g++ ${ARGS} test3.cc -I../src/
g++ test3.o -L../lib/ -o test3 -lnsim2023 -lstdc++fs -lpthread
echo "compile test3 done"

echo "compile test4"
g++ ${ARGS} test4.cc -I../src/
g++ test4.o -L../lib/ -o test4 -lnsim2023 -lstdc++fs -lpthread
echo "compile test4 done"


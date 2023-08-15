#!/bin/sh
# GCC 8.0 and use the filesystem experimental TS library, so the stdc++fs should be added
ARGS="-g -O2 -DNDEBUG -fPIC -fno-semantic-interposition -Wall -std=gnu++17 -c"

echo "compile OLSR"
g++ ${ARGS} address.cc -I../src/
g++ ${ARGS} buffer.cc -I../src/
g++ ${ARGS} chunk.cc -I../src/
g++ ${ARGS} header.cc -I../src/
g++ ${ARGS} ipv4-address.cc -I../src/
g++ ${ARGS} olsr-header.cc -I../src/
g++ ${ARGS} olsr-state.cc -I../src/
g++ ${ARGS} tag-buffer.cc -I../src/
echo "compile OLSR done"


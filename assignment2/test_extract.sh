#!/bin/bash

tar -xzf a2-ece650.tar.gz
cd a2-ece650
rm a2-ece650
make clean
make all
cat ../test.in | ./a2-ece650

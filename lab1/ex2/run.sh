#!/bin/sh

make
./ex2 < big_test.in | diff -Z big_test.out -
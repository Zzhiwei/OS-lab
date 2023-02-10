#!/bin/sh

make
./ex3 small_test.in > res.out
diff -Z res.out small_test.out
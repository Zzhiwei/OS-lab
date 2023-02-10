#!/bin/sh

make
./ex3 sample.in > res.out
diff -Z res.out sample.out
#!/bin/bash

make ..

while read p; do
  "Input $p"
  echo "$p" | ./build/main
done <./tests/math-expressions.clox

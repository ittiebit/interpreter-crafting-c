#!/bin/bash

make ..

while read p; do
  echo "[INPUT] $p";
  echo "$p" | ./build/main;
done <./tests/math-expressions.clox

#! /bin/bash

for test in $(seq 1 10); do
    echo "---------------------------------------------------"
    echo "test: $test"
    time ./tp2 < samples/input${test}.txt > samples/result${test}.txt
    diff samples/result${test}.txt samples/output${test}.txt
    echo "---------------------------------------------------"
    echo ""
done
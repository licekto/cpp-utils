#!/bin/bash

if [[ $# != 1 ]]
then
    >&2 echo "Provide a path to the generated coverage files"
    exit 1
fi

lcov -c -d $1 -o coverage.info
genhtml -o $1/coverage-report coverage.info

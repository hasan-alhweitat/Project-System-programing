#!/bin/bash

file1="hash.txt"
file2="hashResult.txt"


if diff "$file1" "$file2" > /dev/null; then
  echo "The file is same file"
else
  echo "The file are different other file"
fi

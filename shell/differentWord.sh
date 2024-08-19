#!/bin/bash
echo "Enter one word: "
read word1

echo "Enter two words: "
read word2 


if [ "$word1" == "$word2" ]; then
    echo "The words are the same."
else
    echo "The words are different."
fi

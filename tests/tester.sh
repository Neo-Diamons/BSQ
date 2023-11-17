#!/usr/bin/env bash

if [[ $# != 3 ]]
then
    echo "Usage: $0 <binary> <map_folder> <solved_map_folder>"
    exit 1
fi

BINARY=$1
MAP_FOLDER=$2
SOLVED_MAP_FOLDER=$3

if [[ ! -f $BINARY || ! -x $BINARY ]]
then
    echo "Binary $BINARY does not exist or is not executable"
    exit 1
fi

if [[ ! -d $MAP_FOLDER || ! -d $SOLVED_MAP_FOLDER ]]
then
    echo "Map or solved map folder $MAP_FOLDER does not exist"
    exit 1
fi

SUCCESS=0
FAILURE=0
SKIP=0

for f in "$MAP_FOLDER"/*
do
    echo -e "\e[34mTesting\e[0m $f"
    $BINARY "$f" > tmp
    RETURN=$?
    if [[ ! -f $SOLVED_MAP_FOLDER/$(basename "$f") && $RETURN != 1 ]] \
      || [[ -f $SOLVED_MAP_FOLDER/$(basename "$f") && $(diff tmp "$SOLVED_MAP_FOLDER/$(basename "$f")") != "" ]]
    then
        FAILURE=$((FAILURE+1))
        echo -e "\e[31mTest failed [$RETURN]\e[0m"
    else
        SUCCESS=$((SUCCESS+1))
        echo -e "\e[32mTest passed\e[0m"
    fi
done

rm tmp

echo -e "\e[34mTotal tests execute $((SUCCESS+FAILURE+SKIP))\e[0m\n" \
        "- \e[32m$SUCCESS tests passed\e[0m\n"                       \
        "- \e[31m$FAILURE tests failed\e[0m\n"                       \
        "- \e[33m$SKIP tests skipped\e[0m"

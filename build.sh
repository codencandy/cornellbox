#! bin/bash

FRAMEWORKS='-framework AppKit'
FLAGS='-std=c++20 --debug'
TIMEFORMAT=%R

platform()
{
    clang++ ${FRAMEWORKS} CNC_Main.mm -o cornellbox ${FLAGS}
}

main()
{
    time platform
    CODE_SIZE=$(cloc --exclude-dir=libs . | grep -o -E '([0-9]+)' | tail -1)
    echo "-> LINES OF CODE: " $CODE_SIZE
}

main

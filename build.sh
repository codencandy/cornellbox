#! bin/bash

FRAMEWORKS='-framework AppKit -framework CoreVideo -framework Metal -framework MetalKit'
FLAGS='-std=c++20 --debug'
TIMEFORMAT=%R
IGNORE='-Wno-nullability-completeness'

platform()
{
    clang++ ${FRAMEWORKS} CNC_Main.mm -o cornellbox ${FLAGS} ${IGNORE}
}

main()
{
    time platform
    CODE_SIZE=$(cloc --exclude-dir=libs . | grep -o -E '([0-9]+)' | tail -1)
    echo "-> LINES OF CODE: " $CODE_SIZE
}

main

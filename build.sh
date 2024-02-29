#! bin/bash

FRAMEWORKS='-framework AppKit -framework CoreVideo -framework Metal -framework MetalKit -framework GameController'
FLAGS='-std=c++20 --debug -Ilibs/imgui -Ilibs/imgui/backends' 
TIMEFORMAT=%R
IGNORE='-Wno-nullability-completeness -Wno-unused-command-line-argument'
BUILDTYPE=%1

all()
{
    clang++ ${FRAMEWORKS} -c CNC_ImGui.mm ${FLAGS} ${IGNORE}
    platform
}

platform()
{
    clang++ ${FRAMEWORKS} CNC_Main.mm -o cornellbox CNC_ImGui.o ${FLAGS} ${IGNORE}
}

main()
{
    if [ "$BUILD_TYPE" = "ALL" ]
    then
        time all
    else
        time platform
    fi
    
    CODE_SIZE=$(cloc --exclude-dir=libs . | grep -o -E '([0-9]+)' | tail -1)
    echo "-> LINES OF CODE: " $CODE_SIZE
}

main

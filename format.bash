#/bin/bash

## run clang format twice in series, as otherwise some ines are 
## not stable

find ./src/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i
find ./src/ -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i


mkdir build
cmake -S . -B build
cmake --build build --config Debug -j %NUMBER_OF_PROCESSORS%
cmake --build build --config Release -j %NUMBER_OF_PROCESSORS%

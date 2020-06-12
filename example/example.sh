g++ -std=c++17 ../src/input_generator.cpp  -o input_generator;
./input_generator;
g++ -std=c++17 ../src/benchmark.cpp  -o benchmark -pthread;
./benchmark;
g++ -std=c++17 ../src/ui.cpp  -o ui -pthread;
./ui;


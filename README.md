# Advanced C++ Final Project
This project is a basic formatter of protobuf. It also includes some helper file to facilitate the input data generation and benchmarking. Use example.sh to compile and run the program. 
## Protobuf Formatter
- ui.cpp: command line user interface, include main function.
- node.h: define different type fo node, each type represents a data format of protobuf.
- factory.h: use meta-programming to define a node factory.
- format.h: define formatter for a tree.
- indent.h: define IndentStream for formatter.
- build_tree.h: add children to the root and build tree recursively
- parser.h: parse the input data, get tokens to build tree.
## Input Generator
- input\_generator.cpp: generate input to be formatted, The input generated is for demonstration and is not valid protobuf data because it contains duplicate element.
- input\_generator.h: library for input_generator.cpp
## Benchmark
- benchmark.cpp: format two probobuf file and compare the duration with and without multi-threading.
- benchmark.h: library for benchmark.cpp.
- Results show that single-thread program has better performance in both case. I think it is because this is not a program that requires much computation, and multi-threading increase the overhead. 


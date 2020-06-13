#include "node.h"
#include "format.h"
#include "indent.h"
#include "factory.h"
#include "parser.h"
#include "build_tree.h"
#include "benchmark.h"
#include <functional>
#include <iostream>
using namespace std;
using namespace mpcs;
using std::function;

void construct_nested_tree() {
	vector<string> tokens = Tokenizer().tokenize("nested.proto").get_tokens();
	// build the same tree for 10 times.
	for (int i = 0; i < 10; i++)	{
		vector<string> temp = tokens;
		unique_ptr<Node> root = build_tree(temp);
	}
}

void construct_parallel_tree() {
	vector<string> tokens = Tokenizer().tokenize("parallel.proto").get_tokens();
	// build the same tree for 10 times.
	for (int i = 0; i < 10; i++)	{
		vector<string> temp = tokens;
		unique_ptr<Node> root = build_tree(temp);
	}
}

int main() {
	config_init("config.txt");
	// enable multi-thread
	benchmark(10, construct_nested_tree); // muti-thread
	benchmark(10, construct_parallel_tree);
/* output in my computer is:
Run the function for 10 times, total duration is 9844 ms.
Run the function for 10 times, total duration is 622 ms.
*/	
	// disable multi-thread
	config_init("single_thread_config.txt");
	benchmark(10, construct_nested_tree); 
	benchmark(10, construct_parallel_tree); 
/*
Run the function for 10 times, total duration is 4406 ms.
Run the function for 10 times, total duration is 500 ms.
*/
// In both cases, single thread application has better performance.
}
#include "node.h"
#include "format.h"
#include "indent.h"
#include "factory.h"
#include "parser.h"
#include "build_tree.h"
#include "benchmark.h"
#include <functional>
using namespace std;
using namespace mpcs;
using std::function;

void construct_nested_tree(int size_to_async) {
	vector<string> tokens = Tokenizer().tokenize("nested.proto").get_tokens();
	// build the same tree for 10 times.
	for (int i = 0; i < 10; i++)	{
		vector<string> temp = tokens;
		unique_ptr<Node> root = build_tree(temp, size_to_async);
	}
}

void construct_parallel_tree(int size_to_async) {
	vector<string> tokens = Tokenizer().tokenize("parallel.proto").get_tokens();
	// build the same tree for 10 times.
	for (int i = 0; i < 10; i++)	{
		vector<string> temp = tokens;
		unique_ptr<Node> root = build_tree(temp, size_to_async);
	}
}

int main() {
	benchmark(10, construct_nested_tree, 20); // muti-thread
	// use binding
	function<void()> construct_tree_single_thread = bind(construct_nested_tree, 100000000);
	benchmark(10, construct_tree_single_thread); // disable multi-thread
/* output in my computer is:
Run the function for 10 times, total duration is 20419 ms.
Run the function for 10 times, total duration is 109376 ms.
*/	
	benchmark(10, construct_parallel_tree, 20); // enable multi-thread
	benchmark(10, construct_parallel_tree, 100000000); // disable multi-thread
/*
Run the function for 10 times, total duration is 1813 ms.
Run the function for 10 times, total duration is 5080 ms.
*/
}
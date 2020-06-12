#include "input_generator.h"
using namespace std;
using namespace mpcs;
int main() {
	int max_depth = 100;
	ProtoGeneratorVar generator = NestedProtoGenerator(15);
	ofstream fout1("nested.proto", ofstream::out);
	// use variant, visitor and duck typing.
	fout1 << visit([](auto &g) { return g.proto(9); }, generator);
	fout1.close();
	generator = ParallelProtoGenerator();
	ofstream fout2("parallel.proto", ofstream::out);
	fout2 << visit([](auto &g) { return g.proto(100); }, generator);
	fout2.close();
}
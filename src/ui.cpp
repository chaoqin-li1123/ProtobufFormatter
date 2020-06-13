#include <iostream>
#include "node.h"
#include "format.h"
#include "indent.h"
#include "factory.h"
#include "parser.h"
#include "build_tree.h"
using std::endl;
using std::cout;
using std::cin;
using std::make_unique;
using std::ofstream;
using namespace mpcs;

int main() {
	while (true) {
		config_init("config.txt");
		cout << "Name of input file, type \"exit\" to exit the program: ";
		string inputfile;
		cin >> inputfile;
		if (inputfile == "exit") break;
		cout << "Name of output file: ";	
		string outputfile;
		cin >> outputfile;
		ofstream fout;
		fout.open(outputfile);
		IndentStream ins(fout);
		NodeFormatter formatter(ins);
		// input and process
		vector<string> tokens = Tokenizer().tokenize(inputfile).get_tokens();
		vector<vector<string>> splitted = Splitter().split(tokens).get_splitted();
		for (auto& sub: splitted) {
			formatter.format(build_tree(sub));
		}
		// output
		fout.close();
		cout << "Finish." << endl;
	}
	return 0;
}
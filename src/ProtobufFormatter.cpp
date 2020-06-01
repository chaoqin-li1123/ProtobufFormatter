#include <iostream>
#include "node.h"
#include "format.h"
#include "indent.h"
#include "factory.h"
#include "parser.h"
#include "build_tree.h"
using std::cout;
using std::cin;
using std::make_unique;
using std::ofstream;
int main() {
	while (true) {
		cout << "Name of input file:";
		string inputfile;
		cin >> inputfile;
		cout << "Name of output file:";		
		string outputfile;
		cin >> outputfile;
		ofstream fout;
		fout.open(outputfile);
		IndentStream ins(fout);
		NodeFormatter formatter(ins);
		vector<string> tokens = Tokenizer().tokenize(inputfile).get_tokens();
		vector<string> striped = Striper().strip(tokens).get_striped();
		vector<vector<string>> splitted = Splitter().split(striped).get_splitted();
		unique_ptr<Node> root = build_tree(tokens);
		formatter.format(root);
		fout.close();
	}
	return 0;
}

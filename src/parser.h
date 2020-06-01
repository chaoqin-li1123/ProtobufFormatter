#ifndef PARSER_H
#define PARSER_H
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using std::fstream;
using std::string;
using std::cout;
using std::string_view;
using std::vector;
using std::ifstream;
using std::back_inserter;
struct Tokenizer {
	void open_file(string const& filename) {
		filein.open(filename);
	}
	bool is_delimeter(char c) {
		for (char recorded: recorded_delimeter) {
			if (c == recorded) return true;
		}
		for (char escaped: escaped_delimeter) {
			if (c == escaped) return true;
		}
		return false;
	}
	bool is_recorded(char c) {
		for (char recorded: recorded_delimeter) {
			if (c == recorded) return true; 
		}
		return false;
	}
	void tokenize_line(string const& line) {
		int s = -1;
		int t = -1;
		for (int i = 0; i < line.size(); i++) {
			if (is_delimeter(line[i])) {
				if (t != -1) {
					tokens.push_back(line.substr(s, t - s + 1));
					t = -1;
				}
				s = -1;
				if (is_recorded(line[i])) tokens.push_back(line.substr(i, 1));
			}
			else {
				if (s == -1) s = i;
				t = i;
			}
		}
	}
	Tokenizer& tokenize(string filename) {
		open_file(filename);
	    for (string line; getline(filein, line); )
		{
			tokenize_line(line);
		}	
		filein.close();
		return *this;
	}
	vector<string> get_tokens() {
		return move(tokens);
	}
private:
	ifstream filein;
	vector<char> escaped_delimeter{' ', '\n', '=', '\t'};
	vector<char> recorded_delimeter{';', '{', '}'};
	vector<string> tokens;
};
struct Striper {
	Striper& strip(vector<string> tokens) {
		move(tokens.begin() + 3, tokens.end() - 1, back_inserter(striped));
		return *this;
	}
	vector<string> get_striped() {
		return move(striped);
	}
private:
	vector<string> striped;
};
struct Splitter {
	Splitter& split(vector<string> tokens) {
		vector<string> temp;
		int l_curly = 0;
		for (string token: tokens) {
			temp.push_back(token);
			if (token[0] == '{') l_curly++;
			if (token[0] == '}') l_curly--;
			if (l_curly == 0 && [&token](){return token[0] == '}' || token[0] == ';';}()) splitted.push_back(move(temp));
		}
		return *this;
	}
	vector<vector<string>> get_splitted() {
		return move(splitted);
	}
private:
	vector<vector<string>> splitted;
};
#endif



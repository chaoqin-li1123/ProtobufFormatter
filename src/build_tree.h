#ifndef BUILD_TREE_H
#define BUILD_TREE_H

#include "parser.h"
#include "factory.h"
using std::stoi;
unique_ptr<NodeFactory>& get_node_factory() {
	static unique_ptr<NodeFactory> factory;
	return factory;
}

inline bool is_enum_or_message(string const& s) {
	return s == "enum" || s == "message";
}
unique_ptr<Node> build_tree(vector<string> tokens) {
	string type = move(tokens[0]);
	string name = move(tokens[1]);
	unique_ptr<Node> ret; 
	if (type == "message") {
		ret = get_node_factory()->create<MessageNode>(move(type), move(name));
		vector<string> striped = Striper().strip(tokens).get_striped();
		vector<vector<string>> splitted = Splitter().split(striped).get_splitted();
		auto comparator = [] (vector<string>& ss1, vector<string>& ss2) {
			if (is_enum_or_message(ss1[0]) && !is_enum_or_message(ss2[0])) return false;
			if (!is_enum_or_message(ss1[0]) && is_enum_or_message(ss2[0])) return true;
			return ss1[0] < ss2[0];
		};
		sort(splitted.begin(), splitted.end(), comparator);
		for (vector<string> sub: splitted) ret->add_children(build_tree(sub));
	}
	else if (type == "enum") {
		vector<string> striped = Striper().strip(tokens).get_striped();
		vector<vector<string>> splitted = Splitter().split(striped).get_splitted();
		vector<pair<string, int>> enum_list;
		for (auto&& ele: splitted) {
			enum_list.emplace_back(ele[0], stoi(ele[1]));
		}
		ret = get_node_factory()->create<EnumNode>(move(type), move(name), move(enum_list));
	}
	else {
		string field = tokens[2];
		ret = get_node_factory()->create<BasicNode>(move(type), move(name), stoi(field));
	}
	return ret;
}

#endif
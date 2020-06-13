#ifndef BUILD_TREE_H
#define BUILD_TREE_H

#include <exception>
#include <future>
#include "parser.h"
#include "factory.h"
#include <atomic>
#include "config.h"
#include "indent.h"
using std::stoi;
using std::string_view;
using std::exception;
using std::vector;
using std::future;
using std::async;
using std::atomic;
namespace mpcs {
namespace v1 {

static atomic<int> threads = 0; // keep track of the total number of async threads, accessed by multi-threads

namespace {
int max_threads{3};
int size_to_async{20};
bool enable_multi_thread{true};
ConfigVariables config_variables = {
	{ "max_threads", &max_threads },
    { "size_to_async", &size_to_async },
    { "indent_spaces", &indent_spaces }, 
    { "enable_multi_thread", &enable_multi_thread } 
};
}


void config_init(string config_file) {
	string line;
	ifstream configs(config_file);
	while (getline(configs, line)) {
		Setting::factory(line)->process(config_variables);
	}
}

unique_ptr<NodeFactory>& get_node_factory() {
	static unique_ptr<NodeFactory> factory = make_unique<NodeFactory>();
	return factory;
}

// string_view is literal, can be used in a constexpr function
constexpr bool is_enum_or_message(string_view s) {
	return s == "enum" || s == "message";
}

unique_ptr<Node> build_tree(vector<string> tokens) {
	string type = move(tokens[0]);
	string name = move(tokens[1]);
	unique_ptr<Node> ret; 
	if (type == "message") {
		ret = get_node_factory()->create<MessageNode>(type, name);
		vector<string> striped = Striper().strip(tokens).get_striped();
		vector<vector<string>> splitted = Splitter().split(striped).get_splitted();
		// lambda function as comparator for callback
		auto comparator = [] (vector<string>& ss1, vector<string>& ss2) {
			if (is_enum_or_message(ss1[0]) && !is_enum_or_message(ss2[0])) return false;
			if (!is_enum_or_message(ss1[0]) && is_enum_or_message(ss2[0])) return true;
			return ss1[0].size() < ss2[0].size();
		};
		sort(splitted.begin(), splitted.end(), comparator);
		//for (vector<string>& sub: splitted) ret->add_children(build_tree(sub));
		// use future and async
		std::vector<std::future<unique_ptr<Node>>> future_children;
		if (enable_multi_thread) {
		    for (decltype(future_children)::size_type i = 0; i < splitted.size(); i++) {
		    	if (splitted[i].size() > size_to_async && threads <= max_threads) {
	            	future_children.push_back(std::async(build_tree, splitted[i]));
	            	threads++;
		    	}
	        }
		}
	    for (decltype(future_children)::size_type i = 0; i < splitted.size(); i++) {
	    	if (splitted[i].size() <= size_to_async)
            	ret->add_children(build_tree(splitted[i]));
        }
        for (auto& future_child: future_children) {
            ret->add_children(move(future_child.get()));
            threads--;
        }
	}
	else if (type == "enum") {
		vector<string> striped = Striper().strip(tokens).get_striped();
		vector<vector<string>> splitted = Splitter().split(striped).get_splitted();
		vector<pair<string, int>> enum_list;
		try {
			for (auto& ele: splitted) {
				enum_list.emplace_back(ele[0], stoi(ele[1]));
			}
		}
		catch (exception& e) {
			cout << e.what() << '\n';
		}
		ret = get_node_factory()->create<EnumNode>(move(type), move(name), move(enum_list));
	}
	else {
		string field = tokens[2];
		try {
			ret = get_node_factory()->create<BasicNode>(move(type), move(name), stoi(field));			
		}
		catch (exception& e) {
			cout << e.what() << '\n';
		}
	}
	return ret;
}
} // v1

using namespace v1;
}


#endif
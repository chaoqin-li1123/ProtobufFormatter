#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <variant>
using std::string;
using std::ofstream;
using std::vector;
using std::variant;
using std::visit;
using std::string_view;
namespace mpcs {
namespace v1 {


struct NestedProtoGenerator {
NestedProtoGenerator(int max_depth) {
	headers = vector<string>(max_depth + 1);
	footers = vector<string>(max_depth + 1);
	protos = vector<string>(max_depth + 1);
}
string proto(int i) {
	assert(i < headers.size());
	if (protos[i].size()) return protos[i];
	protos[i] = header(i) + footer(i);
	return protos[i];
}
private:
string header(int i) {
	if (headers[i].size()) return headers[i];
	string head;
	if (i == 0)
		headers[i] = R"(message Person {
char c = 4;
       int32 id = 2;string name = 1;
    string email = 3;
    enum Mail {
EDU = 1;
        GMAIL = 2;
    }
        message name {
    )";
    else 
    	headers[i] = header(i - 1) + proto(0);
    return headers[i];
}

string footer(int i) {
	if (footers[i].size()) return footers[i];
	if (i == 0)
		footers[i] = R"(
    string first = 1;
  string second = 2;
                            }
})";
	else 
		footers[i] = proto(i - 1) + footer(i - 1);
	return footers[i];
}
vector<string> headers;
vector<string> footers;
vector<string> protos;
};
// define function in anonymous namespace
namespace {
string get_base() {
    // raw string literal as constexpr
constexpr static auto base = R"(        message Person {
            char c = 4;
            int32 id = 2;
            string name = 1;
            string email = 3;
            enum Mail {
                EDU = 1;
                GMAIL = 2;
            }
            message name {
                string first = 1;
                string second = 2;
            }
        })";
        return base;
}
}
struct ParallelProtoGenerator {
string proto(int repeat) {
	string base = get_base();
	string proto_text("message parallel{");
    // use reserve to avoid re-allocattion of memory.
    proto_text.reserve(repeat * base.size() + 30);
	for (int i = 0; i < repeat; i++) proto_text += base;
	return proto_text + "}";
}
};

using ProtoGeneratorVar = variant<NestedProtoGenerator, ParallelProtoGenerator>;
} // v1
using namespace v1;
} // mpcs

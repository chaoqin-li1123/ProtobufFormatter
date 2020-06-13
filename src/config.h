#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <map>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <variant>

using std::string;
using std::map;
using std::runtime_error;
using std::unique_ptr;
using std::make_unique;
using std::stoi;
using std::variant;
using std::get;

namespace mpcs {
namespace v1 {
using ConfigVariables =
		map < string, variant<string *, bool *, int *>>;

class Setting {
	struct Visitor {
		Visitor(ConfigVariables::mapped_type const &var) : var{var} {}
		void operator()(string const &str) {
			*get<string *>(var) = str;
		}
		void operator()(bool b) {
			*get<bool *>(var) = b;
		}
		void operator()(int i) {
			*get<int *>(var) = i;
		}
		ConfigVariables::mapped_type const &var;
	};

public:
	Setting(string name, variant<string, bool, int> v) 
		: name{ name }, value{v} {}
	void process(ConfigVariables const &cv) {
		auto v = cv.find(name);
		if (v != cv.end())
			visit(Visitor{v->second}, value);
		else
			throw runtime_error("Invalid String Setting: " + name);

	}
	inline static unique_ptr<Setting> factory(string configLine);
protected:
	string name;
	variant<string, bool, int> value;
};


inline unique_ptr<Setting>
Setting::factory(string configLine)
{
	auto firstSpace = configLine.find_first_of(' ');
	string name{ configLine.substr(0, firstSpace) };
	string rawValue{ configLine.substr(firstSpace + 1) };
	if (rawValue[0] == '"')
		return make_unique<Setting>(name, rawValue.substr(1, rawValue.size() - 3));
	else if (rawValue == "true")
		return make_unique<Setting>(name, true);
	else if (rawValue == "false")
		return make_unique<Setting>(name, false);
	else
		return make_unique<Setting>(name, stoi(rawValue));
}
} // v1
using namespace v1;
} // mpcs
#endif
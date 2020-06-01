#ifndef NODE_H
#define NODE_H
#include <vector>
#include <memory>
#include <utility>
#include "indent.h"
using std::vector;
using std::unique_ptr;
using std::vector;
using std::string;
using std::pair;
using std::forward;
enum class NodeType {
	Message, Enum, Basic
};

struct Node {
	Node(string type_, string name_) : type(move(type_)), name(move(name_)) {}
	void set_name(string const& name_) {
		name = name_;
	}
	void set_name(string&& name_) {
		name = move(name_);
	}
	const string& get_name() const& {
		return name;
	}
	string get_name() && {
		return move(name);
	}
	void set_type(string const& type_) {
		type = type;
	}
	void set_type(string && type_) {
		type = move(type_);
	}
	const string& get_type() const& {
		return type;
	}
	string get_type() && {
		return move(type);
	}
	const vector<std::unique_ptr<Node>>& get_children() const {
		return children;
	}
	void add_children(unique_ptr<Node> node_ptr) {
		children.push_back(move(node_ptr));
	}
	virtual NodeType get_node_type() const = 0;
	virtual int set_field(int field_) {}
protected:
	vector<std::unique_ptr<Node>> children; 
	string type;
	string name;
};

struct MessageNode : public Node {
	MessageNode(string type_, string name_) : Node(type_, name_) {}
	virtual NodeType get_node_type() const override {
		return NodeType::Message;
	}
};

struct BasicNode : public Node {
	BasicNode(string type_, string name_) : Node(type_, name_) {}
	bool set_option(bool required_) {
		required = required_;
	}
	virtual int set_field(int field_) override {
		field = field_;
	}
	int get_field() const {
		return field;
	}
	virtual NodeType get_node_type() const override {
		return NodeType::Basic;
	}
private:
	bool required;
	int field;
};

struct EnumNode : public Node {
	EnumNode(string type_, string name_) : Node(type_, name_) {}
	void set_enum_list(vector<pair<string, int>> const& elements_) {
		elements = elements_;
	}
	void set_enum_list(vector<pair<string, int>> && elements_) {
		elements = move(elements_);
	}	
	const vector<pair<string, int>>& get_elements() const {
		return elements;
	}	
	virtual NodeType get_node_type() const override {
		return NodeType::Enum;
	}
private: 
	vector<pair<string, int>> elements;
};


void init(unique_ptr<MessageNode> & message_ptr) {}
void init(unique_ptr<EnumNode> & enum_ptr, vector<pair<string, int>>&& enum_list) {
	enum_ptr->set_enum_list(forward<vector<pair<string, int>>>(enum_list));
}
void init(unique_ptr<BasicNode> & basic_ptr, int field) {
	basic_ptr->set_field(field);
}

#endif


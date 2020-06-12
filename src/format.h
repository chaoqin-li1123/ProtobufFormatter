#ifndef FORMAT_H
#define FORMAT_H
#include "indent.h"
#include "node.h"
using mpcs::Node;
using mpcs::EnumNode;
using mpcs::BasicNode;
using mpcs::MessageNode;
using mpcs::indent;
using mpcs::unindent;
using mpcs::NodeType;
using std::cerr;
namespace mpcs {
namespace v1 {
struct NodeFormatter{
	NodeFormatter(ostream& os_) : os(os_) {}
	void format(unique_ptr<Node> const& node) {
		format_head(node);
		// use static function and reinterpret_cast for dynamic dispatch.
		switch (node->get_node_type()) {
			case NodeType::Enum: 
				format_tail(reinterpret_cast<unique_ptr<EnumNode> const&>(node));
				break;
			case NodeType::Basic:
				format_tail(reinterpret_cast<unique_ptr<BasicNode> const&>(node));
				break;
			case NodeType::Message:
				format_tail(reinterpret_cast<unique_ptr<MessageNode> const&>(node));
				break;
			default:
				cerr <<"Type mismatch.";
		}
		os << "\n";
	}
	void format_head(unique_ptr<Node> const& node) {
		os << node->get_type() << " " << node->get_name();
	}
	// function overload for specific type.
	void format_tail(unique_ptr<EnumNode> const& enum_node) {
		os << " {\n";
		os << indent;
		for (auto& element: enum_node->get_elements()) {
			os << element.first << " = " << element.second << ";\n";
		}
		os << unindent;
		os << "}";
	}
	void format_tail(unique_ptr<BasicNode> const& basic_node) {
		os << " = " << basic_node->get_field() << ";";
	}
	void format_tail(unique_ptr<MessageNode> const& message_node) {
		os << " {\n";
		os << indent;
		for (auto& child: message_node->get_children()) format(child);
		os << unindent;
		os << "}";   
	}
	ostream& os;
};
} // v1	
using namespace v1;
} // mpcs



#endif

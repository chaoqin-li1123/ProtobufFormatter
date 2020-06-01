#ifndef FACTORY_H
#define FACTORY_H
#include <tuple>
#include <memory>
#include "node.h"
#include <utility>
using std::tuple;
using std::unique_ptr;
using std::make_unique;
using std::forward;
template<typename T, typename TypeList>
struct contain;
template<typename T, typename... Types>
struct contain<T, tuple<T, Types...>>  {
	static bool constexpr v = true;
};
template<typename T>
struct contain<T, tuple<>>  {
	static bool constexpr v = false;
};
template<typename T1, typename T2, typename... Types>
struct contain<T1, tuple<T2, Types...>> : contain<T1, tuple<Types...>> {};
template<typename AbstractType, typename ArgsTypesList, typename... ConcreteTypes>
struct Factory;
template<typename AbstractType, typename... ConcreteTypes, typename... Args>
struct Factory<AbstractType, tuple<Args...>, ConcreteTypes...> 
{

	template<typename ConcreteType, typename... Other> 
	unique_ptr<AbstractType> create(Args... args, Other&&... init_arg) {
		static_assert(contain<ConcreteType, tuple<ConcreteTypes...>>::v, "Type is not found in the factory");
		unique_ptr<ConcreteType> concrete = make_unique<ConcreteType>(args...);
		init(concrete, forward<Other>(init_arg)...);
		return move(concrete);
	}	                        	
};

using NodeFactory = Factory<Node, tuple<string, string>, EnumNode, MessageNode, BasicNode>;

#endif
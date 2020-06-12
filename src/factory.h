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

namespace mpcs {
namespace v1 {
// declaration of the template, whether type T is in typelist.
template<typename T, typename TypeList>
struct contain;

// base case 1, terminate the recursion
template<typename T, typename... Types>
struct contain<T, tuple<T, Types...>>  {
	static bool constexpr value = true;
};

// base case 2, terminate the recursion
template<typename T>
struct contain<T, tuple<>>  {
	static bool constexpr value = false;
};

// recursion
template<typename T1, typename T2, typename... Types>
struct contain<T1, tuple<T2, Types...>> : contain<T1, tuple<Types...>> {};

// declaration of the template
template<typename AbstractType, typename ArgsTypesList, typename... ConcreteTypes>
struct Factory;

// specialization of the template
template<typename AbstractType, typename... ConcreteTypes, typename... Args>
struct Factory<AbstractType, tuple<Args...>, ConcreteTypes...> 
{
	template<typename ConcreteType, typename... Other> 
	unique_ptr<AbstractType> create(Args... args, Other&&... init_arg) {
		// The concrete type created must be in the concrete type list of the factory
		static_assert(contain<ConcreteType, tuple<ConcreteTypes...>>::value, "Type is not found in the factory");
		// use && and perfect forward.
		unique_ptr<ConcreteType> concrete = make_unique<ConcreteType>(forward<Args>(args)...);
		init(concrete, forward<Other>(init_arg)...);
		return concrete;
	}	                        	
};

using NodeFactory = Factory<Node, tuple<string, string>, EnumNode, MessageNode, BasicNode>;
} // v1
using namespace v1;
} // mpcs



#endif
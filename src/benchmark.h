#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <ctime> 
#include <iostream>
#include <type_traits>
#include <functional>
using std::is_invocable;
using std::enable_if_t;
using std::invoke;
namespace mpcs {
namespace v1 {

/* duration_count take a callable as a parameter 
and return the total running time in ms.
Use SFINAE for type checking. */
template <
		typename Callable,  
		typename = enable_if_t<is_invocable<Callable>::value>
		>
auto duration_count(Callable func) {
	auto start = std::chrono::system_clock::now();
	invoke(func); 
	//func();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
	return elapsed.count();
}


template <
		typename Callable,  
		typename... Args
		>
auto duration_count(Callable func, Args... args) {
	// use static_assert for type checking.
	static_assert(is_invocable<Callable, Args...>::value, "The first argument is not invokable.\n");
	auto start = std::chrono::system_clock::now();
	invoke(func, forward<Args>(args)...); // use invoke for callable.
	//func(forward<Args>(args)...);
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
	return elapsed.count();
}

 /* run func for repeat times,
 	print the total duration in ms. */
template <
		typename Callable,  
		typename... Args
		>
auto benchmark(int repeat, Callable func, Args... args) {
	decltype(duration_count(func, forward<Args>(args)...)) total_time = 0;
	for (int i = 0; i < repeat; i++) total_time += duration_count(func, forward<Args>(args)...);
	cout << "Run the function for " << repeat << " times, total duration is " << total_time  << " ms." << endl;
}


}
using namespace v1;
} // mpcs

#endif

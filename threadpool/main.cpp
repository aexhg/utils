#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

#include "threadpool.hpp"

int f(int x, int y){
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(3s);
	return x + y;
}
int main(){
	
	threadpool::ThreadPool tp(5);

	std::vector<std::future<int>> v;
	for(int i = 0; i < 10; ++i){
		v.emplace_back(tp.enqueue(f, i, 5));

	}


	for(auto& vv : v ){
		std::cout << vv.get() << std::endl;
	}

	return 0;
}

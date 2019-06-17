#ifndef __THREADPOOL
#define __THREADPOOL

#include <future>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <memory>

namespace threadpool {

class ThreadPool{
	public:
		ThreadPool(size_t);
		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
		~ThreadPool();

	private:
		std::vector<std::thread> workers;
		std::queue<std::function<void()>> tasks;

		std::mutex queue_mutex;
		std::condition_variable condition;
		bool stopped;
};


inline ThreadPool::ThreadPool(size_t t) : 
	stopped(false) {
		for(auto i = 0; i < t; ++i){
			workers.emplace_back([this]{
					for( ; ; ){
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->queue_mutex);
							this->condition.wait(lock, [this]{
									return this->stopped || !this->tasks.empty();
									});
							if(this->stopped && this->tasks.empty()){
								return;
							}
							task = std::move(tasks.front());;
							this->tasks.pop();
						}
						task();
					}
				});
			}
	}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) ->
	std::future<typename std::result_of<F(Args...)>::type> {
		using result_type = 
			typename std::result_of<F(Args...)>::type;
		auto task = std::make_shared<std::packaged_task<result_type()>>(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		auto future = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			if(stopped){
				throw std::runtime_error("Attempt to queue on stopped pool");
			}
			tasks.emplace([task]{(*task)();});
		}
		condition.notify_one();
		return future;
}

inline ThreadPool::~ThreadPool(){
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stopped = true;
	}
	condition.notify_all();
	for(auto& w : workers ) {
		w.join();
	}
}
}
#endif	

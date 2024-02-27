#pragma once
#include <condition_variable>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>


class ThreadPool
{
private:
	std::vector<std::thread> threads_;
	std::queue<std::function<void()>> tasks_;
	std::condition_variable cv_;
	std::mutex mutex_;
	bool stop_;
	int num_threads_;
public:
	ThreadPool() = default;
	ThreadPool(int num_threads);
	void setNumThreads(int num_threads);
	void enqueue(const std::function<void()>& task);
	~ThreadPool();
};


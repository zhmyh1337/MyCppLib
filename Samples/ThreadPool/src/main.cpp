#include <iostream>
#include <atomic>

#include "thread_pool.h"
#include "timer.h"

std::atomic<int> x = 0;

void Job()
{
	volatile double y = 0;
	for (size_t i = 0; i < 1000; i++)
	{
		y += 1337;
		y /= 2.5;
	}
	x.fetch_add(1);
}

int main()
{
	my::timer timer("main", true);
	constexpr size_t iterationsCount = 1'000'000;

#if 1
	my::thread_pool threadPool(std::thread::hardware_concurrency());
	std::cout << "hi" << std::endl;
	for (size_t i = 0; i < iterationsCount; i++)
		threadPool.add_job(Job);
	threadPool.terminate(true);
#else
	for (size_t i = 0; i < iterationsCount; i++)
	{
		Job();
	}
#endif

	std::cout << x << std::endl;
}

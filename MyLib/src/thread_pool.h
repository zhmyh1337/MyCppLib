#pragma once

#include <vector>
#include <thread>
#include <condition_variable>
#include <functional>
#include <queue>

namespace my
{
	class thread_pool
	{
	public:
		using Job = std::function<void()>;

	public:
		thread_pool(size_t threadsCount);
		~thread_pool();

		void add_job(Job job);
		bool any_pending_jobs();
		void terminate(bool waitForPendingJobs = false);

	private:
		void thread_loop();

	private:
		std::vector<std::thread> m_threads;
		std::condition_variable m_cv;
		std::mutex m_queueMutex;
		std::queue<Job> m_jobsQueue;
		bool m_terminated = false;
	};
}

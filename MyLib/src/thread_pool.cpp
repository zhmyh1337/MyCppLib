#include "thread_pool.h"

namespace my
{
	thread_pool::thread_pool(size_t threadsCount)
	{
		m_threads.reserve(threadsCount);
		for (size_t i = 0; i < threadsCount; i++)
		{
			m_threads.push_back(std::thread(&thread_pool::thread_loop, this));
		}
	}

	thread_pool::~thread_pool()
	{
		if (!m_terminated)
		{
			terminate();
		}
	}

	void thread_pool::add_job(Job job)
	{
		{
			std::scoped_lock<std::mutex> queueLock(m_queueMutex);
			m_jobsQueue.push(job);
		}
		m_cv.notify_one();
	}

	bool thread_pool::any_pending_jobs()
	{
		return !m_jobsQueue.empty();
	}

	void thread_pool::terminate(bool waitForPendingJobs)
	{
		if (waitForPendingJobs)
		{
			while (any_pending_jobs())
			{
				std::this_thread::yield();
			}
		}

		m_terminated = true;
		m_cv.notify_all();

		for (auto& thread : m_threads)
			thread.join();
	}

	void thread_pool::thread_loop()
	{
		while (true)
		{
			std::unique_lock queueLock(m_queueMutex);
			m_cv.wait(queueLock, [this]() { return !m_jobsQueue.empty() || m_terminated; });

			if (m_terminated)
				return;

			auto job = m_jobsQueue.front();
			m_jobsQueue.pop();
			queueLock.unlock();
			job();
		}
	}
}

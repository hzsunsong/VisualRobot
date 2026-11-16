/**
 * @file guard_thread.hpp
 * @brief Thread wrapper that joins on destruction.
 * @author songhuirong
 * @date 2023/02/14
 */

#ifndef GUARD_THREAD_H
#define GUARD_THREAD_H
#include <thread>

namespace cxx
{
	/**
	 * @brief Joins a joinable thread during destruction.
	 * @author songhuirong
	 * @date 2023/02/14
	 */
	class guard_thread : public std::thread
	{
		using std::thread::thread;
	public:
		/**
		 * @brief Waits for the thread to finish if it is joinable.
		 * @note Destruction blocks while join() waits for the thread; destruction on the joined thread itself is invalid.
		 */
		~guard_thread(){ if (joinable()) join(); }
	};
}

#endif // GUARD_THREAD_H

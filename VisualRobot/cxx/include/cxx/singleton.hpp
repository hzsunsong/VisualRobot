/**
 * @file singleton.hpp
 * @brief Shared instance storage with initialization guarded per argument type pack.
 * @author songhuirong
 * @date 2023/02/09
 */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP
#include "cxx_def.h"
#include <memory>

namespace cxx
{
	/**
	 * @brief Provides lazily initialized, shared access to one instance of T.
	 * @tparam T The instance type; its constructor must be accessible to singleton.
	 * @details Each instance<Args...> specialization has its own once_flag, while all
	 * specializations share m_inst. Use one consistent argument type pack to avoid
	 * replacing the shared instance; calls with different packs are not mutually synchronized.
	 * @author songhuirong
	 * @date 2023/02/09
	 *
	 * @code{.cpp}
	 * class grace_tool
	 * {
	 * 	friend class cxx::singleton<grace_tool>;
	 * 	public:
	 * 		void fn() {}
	 * 	private:
	 * 		DELETE_CLASS_DEFAULT_OPERATION(grace_tool);
	 * };
	 *
	 * void fn()
	 * {
	 * 	cxx::singleton<grace_tool>::instance()->fn();
	 * }
	 * @endcode
	 */
	template <typename T>
	class singleton
	{
	public:
		/**
		 * @brief Gets the shared pointer after initialization for this argument type pack.
		 * @tparam Args Constructor argument types.
		 * @param[in] args Arguments forwarded to T's constructor during initialization.
		 * @return The instance pointer, or nullptr if allocation fails.
		 * @note Later calls with the same Args... ignore their arguments, even if allocation
		 * returned nullptr. A different Args... specialization can replace the stored instance.
		 */
		template <typename... Args>
		static T *instance(Args&&... args)
		{
			static std::once_flag once_flag_t;

			std::call_once(once_flag_t, [&]
			{
				m_inst.reset(new(std::nothrow) T{ std::forward<Args>(args)... });
			});

			return m_inst.get();
		}
	private:
        /**
         * @brief Defaults construction and destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION(singleton);

		static std::unique_ptr<T> m_inst; ///< Shared owning storage used by every instance<Args...> specialization for this T.
	};

	template <typename T>
	std::unique_ptr<T> cxx::singleton<T>::m_inst;
}

#endif // SINGLETON_HPP
/**
 * @file object_p.h
 * @brief Polymorphic private storage and non-owning public back-reference.
 */

#ifndef OBJECT_P_H
#define OBJECT_P_H
#include "cxx/cxx_def.h"
#include "object.h"

namespace Qtx
{
	/**
	 * @brief Provides polymorphic private storage and a back-reference to the owning object.
	 */
    class ObjectPrivate
	{
		Q_DECLARE_PUBLIC(Object)
	public:
        DELETE_CLASS_DEFAULT_OPERATION_V(ObjectPrivate);

        /** @brief Non-owning back-reference assigned by the owning Object constructor. */
        Object *q_ptr{nullptr};
	};
}

#endif // OBJECT_P_H

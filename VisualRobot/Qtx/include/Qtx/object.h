/**
 * @file object.h
 * @brief Ownership of extensible private implementation storage.
 */

#ifndef OBJECT_H
#define OBJECT_H
#include <QScopedPointer>
#include "qtx_export.h"

namespace Qtx
{
	class ObjectPrivate;

	/**
	 * @brief Owns a private implementation that can be extended by derived classes.
	 */
    class QTX_EXPORT Object
	{
		Q_DECLARE_PRIVATE(Object)
        Q_DISABLE_COPY_MOVE(Object)
	public:
        /**
         * @brief Creates the object and attempts to allocate its private implementation.
         * @note If allocation fails, the object retains a null private implementation.
         */
        Object();
        /** @brief Destroys the owned private implementation through its virtual destructor. */
        virtual ~Object();
    protected:
        /**
         * @brief Takes ownership of the private implementation and binds it to this object.
         * @param[in] dd Private implementation to take exclusive ownership of, or nullptr
         *               if allocation failed.
         * @pre A non-null dd must point to a dynamically allocated object that is not
         *      already owned by another object.
         * @note The caller must not delete dd after ownership is transferred.
         *       A null dd leaves this object without a private implementation.
         */
        explicit Object(ObjectPrivate *dd);

        /** @brief Exclusively owns application-specific private storage, independently of QObject. */
        QScopedPointer<ObjectPrivate> d_ptr;
	};
}

#endif // OBJECT_H

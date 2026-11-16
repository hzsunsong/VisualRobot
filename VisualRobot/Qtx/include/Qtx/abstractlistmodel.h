/**
 * @file abstractlistmodel.h
 * @brief Flat Qt list model with owned entries and guarded notifications.
 */

#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H
#include <cstdint>
#include <memory>
#include <QAbstractListModel>
#include "object.h"
#include "qtx_export.h"

namespace cxx
{
    struct item;
    struct list_item;
}

namespace Qtx
{
	class AbstractListModelPrivate;

    /**
     * @brief Provides an abstract list model base with extensible private storage.
     * @details Object owns the application-specific private implementation independently
     *          of the private storage maintained by the Qt model base.
     * @note The QObject-derived base is kept first for Qt meta-object generation.
     * @pre Construct and access the model on the UI thread. Direct entry changes require
     *      the corresponding model notifications to refresh attached views.
     */
    class QTX_EXPORT AbstractListModel : public QAbstractListModel, public Object
	{
		Q_OBJECT
		/** @brief Selects Object private storage to avoid ambiguity with QObject::d_ptr. */
		Q_DECLARE_PRIVATE_D(Object::d_ptr, AbstractListModel)
        Q_DISABLE_COPY_MOVE(AbstractListModel)
	public:
        /**
         * @brief Constructs the model and attempts to allocate its private implementation.
         * @param[in] parent Parent passed to QAbstractListModel, or nullptr for no parent.
         * @note If private implementation allocation fails, queries return no rows or data.
         */
        explicit AbstractListModel(QObject *parent = nullptr);
        /**
         * @brief Destroys owned entries and private storage through the Object base.
         */
        virtual ~AbstractListModel() override;

        /**
         * @brief Reports the number of entries at the root of this flat model.
         * @param[in] parent Parent index; a valid parent has no child rows.
         * @return Root entry count, or zero for a valid parent or unavailable storage.
         */
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        /**
         * @brief Reads an entry's ID or name for the requested custom role.
         * @param[in] index Index belonging to this model, in column zero.
         * @param[in] role Custom role declared by Qtx::ModelRole.
         * @return Role data, or an invalid QVariant for unsupported roles or invalid indexes.
         * @note Names are decoded using the local 8-bit encoding; DisplayRole is not handled.
         */
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        /** @brief Maps the custom ID and name roles to names used by Qt Quick delegates. */
        QHash<int, QByteArray> roleNames() const override;
        /**
         * @brief Appends an entry with a unique ID and takes exclusive ownership.
         * @param[in,out] item Entry to append; successful insertion leaves this pointer empty.
         * @return True on insertion; false for unavailable storage, reentrancy, null input,
         *         a duplicate ID, the row limit, or a failed map insertion.
         * @note Validation rejection before row notifications preserves caller ownership.
         *       Container allocation exceptions are not translated into a false return value;
         *       this function does not provide rollback if a container operation fails.
         * @pre Call on the UI thread. Notification callbacks must not throw.
         */
        bool insert(std::unique_ptr<cxx::list_item> &&item);
        /**
         * @brief Removes and destroys the entry matching an immutable ID.
         * @param[in] id ID of the entry to remove.
         * @return True if removed; false for unavailable storage, reentrancy,
         *         or an ID not present in both storage containers.
         * @pre Call on the UI thread. Notification callbacks and entry destructors must not throw.
         */
        bool remove(uint64_t id);

        /**
         * @brief Replaces the stored entry matching an immutable ID with a clone of the source.
         * @param[in] item Source entry whose ID selects the target; must dynamically be
         *            list_item or a derived type for the clone to succeed.
         * @return True when the ID matches and cloning succeeds; false for unavailable
         *         storage, reentrancy, null input, an ID not present in both storage
         *         containers, a mismatched source type, or allocation failure.
         * @note The clone adopts the stored entry's dynamic type and keeps the source ID,
         *       so the ID index stays consistent; the replaced entry is destroyed and the
         *       matching row is announced through dataChanged so attached views refresh.
         * @pre Call on the UI thread. Notification callbacks and entry destructors must not throw.
         */
        bool update(const cxx::item *item);
        /**
         * @brief Destroys all entries and resets model indexes through reset notifications.
         * @details Does nothing when both containers are empty, storage is unavailable,
         *          or a modification is already in progress.
         * @pre Call on the UI thread. Notification callbacks and entry destructors must not throw.
         */
        void clear();
    protected:
        /**
         * @brief Constructs the Qt model base and transfers private implementation ownership to Object.
         * @param[in] dd Private implementation to take exclusive ownership of, or nullptr.
         * @param[in] parent Parent passed to QAbstractListModel, or nullptr for no parent.
         * @pre A non-null dd must point to a dynamically allocated object that is not
         *      already owned by another object.
         * @note The caller must not delete dd after ownership is transferred.
         *       A null dd leaves the model without application-specific private storage.
         */
        AbstractListModel(AbstractListModelPrivate *dd, QObject *parent = nullptr);
	};
}

#endif // ABSTRACTLISTMODEL_H

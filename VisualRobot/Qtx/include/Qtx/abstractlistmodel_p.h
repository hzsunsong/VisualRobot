/**
 * @file abstractlistmodel_p.h
 * @brief Row storage, ID index, and modification state for the list model.
 */

#ifndef ABSTRACTLISTMODEL_P_H
#define ABSTRACTLISTMODEL_P_H
#include <map>
#include <vector>
#include "cxx/list_item.h"
#include "abstractlistmodel.h"
#include "object_p.h"

namespace Qtx
{
    /**
     * @brief Provides extensible private storage for AbstractListModel.
     * @details Reuses the non-owning back-reference inherited from ObjectPrivate;
     *          ownership is managed by the model through Object.
     */
    class AbstractListModelPrivate : public ObjectPrivate
	{
		Q_DECLARE_PUBLIC(AbstractListModel)
	public:
        DELETE_CLASS_DEFAULT_OPERATION_V(AbstractListModelPrivate);

        /**
         * @brief Validates and appends an entry between matching insertion notifications.
         * @pre Called through the owner on the UI thread with its modification guard active.
         * @note Container allocation failures are not caught or rolled back.
         */
        bool insert(std::unique_ptr<cxx::list_item> &&item);
        /**
         * @brief Locates an entry once and removes it from both containers with row notifications.
         * @pre Called through the owner on the UI thread with its modification guard active.
         */
        bool remove(uint64_t id);

        /**
         * @brief Swaps the matching entry for a clone of the source and reports the row via dataChanged.
         * @pre Called through the owner on the UI thread with its modification guard active.
         * @note The clone keeps the source ID, so the ID index only needs re-pointing.
         */
        bool update(const cxx::item *item);

        /**
         * @brief Releases entries and lookup storage between matching reset notifications.
         * @pre Called through the owner on the UI thread with its modification guard active.
         */
        void clear();

        /** @brief Blocks synchronous reentrant changes throughout modification notifications. */
        bool modifying{false};
        /** @brief Owns entries in row order; all growth must respect the Qt int row limit. */
        std::vector<std::unique_ptr<cxx::list_item>> items;
        /** @brief Non-owning ID lookup kept consistent with entries owned by items. */
        std::map<uint64_t, cxx::list_item *> itemsT;
	};
}

#endif // ABSTRACTLISTMODEL_P_H

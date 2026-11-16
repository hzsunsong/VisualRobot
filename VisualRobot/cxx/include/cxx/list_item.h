/**
 * @file list_item.h
 * @brief Item specialization for list entries.
 * @date 2026/11/10
 */

#ifndef LIST_ITEM_H
#define LIST_ITEM_H
#include "item.h"

namespace cxx
{
    /**
     * @brief Represents a list item using the identifier and name inherited from @ref item.
     */
    struct CXX_EXPORT list_item : public item
    {
        list_item() = default;
        list_item(const list_item &rhs) = default;
        list_item &operator=(const list_item &rhs) = default;
        list_item(list_item &&) = delete;
        list_item &operator=(list_item &&) = delete;
        virtual ~list_item() = default;

        /**
         * @brief Creates an owning list_item copy of the given source entry.
         * @param[in] item_t Source entry; must dynamically be a list_item.
         * @return Owning copy, or empty for null input, a non-list_item source,
         *         or allocation failure.
         * @note The copy keeps the source ID so the clone can replace the original entry.
         */
        std::unique_ptr<item> clone(const item *item_t) const override;
    };
}

#endif // LIST_ITEM_H

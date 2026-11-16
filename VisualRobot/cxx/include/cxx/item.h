/**
 * @file item.h
 * @brief Base item declaration.
 * @date 2026/11/10
 */

#ifndef ITEM_H
#define ITEM_H
#include <memory>
#include <string>
#include "cxx_export.h"

namespace cxx
{
    /**
     * @brief Provides a name and an immutable identifier for derived item types.
     */
    struct CXX_EXPORT item
    {
        item() = default;
        /**
         * @brief Copies the source including its ID, so a clone can replace the original entry.
         * @note Derived copy constructors propagate the ID the same way through this base.
         */
        item(const item &rhs);
        item &operator=(const item &rhs);
        item(item &&) = delete;
        item &operator=(item &&) = delete;
        virtual ~item() = default;

        virtual std::unique_ptr<item> clone(const item *item_t) const;

        const uint64_t id{makeId()};
        std::string name;
    private:
        /**
         * @brief Increments the sequence shared by all item instances.
         * @return Next unsigned identifier; wraparound is not checked.
         * @note Incrementing the shared counter is not synchronized across threads.
         */
        uint64_t makeId();
    };
}

#endif // ITEM_H

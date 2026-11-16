/**
 * @file list_item.cpp
 * @brief Polymorphic copy of list entries through the list_item copy constructor.
 */
#include "cxx/cxx_def.h"
#include "list_item.h"

std::unique_ptr<cxx::item> cxx::list_item::clone(const item *item_t) const
{
    CHECK_PTR_RETURN_VALUE(item_t, {})

    /** @brief Reject sources whose dynamic type cannot supply list entry state. */
    const auto itemT{dynamic_cast<const list_item *>(item_t)};

    CHECK_PTR_RETURN_VALUE(itemT, {})

    return std::unique_ptr<item>{new(std::nothrow) list_item{*itemT}};
}

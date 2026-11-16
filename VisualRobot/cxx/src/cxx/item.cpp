/**
 * @file item.cpp
 * @brief Shared monotonically incremented item identifier sequence.
 */
#include "cxx/cxx_def.h"
#include "item.h"

cxx::item::item(const item &rhs)
    :id{rhs.id},
    name{rhs.name}
{

}

cxx::item &cxx::item::operator=(const item &rhs)
{
    if (&rhs == this) return *this;

    name = rhs.name;

    return *this;
}

std::unique_ptr<cxx::item> cxx::item::clone(const item *item_t) const
{
    CHECK_PTR_RETURN_VALUE(item_t, {})

    /** @brief Mirror the derived overrides; at the base the cast always succeeds. */
    const auto itemT{dynamic_cast<const item *>(item_t)};

    CHECK_PTR_RETURN_VALUE(itemT, {})

    return std::unique_ptr<item>{new(std::nothrow) item{*itemT}};
}

uint64_t cxx::item::makeId()
{
    static uint64_t id{0};

    return ++id;
}
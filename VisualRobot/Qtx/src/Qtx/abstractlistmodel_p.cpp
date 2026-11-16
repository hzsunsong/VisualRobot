/**
 * @file abstractlistmodel_p.cpp
 * @brief Entry storage updates enclosed in Qt model notifications.
 */

#include <algorithm>
#include <iterator>
#include <limits>
#include <utility>
#include "cxx/cxx_def.h"
#include "cxx/cxx_kit.h"
#include "abstractlistmodel_p.h"

bool Qtx::AbstractListModelPrivate::insert(std::unique_ptr<cxx::list_item> &&item)
{
    Q_Q(AbstractListModel);

    CHECK_PTR_RETURN_FALSE(q)

    CHECK_PTR_RETURN_FALSE(item)

    const auto id{item->id};
    const auto itor{itemsT.find(id)};

    if (itemsT.cend() != itor) return false;

    /** @brief Keep both row indexes and the resulting row count representable as int. */
    constexpr auto maxRows{std::numeric_limits<int>::max()};

    if (items.size() >= static_cast<std::size_t>(maxRows)) return false;

    const auto row{static_cast<int>(items.size())};
    q->beginInsertRows({}, row, row);
    const auto itemT{item.get()};
    items.emplace_back(std::move(item));
    const auto [itor3, inserted]{itemsT.try_emplace(id, itemT)};
    q->endInsertRows();

    if (!inserted) return false;

    return true;
}

bool Qtx::AbstractListModelPrivate::remove(uint64_t id)
{
    Q_Q(AbstractListModel);

    CHECK_PTR_RETURN_FALSE(q)

    const auto itor{std::find_if(items.cbegin(), items.cend(), [&](const auto &item_t){
        CHECK_PTR_RETURN_FALSE(item_t)

        const auto id2{item_t->id};

        return id2 == id;
    })};

    if (items.cend() == itor) return false;

    const auto itor2{itemsT.find(id)};

    if (itemsT.cend() == itor2) return false;

    /** @brief The insertion limit guarantees a safe conversion for existing rows. */
    const auto row{static_cast<int>(std::distance(items.cbegin(), itor))};
    q->beginRemoveRows({}, row, row);
    items.erase(itor);
    itemsT.erase(itor2);
    q->endRemoveRows();

    return true;
}

bool Qtx::AbstractListModelPrivate::update(const cxx::item *item)
{
    Q_Q(AbstractListModel);

    CHECK_PTR_RETURN_FALSE(q)

    CHECK_PTR_RETURN_FALSE(item)

    const auto id{item->id};
    const auto itor{std::find_if(items.begin(), items.end(), [&](const auto &item_t){
        CHECK_PTR_RETURN_FALSE(item_t)

        const auto id2{item_t->id};

        return id2 == id;
    })};

    if (items.end() == itor) return false;

    const auto itor2{itemsT.find(id)};

    if (itemsT.cend() == itor2) return false;

    /** @brief Clone with the stored entry's dynamic type; the copy keeps the source ID. */
    auto itemT{(*itor)->clone(item)};

    CHECK_PTR_RETURN_FALSE(itemT)

    /** @brief Stored entries are list_item or derived, so a successful clone is one too. */
    const auto itemT2{dynamic_cast<cxx::list_item *>(itemT.get())};

    CHECK_PTR_RETURN_FALSE(itemT2)

    /** @brief Re-point the ID index before the owning pointer destroys the replaced entry. */
    itemsT[id] = itemT2;
    (void)itemT.release();
    *itor = std::unique_ptr<cxx::list_item>{itemT2};

    /** @brief The insertion limit guarantees a safe conversion for existing rows. */
    const auto row{static_cast<int>(std::distance(items.begin(), itor))};
    const auto idx{q->index(row)};
    emit q->dataChanged(idx, idx);

    return true;
}

void Qtx::AbstractListModelPrivate::clear()
{
    Q_Q(AbstractListModel);

    CHECK_PTR_RETURN(q)

    if (items.empty() && itemsT.empty()) return;

    q->beginResetModel();
    cxx::free(items);
    cxx::free(itemsT);
    q->endResetModel();
}

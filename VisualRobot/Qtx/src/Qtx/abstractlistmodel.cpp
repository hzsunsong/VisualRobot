/**
 * @file abstractlistmodel.cpp
 * @brief List role queries and protection against reentrant modifications.
 */

#include <QScopedValueRollback>
#include "abstractlistmodel.h"
#include "abstractlistmodel_p.h"
#include "qtx_def.h"

Qtx::AbstractListModel::AbstractListModel(QObject *parent)
:AbstractListModel{new(std::nothrow) AbstractListModelPrivate, parent}
{
}

Qtx::AbstractListModel::~AbstractListModel() = default;

int Qtx::AbstractListModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const AbstractListModel);

    CHECK_PTR_RETURN_VALUE(d, 0)

    return parent.isValid() ? 0 : static_cast<int>(d->items.size());
}

QVariant Qtx::AbstractListModel::data(const QModelIndex &index, int role) const
{
    Q_D(const AbstractListModel);

    CHECK_PTR_RETURN_VALUE(d, {})

    if (!index.isValid() || index.model() != this
        || 0 != index.column() || d->items.empty()) return {};

    const auto row{index.row()};

    if (!(row >= 0 && row < d->items.size())) return {};

    const auto item{d->items[row].get()};

    if (!item) return {};

    if (static_cast<int>(ModelRole::Id) == role)
    {
        return item->id;
    }
    else if (static_cast<int>(ModelRole::Name) == role)
    {
        return QString::fromLocal8Bit(item->name.data());
    }

    return {};
}

QHash<int, QByteArray> Qtx::AbstractListModel::roleNames() const
{
    return {{static_cast<int>(ModelRole::Id), ID_ROLE_NAME},
            {static_cast<int>(ModelRole::Name), NAME_ROLE_NAME}};
}

bool Qtx::AbstractListModel::insert(std::unique_ptr<cxx::list_item> &&item)
{
    Q_D(AbstractListModel);

    CHECK_PTR_RETURN_FALSE(d)

    /** @brief Reject synchronous reentrant changes until the active notification sequence ends. */
    if (d->modifying) return false;

    QScopedValueRollback<bool> guard{d->modifying, true};

    return d->insert(std::move(item));
}

bool Qtx::AbstractListModel::remove(uint64_t id)
{
    Q_D(AbstractListModel);

    CHECK_PTR_RETURN_FALSE(d)

    /** @brief Reject synchronous reentrant changes until the active notification sequence ends. */
    if (d->modifying) return false;

    QScopedValueRollback<bool> guard{d->modifying, true};

    return d->remove(id);
}

bool Qtx::AbstractListModel::update(const cxx::item *item)
{
    Q_D(AbstractListModel);

    CHECK_PTR_RETURN_FALSE(d)

    /** @brief Reject synchronous reentrant changes until the active notification sequence ends. */
    if (d->modifying) return false;

    QScopedValueRollback<bool> guard{d->modifying, true};

    return d->update(item);
}

void Qtx::AbstractListModel::clear()
{
    Q_D(AbstractListModel);

    CHECK_PTR_RETURN(d)

    /** @brief Reject synchronous reentrant changes until the active notification sequence ends. */
    if (d->modifying) return;

    QScopedValueRollback<bool> guard{d->modifying, true};
    d->clear();
}

Qtx::AbstractListModel::AbstractListModel(AbstractListModelPrivate *dd, QObject *parent)
    :QAbstractListModel(parent),
    Object{dd}
{
}

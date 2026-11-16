/**
 * @file qtx_def.h
 * @brief Custom list model roles and their QML names.
 */

#ifndef QTX_DEF_H
#define QTX_DEF_H
#include <Qt>

namespace Qtx
{
    /** @brief Custom list roles, reserving User as the first role for derived models. */
    enum class ModelRole
    {
        Id = Qt::UserRole + 1,
        Name,
        User
    };

    /** @brief QML role name for the immutable entry ID. */
    static constexpr auto ID_ROLE_NAME{"id"};
    /** @brief QML role name for the entry name. */
    static constexpr auto NAME_ROLE_NAME{"name"};
}

#endif // QTX_DEF_H

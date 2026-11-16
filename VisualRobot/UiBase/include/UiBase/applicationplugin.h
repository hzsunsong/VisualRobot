/**
 * @file applicationplugin.h
 * @brief Common QObject base for application UI plugins.
 */

#ifndef APPLICATIONPLUGIN_H
#define APPLICATIONPLUGIN_H
#include <QObject>
#include "cxx/cxx_def.h"
#include "Ui/iapplicationplugin.h"
#include "uibase_export.h"

namespace Ui
{
    class Context;
}

namespace UiBase
{
    /**
     * @brief Provides shared context storage and a QML component factory for derived UI plugins.
     * @details Derived plugins supply their own metadata and override the operations
     * needed to expose their UI and manage its resources.
     */
    class UIBASE_EXPORT ApplicationPlugin : public QObject, public Ui::IApplicationPlugin
    {
        Q_OBJECT
        Q_INTERFACES(Ui::IApplicationPlugin)
    public:
        /**
         * @brief Exposes QObject construction for derived plugin implementations.
         */
        using QObject::QObject;
        /**
         * @brief Defaults construction and polymorphic destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION_V(ApplicationPlugin);

        /**
         * @brief Returns the borrowed host context, or nullptr before initialization.
         */
        Ui::Context *context() const
        {
            return m_context;
        }

        /**
         * @brief Stores the host context without taking ownership or extending its lifetime.
         * @param[in] context Shared context used to obtain the engine; const qualification is removed.
         */
        void context(const Ui::Context *context)
        {
            m_context = const_cast<Ui::Context *>(context);
        }

        /**
         * @brief Allocates a component with the shared engine and marks it as C++ owned.
         * @param[in] url Page URL passed to QQmlComponent.
         * @return Newly allocated component for the caller to delete, or nullptr if context, engine, or allocation is unavailable.
         * @note A non-null result can still contain QML loading errors; component status is not checked.
         */
        QQmlComponent *qmlComponent(const QString &url) const;
    private:
        /**
         * @brief Borrows host state; the host must keep it alive while the plugin uses it.
         */
        Ui::Context *m_context{nullptr};
    };
}

#endif // APPLICATIONPLUGIN_H

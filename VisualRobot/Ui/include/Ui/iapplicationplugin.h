/**
 * @file iapplicationplugin.h
 * @brief Qt application UI plugin interface and interface identifier.
 */

#ifndef IAPPLICATIONPLUGIN_H
#define IAPPLICATIONPLUGIN_H
#include <QString>
#include "cxx/cxx_def.h"

QT_BEGIN_NAMESPACE
class QQmlComponent;
QT_END_NAMESPACE

namespace Ui
{
    class Context;

    /**
     * @brief Defines metadata, initialization, page creation, and cleanup for Qt UI plugins.
     */
    class IApplicationPlugin
    {
    public:
        /**
         * @brief Enables polymorphic destruction and prevents copying or moving plugin instances.
         */
        DELETE_CLASS_DEFAULT_OPERATION_V(IApplicationPlugin);

        /**
         * @brief Supplies the key used to register and find the plugin.
         * @return Plugin name; names should be unique and stable across translations.
         */
        virtual QString name() const = 0;

        /**
         * @brief Supplies an icon URL usable by QML.
         * @return Resource or file URL for the plugin icon.
         */
        virtual QString icon() const = 0;

        /**
         * @brief Initializes the plugin with its shared context.
         * @param[in] context Borrowed host context used to access the QML engine.
         * @return Implementation-defined status; UiSet currently ignores this result.
         */
        virtual int initial(const Context *context) = 0;

        /**
         * @brief Creates a page component for caching by the host.
         * @return Component released by UiSet during cleanup, or nullptr if unavailable.
         */
        virtual QQmlComponent *qmlComponent() const = 0;

        /**
         * @brief Releases plugin resources before the host deletes cached components.
         * @return Implementation-defined status; UiSet currently ignores this result.
         */
        virtual int cleanup() = 0;
    };
}

QT_BEGIN_NAMESPACE
/**
 * @brief Identifies the interface used by Qt to cast loaded plugin instances.
 */
#define IApplicationPlugin_iid "org.qt-project.Qt.IApplicationPlugin/1.0.0"
Q_DECLARE_INTERFACE(Ui::IApplicationPlugin, IApplicationPlugin_iid)
QT_END_NAMESPACE

#endif // IAPPLICATIONPLUGIN_H

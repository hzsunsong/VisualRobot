/**
 * @file context.h
 * @brief Shared QML engine and plugin registration contract.
 */

#ifndef CONTEXT_H
#define CONTEXT_H
#include <qtconfigmacros.h>
#include "cxx/cxx_def.h"

QT_BEGIN_NAMESPACE
class QString;
class QQmlEngine;
QT_END_NAMESPACE

namespace Ui
{
    class IApplicationPlugin;

    /**
     * @brief Defines the engine and plugin registry shared with UI plugins.
     */
    class Context
    {
    public:
        /**
         * @brief Allows value operations and polymorphic destruction of the context interface.
         */
        CLASS_DEFAULT_OPERATION_V(Context);

        /**
         * @brief Gets the engine available to UI plugin component factories.
         * @return A borrowed engine pointer, or nullptr when none is configured.
         */
        virtual QQmlEngine *qmlEngine() const = 0;

        /**
         * @brief Supplies the engine used by UI plugins.
         * @param[in] qmlEngine Borrowed engine; the caller manages its lifetime.
         */
        virtual void qmlEngine(const QQmlEngine *qmlEngine) = 0;

        /**
         * @brief Registers a plugin under a lookup name.
         * @param[in] name Registry key.
         * @param[in] applicationPlugin Borrowed plugin pointer; registration does not transfer ownership.
         */
        virtual void registerPlugin(const QString &name, const IApplicationPlugin *applicationPlugin) = 0;

        /**
         * @brief Removes a plugin registration without destroying the plugin.
         * @param[in] name Registry key to remove.
         */
        virtual void unregisterPlugin(const QString &name) = 0;
    };
}

#endif // CONTEXT_H

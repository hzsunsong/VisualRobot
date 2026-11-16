/**
 * @file uicontext.h
 * @brief Borrowed QML engine and named UI plugin registrations.
 */

#ifndef UICONTEXT_H
#define UICONTEXT_H
#include <QMap>
#include "context.h"

namespace Ui
{
    /**
     * @brief Stores borrowed pointers to the QML engine and registered UI plugins.
     */
    class UiContext final : public Context
    {
    public:
        /**
         * @brief Returns the configured engine without transferring ownership.
         */
        QQmlEngine *qmlEngine() const override
        {
            return m_qmlEngine;
        }

        /**
         * @brief Stores the engine after removing const qualification; does not extend its lifetime.
         * @param[in] qmlEngine Borrowed engine used for component construction.
         */
        void qmlEngine(const QQmlEngine *qmlEngine) override
        {
            m_qmlEngine = const_cast<QQmlEngine *>(qmlEngine);
        }

        /**
         * @brief Records a borrowed plugin pointer only if its name is not already registered.
         * @param[in] name Registry key.
         * @param[in] applicationPlugin Pointer stored without ownership or null validation.
         */
        void registerPlugin(const QString &name, const IApplicationPlugin *applicationPlugin) override;

        /**
         * @brief Removes the named entry if present without deleting its plugin.
         * @param[in] name Registry key.
         */
        void unregisterPlugin(const QString &name) override;
    private:
        QQmlEngine *m_qmlEngine{nullptr};
        QMap<QString, IApplicationPlugin *> m_applicationPlugins;
    };
}

#endif // UICONTEXT_H

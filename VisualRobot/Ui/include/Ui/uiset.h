/**
 * @file uiset.h
 * @brief Discovery, lifecycle management, and cached resources of Qt UI plugins.
 */

#ifndef UISET_H
#define UISET_H
#include <memory>
#include <QObject>
#include <QMap>
#include "cxx/cxx_def.h"
#include "ui_export.h"

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QQmlComponent;
QT_END_NAMESPACE

namespace Ui
{
    class IApplicationPlugin;
    class Context;

    /**
     * @brief Loads Qt UI plugins and caches their icons and page components.
     * @note Plugin instances are managed by Qt; this class deletes cached components.
     */
    class UI_EXPORT UiSet : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Allocates the shared context for plugin initialization.
         * @param[in] parent QObject parent controlling host ownership.
         */
        explicit UiSet(QObject *parent = nullptr);
        /**
         * @brief Prevents duplication or transfer of cached component ownership.
         */
        DELETE_CLASS_DEFAULT_COPY_MOVE(UiSet);
        /**
         * @brief Cleans up plugins and deletes cached components before destroying the context.
         */
        ~UiSet() override;

        /**
         * @brief Cleans up the previous cache and scans plugins/ui below the executable directory.
         * @param[in] qmlEngine Borrowed engine supplied to the shared context.
         * @return Names of accepted interface instances, including those without a component.
         * @note Initialization results and component readiness are not checked; duplicate names overwrite cache entries.
         */
        QStringList loadPlugin(const QQmlEngine *qmlEngine);
        /**
         * @brief Looks up the cached icon URL.
         * @param[in] pluginName Plugin registration key.
         * @return Cached URL, or an empty string when not found.
         */
        QString icon(const QString &pluginName) const;
        /**
         * @brief Looks up a cached page component.
         * @param[in] pluginName Plugin registration key.
         * @return Borrowed component, or nullptr when not found; invalidated by cleanup or reloading.
         */
        QQmlComponent *qmlComponent(const QString &pluginName) const;
    private:
        /**
         * @brief Delegates final resource release to cleanupPlugin().
         */
        void cleanup();
        /**
         * @brief Unregisters plugins, calls their cleanup hooks, deletes components, and clears caches.
         * @note Plugin instances are not explicitly deleted or unloaded here.
         */
        void cleanupPlugin();

        /**
         * @brief Owns the context borrowed by plugins during initialization and component creation.
         */
        std::unique_ptr<Context> m_context;
        /**
         * @brief Tracks Qt-managed plugin instances without owning their pointers.
         */
        QMap<QString, IApplicationPlugin *> m_applicationPlugins;
        QMap<QString, QString> m_icons;
        /**
         * @brief Owns the cached components returned by plugin factories.
         */
        QMap<QString, QQmlComponent *> m_qmlComponents;
    };
}

#endif // UISET_H

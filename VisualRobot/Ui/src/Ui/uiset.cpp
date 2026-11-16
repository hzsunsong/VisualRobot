/**
 * @file uiset.cpp
 * @brief Qt plugin discovery and cleanup of cached QML components.
 */

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>
#include <QQmlComponent>
#include "cxx/cxx_def.h"
#include "cxx/cxx_kit.h"
#include "uiset.h"
#include "uicontext.h"
#include "iapplicationplugin.h"

namespace
{
    static constexpr auto PLUGIN_DIR{"plugins/ui"};
}

Ui::UiSet::UiSet(QObject *parent)
    : QObject{parent},
    m_context{new(std::nothrow) UiContext}
{}

Ui::UiSet::~UiSet()
{
    cleanup();
}

QStringList Ui::UiSet::loadPlugin(const QQmlEngine *qmlEngine)
{
    cleanupPlugin();

    CHECK_PTR_RETURN_VALUE(m_context, {})

    m_context->qmlEngine(qmlEngine);

    auto pluginDir{QDir::toNativeSeparators(QCoreApplication::applicationDirPath())};

    if (!pluginDir.endsWith(QDir::separator())) pluginDir += QDir::separator();

    pluginDir += QDir::toNativeSeparators(PLUGIN_DIR);
    const auto files{QDir{pluginDir}.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)};
    QStringList pluginNames;

    for (const auto &file : files)
    {
        QPluginLoader pluginLoader{file.absoluteFilePath()};
        const auto instance{pluginLoader.instance()};

        if (!instance)
        {
            qDebug() << Q_FUNC_INFO << ">>>load the application plugin failed!path:" << file << ", error:" << pluginLoader.errorString();

            continue;
        }

        const auto applicationPlugin{qobject_cast<IApplicationPlugin *>(instance)};

        CHECK_PTR_CONTINUE(applicationPlugin)

        /**
         * @brief Registers every accepted interface even when its initialization hook reports failure.
         */
        applicationPlugin->initial(m_context.get());
        const auto name{applicationPlugin->name()};
        const auto icon{applicationPlugin->icon()};
        m_context->registerPlugin(name, applicationPlugin);
        m_icons.insert(name, icon);
        m_applicationPlugins.insert(name, applicationPlugin);
        pluginNames << name;
        const auto qmlComponent{applicationPlugin->qmlComponent()};

        CHECK_PTR_CONTINUE(qmlComponent)

        /**
         * @brief Caches non-null components without checking QML load status; cleanup deletes the cached pointers.
         */
        m_qmlComponents.insert(name, qmlComponent);
    }

    return pluginNames;
}

QString Ui::UiSet::icon(const QString &pluginName) const
{
    if (!m_icons.contains(pluginName)) return {};

    return m_icons[pluginName];
}

QQmlComponent *Ui::UiSet::qmlComponent(const QString &pluginName) const
{
    if (!m_qmlComponents.contains(pluginName)) return {};

    return m_qmlComponents[pluginName];
}

void Ui::UiSet::cleanup()
{
    cleanupPlugin();
}

void Ui::UiSet::cleanupPlugin()
{
    CHECK_PTR_RETURN(m_context)

    QMapIterator<QString, IApplicationPlugin *> mapItor{m_applicationPlugins};

    while (mapItor.hasNext())
    {
        mapItor.next();
        const auto name{mapItor.key()};
        m_context->unregisterPlugin(name);
        const auto applicationPlugin{mapItor.value()};

        CHECK_PTR_CONTINUE(applicationPlugin)

        applicationPlugin->cleanup();
    }

    QMapIterator<QString, QQmlComponent *> mapItor2{m_qmlComponents};

    while (mapItor2.hasNext())
    {
        mapItor2.next();
        auto qmlComponent{mapItor2.value()};

        CHECK_PTR_CONTINUE(qmlComponent)

        DELETE(qmlComponent);
    }

    cxx::free(m_applicationPlugins);
    cxx::free(m_icons);
    cxx::free(m_qmlComponents);
}

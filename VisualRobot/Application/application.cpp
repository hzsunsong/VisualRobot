/**
 * @file application.cpp
 * @brief QML context registration, device opening, and device/UI plugin resource lookup.
 */

#include <QQmlEngine>
#include <QQmlContext>
#include "cxx/cxx_kit.h"
#include "Device/device_def.h"
#include "Device/devicepluginset.h"
#include "Ui/uiset.h"
#include "application.h"

Application::Application::Application(QQmlEngine *qmlEngine, QObject *parent)
    :QObject{parent},
    m_qmlEngine{qmlEngine},
    m_devicePluginSet{new(std::nothrow) Device::DevicePluginSet},
    m_uiSet{new(std::nothrow) Ui::UiSet}
{
    initial();
}

bool Application::Application::openDevice(const QString &devicePluginName, Device::DEVICE_ID deviceId)
{
    CHECK_PTR_RETURN_FALSE(m_devicePluginSet)

    return m_devicePluginSet->openDevice(std::string(devicePluginName.toLocal8Bit().constData()), deviceId, Device::ConnectType::Network);
}

QString Application::Application::icon(const QString &applicationPluginName) const
{
    if (!m_uiSet) return {};

    return m_uiSet->icon(applicationPluginName);
}

QQmlComponent *Application::Application::qmlComponent(const QString &applicationPluginName) const
{
    if (!m_uiSet) return {};

    return m_uiSet->qmlComponent(applicationPluginName);
}

void Application::Application::initial()
{
    initialContextProperties();
    loadPlugin();
}

void Application::Application::initialContextProperties()
{
    CHECK_PTR_RETURN(m_qmlEngine)
    const auto rootContext{m_qmlEngine->rootContext()};

    CHECK_PTR_RETURN(rootContext)

    /**
     * @brief Makes the application API available before QML components are loaded.
     */
    rootContext->setContextProperty("$app", this);
}

void Application::Application::loadPlugin()
{
    CHECK_PTR_RETURN(m_devicePluginSet)

    CHECK_PTR_RETURN(m_uiSet)

    cxx::free(m_devicePluginNames);
    cxx::free(m_uiPluginNames);

    m_devicePluginNames = m_devicePluginSet->loadPlugin();
    m_uiPluginNames = m_uiSet->loadPlugin(m_qmlEngine);
}

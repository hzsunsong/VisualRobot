/**
 * @file application.h
 * @brief QObject application API exposed to the QML root context.
 */

#ifndef APPLICATION_H
#define APPLICATION_H
#include <QObject>
#include "cxx/cxx_def.h"
#include "Device/device_def.h"

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QQmlComponent;
QT_END_NAMESPACE

namespace Device
{
    class DevicePluginSet;
}

namespace Ui
{
    class UiSet;
}

namespace Application
{
    /**
     * @brief Exposes device opening and UI plugin resource lookup to QML through the root context.
     */
    class Application : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Registers the QML context object and loads device and UI plugins.
         * @param[in] qmlEngine The borrowed engine used to register the $app context property.
         * @param[in] parent The QObject parent controlling this object's Qt ownership.
         */
        explicit Application(QQmlEngine *qmlEngine, QObject *parent = nullptr);

        /**
         * @brief Opens a device through its plugin using the network transport.
         * @param[in] devicePluginName The plugin registration key, converted to local 8-bit text.
         * @param[in] deviceId The identifier within the plugin's device set.
         * @return True when the plugin manager exists and the device reports successful opening.
         */
        Q_INVOKABLE bool openDevice(const QString &devicePluginName, Device::DEVICE_ID deviceId);

        /**
         * @brief Looks up the icon for a UI plugin.
         * @param[in] applicationPluginName Exact UI plugin registration key.
         * @return Cached URL, or an empty string if the manager or plugin is unavailable.
         */
        Q_INVOKABLE QString icon(const QString &applicationPluginName) const;

        /**
         * @brief Looks up the page component for a UI plugin.
         * @param[in] applicationPluginName Exact UI plugin registration key.
         * @return Borrowed component, or nullptr if unavailable; owned by UiSet.
         */
        Q_INVOKABLE QQmlComponent *qmlComponent(const QString &applicationPluginName) const;
    private:
        /**
         * @brief Registers context properties before loading device and UI plugins.
         */
        void initial();
        /**
         * @brief Publishes this object as $app when the engine and its root context are available.
         */
        void initialContextProperties();
        /**
         * @brief Replaces both recorded plugin name lists with the managers' load results.
         * @note Both managers clean up previous registrations before loading again; cached UI component pointers are invalidated.
         */
        void loadPlugin();

        /**
         * @brief Borrows the QML engine; this object does not own or delete it.
         */
        QQmlEngine *m_qmlEngine{nullptr};
        std::unique_ptr<Device::DevicePluginSet> m_devicePluginSet;
        std::unique_ptr<Ui::UiSet> m_uiSet;
        cxx::string_list<> m_devicePluginNames;
        QStringList m_uiPluginNames;
    };
}

#endif // APPLICATION_H

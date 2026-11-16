/**
 * @file remotecontrolplugin.h
 * @brief Exposes the remote control page through the application UI plugin interface.
 */

#ifndef REMOTECONTROLPLUGIN_H
#define REMOTECONTROLPLUGIN_H
#include <QObject>
#include "cxx/cxx_def.h"
#include "UiBase/applicationplugin.h"

namespace RemoteControl
{
    /** @brief Uses the shared UI context to create the remote control QML component. */
    class RemoteControlPlugin final : public UiBase::ApplicationPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.RemoteControlPlugin")
        Q_INTERFACES(Ui::IApplicationPlugin)
    public:
        /**
         * @brief Defaults construction and destruction and prevents copying or moving the plugin.
         */
        DELETE_CLASS_DEFAULT_OPERATION(RemoteControlPlugin);

        /**
         * @brief Returns the translated registration key.
         * @return QObject::tr("RemoteControlPlugin").
         * @note Translations must preserve the key expected by application lookups.
         */
        virtual QString name() const override;

        /**
         * @brief Returns the plugin icon resource URL.
         * @return qrc:/RemoteControl/image/remotecontrol.svg.
         */
        virtual QString icon() const override;

        /**
         * @brief Stores the borrowed context without validation.
         * @param[in] context Shared UI context used for component creation.
         * @return The integer value of Ui::ErrorCode::NoError.
         */
        virtual int initial(const Ui::Context *context) override;

        /**
         * @brief Creates the page from qrc:/RemoteControl/qml/Main.qml.
         * @return New C++-owned component for the host to delete, or nullptr if unavailable.
         * @note Component loading status is not checked.
         */
        virtual QQmlComponent *qmlComponent() const override;

        /**
         * @brief Returns success without releasing resources or clearing the borrowed context.
         * @return The integer value of Ui::ErrorCode::NoError.
         */
        virtual int cleanup() override;
    };
}

#endif // REMOTECONTROLPLUGIN_H

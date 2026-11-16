/**
 * @file remotecontrolplugin.cpp
 * @brief Remote control plugin metadata, context binding, and page creation.
 */

#include "Ui/ui_def.h"
#include "remotecontrolplugin.h"

QString RemoteControl::RemoteControlPlugin::name() const
{
    return QObject::tr("RemoteControlPlugin");
}

QString RemoteControl::RemoteControlPlugin::icon() const
{
    return "qrc:/RemoteControl/image/remotecontrol.svg";
}

int RemoteControl::RemoteControlPlugin::initial(const Ui::Context *context)
{
    UiBase::ApplicationPlugin::context(context);

    return static_cast<int>(Ui::ErrorCode::NoError);
}

QQmlComponent *RemoteControl::RemoteControlPlugin::qmlComponent() const
{
    /** @brief Keeps the page resource distinct when other UI plugins are loaded. */
    return UiBase::ApplicationPlugin::qmlComponent("qrc:/RemoteControl/qml/Main.qml");
}

int RemoteControl::RemoteControlPlugin::cleanup()
{
    return static_cast<int>(Ui::ErrorCode::NoError);
}

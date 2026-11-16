/**
 * @file homeplugin.cpp
 * @brief Home plugin metadata, context binding, and page creation.
 */

#include "Ui/ui_def.h"
#include "homeplugin.h"

QString Home::HomePlugin::name() const
{
    return QObject::tr("HomePlugin");
}

QString Home::HomePlugin::icon() const
{
    return "qrc:/Home/image/home.jpg";
}

int Home::HomePlugin::initial(const Ui::Context *context)
{
    UiBase::ApplicationPlugin::context(context);

    return static_cast<int>(Ui::ErrorCode::NoError);
}

QQmlComponent *Home::HomePlugin::qmlComponent() const
{
    return UiBase::ApplicationPlugin::qmlComponent("qrc:/qml/Main.qml");
}

int Home::HomePlugin::cleanup()
{
    return static_cast<int>(Ui::ErrorCode::NoError);
}

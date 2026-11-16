/**
 * @file homeplugin.h
 * @brief Home page metadata and QML component factory.
 */

#ifndef HOMEPLUGIN_H
#define HOMEPLUGIN_H
#include <QObject>
#include "cxx/cxx_def.h"
#include "UiBase/applicationplugin.h"

namespace Home
{
    /**
     * @brief Exposes robot selection through the shared application UI plugin interface.
     */
    class HomePlugin final : public UiBase::ApplicationPlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.HomePlugin")
        Q_INTERFACES(Ui::IApplicationPlugin)
    public:
        /**
         * @brief Defaults construction and destruction and prevents copying or moving the plugin.
         */
        DELETE_CLASS_DEFAULT_OPERATION(HomePlugin);

        /**
         * @brief Returns the translated registration key.
         * @return QObject::tr("HomePlugin").
         * @note Translations must preserve the key expected by application lookups.
         */
        virtual QString name() const override;

        /**
         * @brief Returns the plugin icon resource URL.
         * @return qrc:/Home/image/home.jpg.
         */
        virtual QString icon() const override;

        /**
         * @brief Stores the borrowed host context for subsequent page construction.
         * @param[in] context Shared UI context; no validation is performed.
         * @return The integer value of Ui::ErrorCode::NoError.
         */
        virtual int initial(const Ui::Context *context) override;

        /**
         * @brief Creates the home page from qrc:/qml/Main.qml using the shared engine.
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

#endif // HOMEPLUGIN_H

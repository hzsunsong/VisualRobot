/**
 * @file applicationplugin.cpp
 * @brief Creation of C++-owned QML components using the shared engine.
 */

#include <QQmlEngine>
#include <QQmlComponent>
#include "cxx/cxx_def.h"
#include "Ui/context.h"
#include "applicationplugin.h"

QQmlComponent *UiBase::ApplicationPlugin::qmlComponent(const QString &url) const
{
    CHECK_PTR_RETURN_VALUE(m_context, nullptr)

    const auto qmlEngine{m_context->qmlEngine()};

    CHECK_PTR_RETURN_VALUE(qmlEngine, nullptr)

    const auto qmlComponent{new(std::nothrow) QQmlComponent{qmlEngine, url}};

    CHECK_PTR_RETURN_VALUE(qmlComponent, nullptr)

    /**
     * @brief Keeps deletion under C++ host control instead of QML garbage collection.
     */
    QQmlEngine::setObjectOwnership(qmlComponent, QJSEngine::CppOwnership);

    return qmlComponent;
}
/**
 * @file uicontext.cpp
 * @brief Non-owning plugin registration with duplicate-name protection.
 */

#include "uicontext.h"

void Ui::UiContext::registerPlugin(const QString &name, const IApplicationPlugin *applicationPlugin)
{
    if (m_applicationPlugins.contains(name)) return;

    m_applicationPlugins.insert(name, const_cast<IApplicationPlugin *>(applicationPlugin));
}

void Ui::UiContext::unregisterPlugin(const QString &name)
{
    if (!m_applicationPlugins.contains(name)) return;

    m_applicationPlugins.remove(name);
}

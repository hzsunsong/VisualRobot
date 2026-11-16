/**
 * @file devicepluginset.cpp
 * @brief Device provider discovery, registration, device opening, and terminal cleanup.
 */

#include <algorithm>
#include "cxx/cxx_kit.h"
#include "cxx/logger.h"
#include "Device/endpoint.h"
#include "Device/device.h"
#include "devicepluginset.h"
#include "ideviceplugin.h"
#include "deviceset.h"

namespace
{
    static constexpr auto PLUGIN_DIR{"plugins/device"};
}

Device::DevicePluginSet::~DevicePluginSet()
{
    cleanup();
}

cxx::string_list<> Device::DevicePluginSet::loadPlugin()
{
    cleanupPlugin();

    CHECK_PTR_RETURN_VALUE(m_pluma, {})

    m_pluma->acceptProviderType<IDevicePluginProvider>();
    cxx::logger::info("the device plugin loaded.pluginCount: {}",
                      m_pluma->loadFromFolder(PLUGIN_DIR, true));
    std::vector<IDevicePluginProvider *> providers;
    m_pluma->getProviders(providers);
    cxx::string_list<> devicePluginNames;

    for (const auto &provider : providers)
    {
        if (!provider)
        {
            cxx::logger::error("the provider is nullptr.");

            continue;
        }

        const auto devicePlugin{provider->create()};

        if (!devicePlugin)
        {
            cxx::logger::error("the devicePlugin is nullptr.provider:{}", reinterpret_cast<std::uintptr_t>(provider));

            continue;
        }

        const auto name{devicePlugin->name()};

        if (name.empty())
        {
            cxx::logger::error("the device plugin name is nullptr.devicePlugin:{}", reinterpret_cast<std::uintptr_t>(devicePlugin));

            continue;
        }

        /**
         * @brief Records the name before initialization, so the result is not a list of registered plugins.
         */
        devicePluginNames.emplace_back(name);

        if (0 != devicePlugin->initial())
        {
            cxx::logger::error("initial the device plugin failed.devicePlugin:{}", reinterpret_cast<std::uintptr_t>(devicePlugin));

            continue;
        }

        const auto deviceSet{devicePlugin->createDeviceSet()};

        if (!deviceSet)
        {
            cxx::logger::error("create the device set failed.devicePlugin:{}", reinterpret_cast<std::uintptr_t>(devicePlugin));

            continue;
        }

        m_devicePlugins.emplace(name, devicePlugin);
        m_deviceSets.emplace(name, deviceSet);
    }

    return devicePluginNames;
}

bool Device::DevicePluginSet::openDevice(const std::string &devicePluginName, ::Device::DEVICE_ID deviceId, ConnectType connectType)
{
    const auto itor{m_deviceSets.find(devicePluginName)};

    if (m_deviceSets.cend() == itor) return false;

    const auto deviceSet{itor->second};

    CHECK_PTR_RETURN_FALSE(deviceSet)

    const auto device{deviceSet->device(deviceId)};

    CHECK_PTR_RETURN_FALSE(device)

    std::unique_ptr<Endpoint> endpoint;

    switch (connectType)
    {
        case ConnectType::Network:
        {
            /**
             * @brief Uses the current fixed network endpoint; no per-device endpoint configuration is applied.
             */
            endpoint.reset(new(std::nothrow) NetworkEndpoint{"192.168.0.1", 5050});
        }
            break;
        default:
        {
            assert(false);
        }
            break;
    }

    CHECK_PTR_RETURN_FALSE(endpoint)

    return static_cast<int>(::Device::ErrorCode::NoError) == device->open(*endpoint);
}

void Device::DevicePluginSet::cleanup()
{
    cleanupPlugin();
}

void Device::DevicePluginSet::cleanupPlugin()
{
    std::for_each(m_devicePlugins.cbegin(), m_devicePlugins.cend(), [=](const auto &itor){
        IDevicePlugin *devicePlugin{itor.second};
        CHECK_PTR_RETURN(devicePlugin)

        if (0 != devicePlugin->cleanup())
        {
            cxx::logger::error("cleanup the device plugin failed.devicePlugin:{}", reinterpret_cast<std::uintptr_t>(devicePlugin));
        }

        DELETE(devicePlugin);
    });

    std::for_each(m_deviceSets.cbegin(), m_deviceSets.cend(), [=](const auto &itor){
        DeviceSet *deviceSet{itor.second};
        CHECK_PTR_RETURN(deviceSet)

        DELETE(deviceSet);
    });

    /**
     * @brief Unloads plugin code only after registered plugin and device set objects have been deleted.
     */
    if (m_pluma)
    {
        m_pluma->unloadAll();
    }

    /**
     * @brief Clears pointer entries only after deleting their pointees and unloading plugin code.
     */
    cxx::free(m_devicePlugins);
    cxx::free(m_deviceSets);
}

/**
 * @file devicepluginset.h
 * @brief Ownership and lookup of dynamically loaded device plugins and device sets.
 */

#ifndef DEVICEPLUGINSET_H
#define DEVICEPLUGINSET_H
#include <string>
#include <map>
#include <memory>
#include "Pluma/Pluma.hpp"
#include "cxx/cxx_def.h"
#include "Device/device_def.h"
#include "device_export.h"

namespace Device
{
    class IDevicePlugin;
    class DeviceSet;

    /**
     * @brief Loads device plugins and owns their registered plugin and device set instances.
     * @note Reloading deletes previous registered instances and unloads their libraries before discovery.
     */
    class DEVICE_EXPORT DevicePluginSet
    {
    public:
        /**
         * @brief Allocates the Pluma host; allocation failure is checked by loadPlugin().
         */
        DevicePluginSet() = default;
        /**
         * @brief Prevents copying an object responsible for device identity or owned resources.
         */
        DevicePluginSet(const DevicePluginSet &) = delete;
        /**
         * @brief Prevents copy assignment from duplicating device identity or resource ownership.
         */
        DevicePluginSet &operator=(const DevicePluginSet &) = delete;
        /**
         * @brief Prevents moving the host and its registered object ownership.
         */
        DevicePluginSet(DevicePluginSet &&) = delete;
        /**
         * @brief Prevents move assignment from replacing the host and registered objects.
         */
        DevicePluginSet &operator=(DevicePluginSet &&) = delete;
        /**
         * @brief Cleans up registered plugins and device sets before unloading their libraries.
         */
        ~DevicePluginSet();

        /**
         * @brief Loads providers recursively from plugins/device and initializes their plugins.
         * @return Non-empty plugin names encountered before initialization and device set creation.
         * The list can therefore include plugins that were not successfully registered.
         * @note Previous registered objects are cleaned up and their libraries unloaded before loading.
         */
        cxx::string_list<> loadPlugin();
        /**
         * @brief Looks up a registered device set and opens its device.
         * @param[in] devicePluginName The exact plugin name used as the registration key.
         * @param[in] deviceId The device identifier, created on demand by the set.
         * @param[in] connectType The requested transport; currently only Network is supported.
         * @return True only if lookup and endpoint creation succeed and open() returns NoError.
         * @note Network connections currently use the fixed endpoint 192.168.0.1:5050.
         * Unsupported transports trigger an assertion and otherwise return false.
         */
        bool openDevice(const std::string &devicePluginName, ::Device::DEVICE_ID deviceId, ConnectType connectType);
    private:
        /**
         * @brief Calls plugin cleanup, deletes registered objects, and unloads all plugin libraries.
         * @note Pointer maps are cleared after their registered objects are deleted.
         */
        void cleanup();

        /**
         * @brief Calls plugin cleanup hooks, deletes plugin instances and device sets, unloads libraries, and clears both maps.
         * @note Plugin objects are deleted before device sets; cleanup hook failures are logged.
         */
        void cleanupPlugin();

        std::unique_ptr<pluma::Pluma> m_pluma{new(std::nothrow) pluma::Pluma};
        /**
         * @brief Owns plugin pointers registered after successful initialization and set creation.
         */
        std::map<std::string, IDevicePlugin *> m_devicePlugins;
        /**
         * @brief Owns device set pointers indexed by the same keys as the registered plugins.
         */
        std::map<std::string, DeviceSet *> m_deviceSets;
    };
}

#endif // DEVICEPLUGINSET_H

/**
 * @file IDevicePlugin.h
 * @brief Pluma device plugin lifecycle and device set factory contract.
 */

#ifndef IDEVICEPLUGIN_H
#define IDEVICEPLUGIN_H
#include "Pluma/Pluma.hpp"
#include "cxx/cxx_def.h"

namespace Device
{
    class DeviceSet;

    /**
     * @brief Defines the lifecycle and device factory interface for Pluma plugins.
     */
    class IDevicePlugin
    {
    public:
        /**
         * @brief Defaults construction and virtual destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION_V(IDevicePlugin);

        /**
         * @brief Gets the plugin lookup key.
         * @return A non-empty name; registered plugins should use distinct names.
         */
        virtual std::string name() const = 0;

        /**
         * @brief Initializes plugin resources before creating its device set.
         * @return Zero on success; any nonzero value causes the loader to skip registration.
         */
        virtual int initial() = 0;

        /**
         * @brief Creates the device set associated with the plugin.
         * @return A new device set owned by the caller, or nullptr on failure.
         */
        virtual DeviceSet *createDeviceSet() = 0;

        /**
         * @brief Releases plugin resources before the plugin is deleted and unloaded.
         * @return Zero on success, or a nonzero status logged by the plugin manager.
         * @note The plugin manager releases the device set separately.
         */
        virtual int cleanup() = 0;
    };

    /**
     * @brief Declares the provider type accepted by the Pluma device plugin host.
     */
    PLUMA_PROVIDER_HEADER(IDevicePlugin)
}

#endif // IDEVICEPLUGIN_H

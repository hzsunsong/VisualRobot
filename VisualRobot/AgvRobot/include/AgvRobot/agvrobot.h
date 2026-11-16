/**
 * @file agvrobot.h
 * @brief AGV plugin lifecycle and device set factory.
 */

#ifndef AGVROBOT_H
#define AGVROBOT_H
#include "Pluma/Pluma.hpp"
#include "cxx/cxx_def.h"
#include "Device/ideviceplugin.h"

namespace Device
{
    class DeviceSet;
}

namespace AgvRobot
{
    /**
     * @brief Provides the AGV device plugin for the Pluma host.
     */
    class AgvRobot final : public Device::IDevicePlugin
    {
    public:
        /**
         * @brief Defaults construction and destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION(AgvRobot);

        /**
         * @brief Gets the identifier used to select this plugin.
         * @return The literal "AgvRobotPlugin".
         */
        std::string name() const override;

        /**
         * @brief Logs plugin initialization without allocating device resources.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int initial() override;

        /**
         * @brief Allocates a AgvRobotSet for this plugin.
         * @return A new device set owned by the caller, or nullptr if allocation fails.
         */
        Device::DeviceSet *createDeviceSet() override;

        /**
         * @brief Logs plugin cleanup; the device set is released separately by its owner.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int cleanup() override;
    };

    /**
     * @brief Declares the provider used by Pluma to instantiate the AGV plugin.
     */
    PLUMA_INHERIT_PROVIDER(AgvRobot, Device::IDevicePlugin)
}

#endif // AGVROBOT_H

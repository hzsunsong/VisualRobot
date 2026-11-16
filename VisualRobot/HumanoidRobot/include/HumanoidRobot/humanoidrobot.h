/**
 * @file humanoidrobot.h
 * @brief Humanoid robot plugin lifecycle and generic device set factory.
 */

#ifndef HUMANOIDROBOT_H
#define HUMANOIDROBOT_H
#include "Pluma/Pluma.hpp"
#include "cxx/cxx_def.h"
#include "Device/ideviceplugin.h"

namespace Device
{
class DeviceSet;
}

namespace HumanoidRobot
{
    /**
     * @brief Provides the humanoid robot device plugin for the Pluma host.
     */
    class HumanoidRobot final : public Device::IDevicePlugin
    {
    public:
        /**
         * @brief Defaults construction and destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION(HumanoidRobot);

        /**
         * @brief Gets the identifier used to select this plugin.
         * @return The literal "HumanoidRobotPlugin".
         */
        std::string name() const override;

        /**
         * @brief Logs plugin initialization without allocating device resources.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int initial() override;

        /**
         * @brief Allocates a DeviceBase::BaseDeviceSet for this plugin.
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
     * @brief Declares the provider used by Pluma to instantiate the humanoid robot plugin.
     */
    PLUMA_INHERIT_PROVIDER(HumanoidRobot, Device::IDevicePlugin)
}

#endif // HUMANOIDROBOT_H

/**
 * @file humanoidrobot.cpp
 * @brief Humanoid plugin lifecycle logging and generic device set allocation.
 */

#include "cxx/logger.h"
#include "Device/device_def.h"
#include "DeviceBase/basedeviceset.h"
#include "humanoidrobot.h"

namespace Device
{
    PLUMA_PROVIDER_SOURCE(IDevicePlugin, DEVICE_PLUGIN_VERSION, DEVICE_PLUGIN_LOWEST_VERSION)
}

std::string HumanoidRobot::HumanoidRobot::name() const
{
    return "HumanoidRobotPlugin";
}

int HumanoidRobot::HumanoidRobot::initial()
{
    cxx::logger::info("initial the humanoid robot.");

    return static_cast<int>(Device::ErrorCode::NoError);
}

Device::DeviceSet *HumanoidRobot::HumanoidRobot::createDeviceSet()
{
    cxx::logger::info("create the humanoid robot device set.");

    /**
     * @brief Uses the generic cache and placeholder devices; no humanoid-specific transport is created.
     */
    return new(std::nothrow) DeviceBase::BaseDeviceSet;
}

int HumanoidRobot::HumanoidRobot::cleanup()
{
    cxx::logger::info("cleanup the humanoid robot.");

    return static_cast<int>(Device::ErrorCode::NoError);
}

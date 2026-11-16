/**
 * @file agvrobot.cpp
 * @brief AGV plugin name, lifecycle logging, and device set allocation.
 */

#include "cxx/logger.h"
#include "Device/device_def.h"
#include "agvrobotset.h"
#include "agvrobot.h"

namespace Device
{
    PLUMA_PROVIDER_SOURCE(IDevicePlugin, DEVICE_PLUGIN_VERSION, DEVICE_PLUGIN_LOWEST_VERSION)
}

std::string AgvRobot::AgvRobot::name() const
{
    return "AgvRobotPlugin";
}

int AgvRobot::AgvRobot::initial()
{
    cxx::logger::info("initial the agv robot.");

    return static_cast<int>(Device::ErrorCode::NoError);
}

Device::DeviceSet *AgvRobot::AgvRobot::createDeviceSet()
{
    cxx::logger::info("create the agv robot device set.");

    return new(std::nothrow) AgvRobotSet;
}

int AgvRobot::AgvRobot::cleanup()
{
    cxx::logger::info("cleanup the agv robot.");

    return static_cast<int>(Device::ErrorCode::NoError);
}

/**
 * @file agvrobotset.cpp
 * @brief AGV device allocation for the inherited device cache.
 */

#include "agvrobotset.h"
#include "agvrobotdevice.h"

Device::Device *AgvRobot::AgvRobotSet::createDevice(Device::DEVICE_ID deviceId)
{
    return new(std::nothrow) AgvRobotDevice{deviceId};
}

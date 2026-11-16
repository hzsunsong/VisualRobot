/**
 * @file agvrobotset.h
 * @brief Device set that creates and caches AGV device instances.
 */

#ifndef AGVROBOTSET_H
#define AGVROBOTSET_H
#include "cxx/cxx_def.h"
#include "DeviceBase/basedeviceset.h"

namespace AgvRobot
{
    /**
     * @brief Caches AGV devices by identifier using the base device set.
     */
    class AgvRobotSet : public DeviceBase::BaseDeviceSet
    {
    public:
        /**
         * @brief Defaults construction and virtual destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION_V(AgvRobotSet);

    protected:
        /**
         * @brief Allocates an AGV device for the inherited device cache.
         * @param[in] deviceId The identifier assigned to the new device.
         * @return A new device owned by the device set, or nullptr if allocation fails.
         */
        Device::Device *createDevice(Device::DEVICE_ID deviceId) override;
    };
}

#endif // AGVROBOTSET_H

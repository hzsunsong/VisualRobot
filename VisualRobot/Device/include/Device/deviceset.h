/**
 * @file deviceset.h
 * @brief Abstract device lookup and factory API.
 */

#ifndef DEVICESET_H
#define DEVICESET_H
#include "cxx/cxx_def.h"
#include "Device/device_def.h"

namespace Device
{
    class Device;

    /**
     * @brief Defines lookup and creation of devices within a plugin.
     * @details Ownership of returned devices is managed by the device set implementation.
     */
    class DeviceSet
    {
    public:
        /**
         * @brief Defaults construction and virtual destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION_V(DeviceSet);

        /**
         * @brief Looks up a device, allowing the implementation to create it on demand.
         * @param[in] deviceId The identifier within this set.
         * @return A device pointer, or nullptr if it cannot be obtained.
         */
        virtual Device *device(DEVICE_ID deviceId) = 0;
    protected:
        /**
         * @brief Creates a device for a lookup that cannot be satisfied by the implementation.
         * @param[in] deviceId The identifier to assign to the device.
         * @return A new device, or nullptr if creation fails.
         */
        virtual Device *createDevice(DEVICE_ID deviceId) = 0;
    };
}

#endif // DEVICESET_H

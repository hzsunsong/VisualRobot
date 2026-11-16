/**
 * @file basedeviceset.h
 * @brief Owning device cache with an overridable creation hook.
 */

#ifndef BASEDEVICESET_H
#define BASEDEVICESET_H
#include <map>
#include "Device/device.h"
#include "Device/deviceset.h"
#include "devicebase_export.h"

namespace DeviceBase
{
    /**
     * @brief Owns devices and caches them by identifier until destruction.
     */
    class DEVICEBASE_EXPORT BaseDeviceSet : public Device::DeviceSet
    {
    public:
        /**
         * @brief Creates an empty device cache.
         */
        BaseDeviceSet() = default;
        /**
         * @brief Prevents copying an object responsible for device identity or owned resources.
         */
        BaseDeviceSet(const BaseDeviceSet &) = delete;
        /**
         * @brief Prevents copy assignment from duplicating device identity or resource ownership.
         */
        BaseDeviceSet &operator=(const BaseDeviceSet &) = delete;
        /**
         * @brief Prevents moving an object whose identity or cached pointers must remain stable.
         */
        BaseDeviceSet(BaseDeviceSet &&) = delete;
        /**
         * @brief Prevents move assignment from replacing stable device identity or cached pointers.
         */
        BaseDeviceSet &operator=(BaseDeviceSet &&) = delete;
        /**
         * @brief Deletes every cached device through its virtual destructor.
         */
        virtual ~BaseDeviceSet();

        /**
         * @brief Reuses a cached device or creates and caches one on first lookup.
         * @param[in] deviceId The identifier used as the cache key.
         * @return A borrowed pointer valid until set destruction, or nullptr if creation fails.
         */
        Device::Device *device(Device::DEVICE_ID deviceId) override;
    protected:
        /**
         * @brief Allocates a generic device; derived sets may override the device type.
         * @param[in] deviceId The identifier assigned to the device.
         * @return A new device for ownership by the cache, or nullptr if allocation fails.
         */
        Device::Device *createDevice(Device::DEVICE_ID deviceId) override;
    private:
        /**
         * @brief Deletes cached device objects during destruction.
         * @note Cache entries are not erased; this method must not be used for repeated cleanup.
         */
        void cleanup();

        /**
         * @brief Owns one device per identifier; lookup returns borrowed pointers to these objects.
         */
        std::map<Device::DEVICE_ID, Device::Device *> m_devices;
    };
}

#endif // BASEDEVICESET_H

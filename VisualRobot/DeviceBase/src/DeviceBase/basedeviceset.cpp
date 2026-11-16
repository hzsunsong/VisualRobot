/**
 * @file basedeviceset.cpp
 * @brief On-demand device creation, cache lookup, and device destruction.
 */

#include <algorithm>
#include "basedeviceset.h"
#include "basedevice.h"

DeviceBase::BaseDeviceSet::~BaseDeviceSet()
{
    cleanup();
}

Device::Device *DeviceBase::BaseDeviceSet::device(Device::DEVICE_ID deviceId)
{
    const auto itor{m_devices.find(deviceId)};

    if (m_devices.cend() != itor) return itor->second;

    /**
     * @brief Creates through the virtual factory so derived sets can supply their own device types.
     */
    const auto device{createDevice(deviceId)};

    CHECK_PTR_RETURN_VALUE(device, {})

    m_devices.emplace(deviceId, device);

    return device;
}

Device::Device *DeviceBase::BaseDeviceSet::createDevice(Device::DEVICE_ID deviceId)
{
    return new(std::nothrow) BaseDevice{deviceId};
}

/**
 * @brief Deletes each cached device during destruction; entries are not erased for reuse.
 */
void DeviceBase::BaseDeviceSet::cleanup()
{
    std::for_each(m_devices.cbegin(), m_devices.cend(), [](const auto &itor){
        auto device{itor.second};

        CHECK_PTR_RETURN(device)

        DELETE(device);
    });
}

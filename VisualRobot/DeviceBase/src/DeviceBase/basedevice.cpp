/**
 * @file basedevice.cpp
 * @brief Generic device state transitions and placeholder communication operations.
 */

#include "Device/device_def.h"
#include "basedevice.h"

DeviceBase::BaseDevice::BaseDevice(::Device::DEVICE_ID id)
    :m_id{id}
{

}

std::string DeviceBase::BaseDevice::name() const
{
    return "BaseDevice";
}

::Device::DEVICE_ID DeviceBase::BaseDevice::id() const
{
    return m_id;
}

bool DeviceBase::BaseDevice::isOpen() const
{
    return m_opened;
}

int DeviceBase::BaseDevice::open(const ::Device::Endpoint &endpoint)
{
    m_opened = true;

    return static_cast<int>(::Device::ErrorCode::NoError);
}

int DeviceBase::BaseDevice::request(const cxx::byte_array &data, const std::function<void (const cxx::byte_array &)> &cb)
{
    return static_cast<int>(::Device::ErrorCode::NoError);
}

int DeviceBase::BaseDevice::close()
{
    m_opened = false;

    return static_cast<int>(::Device::ErrorCode::NoError);
}
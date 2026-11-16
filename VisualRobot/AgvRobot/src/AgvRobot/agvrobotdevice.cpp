/**
 * @file agvrobotdevice.cpp
 * @brief AGV endpoint validation and placeholder device operations.
 */

#include "cxx/logger.h"
#include "agvrobotdevice.h"

AgvRobot::AgvRobotDevice::AgvRobotDevice(::Device::DEVICE_ID id)
    :DeviceBase::BaseDevice{id}
{

}

std::string AgvRobot::AgvRobotDevice::name() const
{
    return "AgvRobotDevice";
}

int AgvRobot::AgvRobotDevice::open(const ::Device::Endpoint &endpoint)
{
    cxx::logger::info("open the agv robot device.");
    const auto networkEndpoint{dynamic_cast<::Device::NetworkEndpoint *>(const_cast<::Device::Endpoint *>(&endpoint))};

    CHECK_PTR_RETURN_VALUE(networkEndpoint, static_cast<int>(::Device::ErrorCode::EndpointTypeError))

    cxx::logger::info("ipv4Addr:{}, port:{}", networkEndpoint->ipv4Addr, networkEndpoint->port);

    /**
     * @brief Reports endpoint validation success without opening a transport or updating BaseDevice state.
     */
    return static_cast<int>(::Device::ErrorCode::NoError);
}

int AgvRobot::AgvRobotDevice::request(const cxx::byte_array &data, const std::function<void (const cxx::byte_array &)> &cb)
{
    return static_cast<int>(::Device::ErrorCode::NoError);
}

int AgvRobot::AgvRobotDevice::close()
{
    return static_cast<int>(::Device::ErrorCode::NoError);
}
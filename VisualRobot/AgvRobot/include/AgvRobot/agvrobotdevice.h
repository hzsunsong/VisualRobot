/**
 * @file agvrobotdevice.h
 * @brief AGV device stub and network endpoint validation.
 */

#ifndef AGVROBOTDEVICE_H
#define AGVROBOTDEVICE_H
#include "cxx/cxx_def.h"
#include "DeviceBase/basedevice.h"

namespace AgvRobot
{
    /**
     * @brief Provides the current AGV device stub.
     * @note Network endpoints are validated and logged, but no transport is opened.
     * The overrides do not update the open state stored by BaseDevice.
     */
    class AgvRobotDevice : public DeviceBase::BaseDevice
    {
    public:
        /**
         * @brief Creates a device with an identifier managed by the base class.
         * @param[in] id The identifier used within the owning device set.
         */
        AgvRobotDevice(::Device::DEVICE_ID id);
        /**
         * @brief Prevents copying an object responsible for device identity or owned resources.
         */
        AgvRobotDevice(const AgvRobotDevice &) = delete;
        /**
         * @brief Prevents copy assignment from duplicating device identity or resource ownership.
         */
        AgvRobotDevice &operator=(const AgvRobotDevice &) = delete;
        /**
         * @brief Prevents moving an object whose identity or cached pointers must remain stable.
         */
        AgvRobotDevice(AgvRobotDevice &&) = delete;
        /**
         * @brief Prevents move assignment from replacing stable device identity or cached pointers.
         */
        AgvRobotDevice &operator=(AgvRobotDevice &&) = delete;
        /**
         * @brief Allows destruction through a base pointer; no explicit close() call is performed.
         */
        virtual ~AgvRobotDevice() = default;

        /**
         * @brief Gets the AGV device type name.
         * @return The literal "AgvRobotDevice".
         */
        std::string name() const override;

        /**
         * @brief Validates and logs a network endpoint without establishing a connection.
         * @param[in] endpoint The endpoint, which must be a Device::NetworkEndpoint.
         * @return EndpointTypeError for an incompatible endpoint; otherwise NoError, as an integer.
         */
        int open(const ::Device::Endpoint &endpoint) override;

        /**
         * @brief Accepts a request without sending data or invoking a callback.
         * @param[in] data The request payload, currently unused.
         * @param[in] cb The optional response callback, currently unused.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int request(const cxx::byte_array &data,
                    const std::function<void(const cxx::byte_array &)> &cb = nullptr) override;

        /**
         * @brief Returns success without releasing transport resources or changing open state.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int close() override;
    };
}

#endif // AGVROBOTDEVICE_H

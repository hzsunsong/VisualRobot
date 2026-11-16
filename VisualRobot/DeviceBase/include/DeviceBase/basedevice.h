/**
 * @file basedevice.h
 * @brief Generic device identifier storage and in-memory open state.
 */

#ifndef BASEDEVICE_H
#define BASEDEVICE_H
#include "cxx/cxx_def.h"
#include "Device/device.h"
#include "Device/endpoint.h"
#include "devicebase_export.h"

namespace DeviceBase
{
    /**
     * @brief Provides identifier storage and an in-memory open state without transport I/O.
     */
    class DEVICEBASE_EXPORT BaseDevice : public Device::Device
    {
    public:
        /**
         * @brief Creates a closed device with a stable identifier.
         * @param[in] id The identifier used by the owning device set.
         */
        BaseDevice(::Device::DEVICE_ID id);
        /**
         * @brief Prevents copying an object responsible for device identity or owned resources.
         */
        BaseDevice(const BaseDevice &) = delete;
        /**
         * @brief Prevents copy assignment from duplicating device identity or resource ownership.
         */
        BaseDevice &operator=(const BaseDevice &) = delete;
        /**
         * @brief Prevents moving an object whose identity or cached pointers must remain stable.
         */
        BaseDevice(BaseDevice &&) = delete;
        /**
         * @brief Prevents move assignment from replacing stable device identity or cached pointers.
         */
        BaseDevice &operator=(BaseDevice &&) = delete;
        /**
         * @brief Allows destruction through a base pointer; no explicit close() call is performed.
         */
        virtual ~BaseDevice() = default;

        /**
         * @brief Gets the generic device type name.
         * @return The literal "BaseDevice".
         */
        std::string name() const override;

        /**
         * @brief Gets the identifier supplied at construction.
         * @return The stored device identifier.
         */
        ::Device::DEVICE_ID id() const override;

        /**
         * @brief Gets the state maintained by the base open() and close() implementations.
         * @return True after base open() and false after base close().
         */
        bool isOpen() const override;

        /**
         * @brief Marks the device open without creating a transport connection.
         * @param[in] endpoint The connection settings, currently unused.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int open(const ::Device::Endpoint &endpoint) override;

        /**
         * @brief Returns success without sending a request or invoking a response callback.
         * @param[in] data The payload, currently unused.
         * @param[in] cb The optional callback, currently unused.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int request(const cxx::byte_array &data,
                    const std::function<void(const cxx::byte_array &)> &cb = nullptr) override;

        /**
         * @brief Accepts a receive callback without storing or invoking it.
         * @param[in] cb The callback, currently unused.
         */
        void setReceiver(const std::function<void(const cxx::byte_array &)> &cb) override {}

        /**
         * @brief Marks the device closed without performing transport cleanup.
         * @return The integer value of Device::ErrorCode::NoError.
         */
        int close() override;
    private:
        ::Device::DEVICE_ID m_id{};
        bool m_opened{false};
    };
}

#endif // BASEDEVICE_H

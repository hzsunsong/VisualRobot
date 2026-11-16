/**
 * @file device.h
 * @brief Abstract device lifecycle and byte-oriented communication API.
 */

#ifndef DEVICE_H
#define DEVICE_H
#include "cxx/cxx_def.h"
#include "device_def.h"

namespace Device
{
    struct Endpoint;

    /**
     * @brief Defines the device lifecycle, request, and receive interfaces.
     * @details Integer status codes use ErrorCode, with zero indicating success.
     * Transport behavior and callback invocation are supplied by each implementation.
     */
    class Device
    {
    public:
        /**
         * @brief Defaults construction and virtual destruction and disables copying and moving.
         */
        DELETE_CLASS_DEFAULT_OPERATION_V(Device);

        /**
         * @brief Gets the device type name.
         * @return The implementation-defined name.
         */
        virtual std::string name() const = 0;

        /**
         * @brief Gets the identifier used to look up this device in its device set.
         * @return The device identifier.
         */
        virtual DEVICE_ID id() const = 0;

        /**
         * @brief Queries the open state reported by the implementation.
         * @return True when the implementation considers the device open.
         */
        virtual bool isOpen() const = 0;

        /**
         * @brief Opens the device using a supported endpoint.
         * @param[in] endpoint The transport-specific connection settings.
         * @return Zero on success, or an implementation-defined error code.
         */
        virtual int open(const Endpoint &endpoint) = 0;

        /**
         * @brief Submits a request and optionally supplies a response callback.
         * @param[in] data The payload to send.
         * @param[in] cb The optional callback receiving response bytes.
         * @return Zero on success, or an implementation-defined error code.
         * @note Callback timing and support depend on the implementation.
         */
        virtual int request(const cxx::byte_array &data,
                            const std::function<void(const cxx::byte_array &)> &cb = nullptr) = 0;

        /**
         * @brief Sets the callback used by an implementation to deliver received bytes.
         * @param[in] cb The receive callback; handling an empty callback is implementation-defined.
         */
        virtual void setReceiver(const std::function<void(const cxx::byte_array &)> &cb) = 0;

        /**
         * @brief Closes the device and releases its connection resources.
         * @return Zero on success, or an implementation-defined error code.
         */
        virtual int close() = 0;
    };
}

#endif // DEVICE_H

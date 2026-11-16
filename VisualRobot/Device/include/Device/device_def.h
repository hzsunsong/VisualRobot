/**
 * @file device_def.h
 * @brief Device identifiers, plugin versions, operation status, and transport selection.
 */

#ifndef DEVICE_DEF_H
#define DEVICE_DEF_H

namespace Device
{
    /**
     * @brief Identifies a device within a plugin-specific device set.
     */
    using DEVICE_ID = int;

    /**
     * @brief Defines the current Pluma device provider version.
     */
    static constexpr auto DEVICE_PLUGIN_VERSION{1};
    /**
     * @brief Defines the oldest compatible Pluma device provider version.
     */
    static constexpr auto DEVICE_PLUGIN_LOWEST_VERSION{0};

    /**
     * @brief Defines status values returned by device operations.
     */
    enum class ErrorCode
    {
        NoError = 0, ///< The operation reports success.
        EndpointTypeError ///< The endpoint does not match the required transport.
    };

    /**
     * @brief Selects a transport when constructing device endpoints.
     * @note DevicePluginSet currently constructs endpoints only for Network.
     */
    enum class ConnectType
    {
        Network, ///< Use an IPv4 address and port.
        Serialport, ///< Reserved for serial-port connections.
        Usb ///< Reserved for USB connections.
    };
}

#endif // DEVICE_DEF_H

/**
 * @file endpoint.h
 * @brief Polymorphic endpoint settings and the IPv4 network endpoint.
 */

#ifndef ENDPOINT_H
#define ENDPOINT_H
#include "cxx/cxx_def.h"

namespace Device
{
    /**
     * @brief Provides a polymorphic base for transport-specific endpoint settings.
     */
    struct Endpoint
    {
        /**
         * @brief Allows endpoint value operations and destruction through the base type.
         */
        CLASS_DEFAULT_OPERATION_V(Endpoint);
    };

    /**
     * @brief Stores IPv4 connection settings for a network device.
     */
    struct NetworkEndpoint : public Endpoint
    {
        /**
         * @brief Copies network settings without validating the address or port.
         * @param[in] ipv4Addr The textual IPv4 address.
         * @param[in] port The destination port.
         */
        NetworkEndpoint(const std::string &ipv4Addr, uint16_t port)
            :ipv4Addr{ipv4Addr},
            port{port}
        {

        }

        /**
         * @brief Keeps the supplied address text unchanged for interpretation by the device implementation.
         */
        std::string ipv4Addr;
        /**
         * @brief Stores the destination TCP/UDP port value without validating service availability.
         */
        uint16_t port;
    };
}

#endif // ENDPOINT_H

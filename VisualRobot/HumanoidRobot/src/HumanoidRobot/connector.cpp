/**
 * @file connector.cpp
 * @brief Pluma provider registration entry point for the humanoid plugin.
 */

#include "Pluma/Connector.hpp"
#include "cxx/cxx_def.h"
#include "humanoidrobot.h"

/**
 * @brief Registers this library's device plugin provider with the Pluma host.
 * @param[in,out] host The host receiving the provider.
 * @return False if provider allocation or host registration fails; otherwise true.
 */
PLUMA_CONNECTOR
bool connect(pluma::Host &host)
{
    const auto provider{new(std::nothrow) HumanoidRobot::HumanoidRobotProvider};

    CHECK_PTR_RETURN_FALSE(provider)

    return host.add(provider);
}

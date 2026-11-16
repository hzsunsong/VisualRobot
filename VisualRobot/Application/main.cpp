/**
 * @file main.cpp
 * @brief Logging initialization and the Qt Quick application entry point.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "cxx/cxx_def.h"
#include "cxx/logger.h"
#include "application.h"

namespace
{
    static constexpr auto APP_NAME{"VisualRobot"};
}

/**
 * @brief Initializes logging, exposes the application API, and starts the QML event loop.
 * @param[in] argc The number of process arguments.
 * @param[in] argv The process argument array.
 * @return The Qt event loop exit code, or EXIT_FAILURE if application allocation fails.
 */
int main(int argc, char *argv[])
{
    cxx::logger logger_t;
    logger_t.initial(APP_NAME);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const std::unique_ptr<Application::Application>
        application{new(std::nothrow) Application::Application{&engine}};

    CHECK_PTR_RETURN_VALUE(application, EXIT_FAILURE)

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Application", "Main");

    return QGuiApplication::exec();
}

//
// Created by vogje01 on 1/18/26.
//

#ifndef AWSFORWARDER_LOGGING_UTILS_H
#define AWSFORWARDER_LOGGING_UTILS_H

#include <QObject>
#include <QDateTime>

#include <utils/EventBus.h>

class LoggingUtils : QObject {
    Q_OBJECT

public:
    static void trace(const QString &message, const QString &file, const int line) {
        emit EventBus::instance().LoggingSignal("TRACE", file, line, message, QDateTime::currentDateTime());
    }

    static void debug(const QString &message, const QString &file, const int line) {
        emit EventBus::instance().LoggingSignal("DEBUG", file, line, message, QDateTime::currentDateTime());
    }

    static void info(const QString &message, const QString &file, const int line) {
        emit EventBus::instance().LoggingSignal("INFO", file, line, message, QDateTime::currentDateTime());
    }

    static void warning(const QString &message, const QString &file, const int line) {
        emit EventBus::instance().LoggingSignal("WARN", file, line, message, QDateTime::currentDateTime());
    }

    static void error(const QString &message, const QString &file, const int line) {
        emit EventBus::instance().LoggingSignal("ERROR", file, line, message, QDateTime::currentDateTime());
    }
};

#define log_trace(message) LoggingUtils::trace(message, __FILE__, __LINE__);
#define log_debug(message) LoggingUtils::debug(message, __FILE__, __LINE__);
#define log_info(message) LoggingUtils::info(message, __FILE__, __LINE__);
#define log_warning(message) LoggingUtils::warning(message, __FILE__, __LINE__);
#define log_error(message) LoggingUtils::error(message, __FILE__, __LINE__);

#endif //AWSFORWARDER_LOGGING_UTILS_H

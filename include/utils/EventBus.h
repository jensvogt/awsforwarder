//
// Created by vogje01 on 11/19/25.
//

#ifndef AWSFORWARDER_QT_UI_EVENT_BUS_H
#define AWSFORWARDER_QT_UI_EVENT_BUS_H

#include <QObject>

class EventBus final : public QObject {
    Q_OBJECT

public:
    static EventBus &instance() {
        static EventBus b;
        return b;
    }

signals:
    void LoggingSignal(const QString &level, const QString &file, int line, const QString &message, const QDateTime &timeStamp);

    void ShutdownSignal();
};

#endif // AWSFORWARDER_QT_UI_EVENT_BUS_H

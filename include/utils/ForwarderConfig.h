//
// Created by vogje01 on 1/18/26.
//

#ifndef AWSFORWARDER_FORWARDER_CONFIG_H
#define AWSFORWARDER_FORWARDER_CONFIG_H

#include <QString>

struct ForwarderConfig {

    QString name;

    QString nameSpace;

    QString password;

    QString username;

    QString type;

    QString cluster;

    QString secret;

    QString awsAccount;

    bool active;

    int localPort;

    int remotePort;

    int clusterPort;

    QDateTime lastStarted;

    QDateTime expired;
};
#endif //AWSFORWARDER_FORWARDER_CONFIG_H

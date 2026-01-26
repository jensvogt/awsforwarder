//
// Created by vogje01 on 1/21/26.
//

#ifndef AWSFORWARDER_SESSION_WORKER_H
#define AWSFORWARDER_SESSION_WORKER_H

// C++ includes
#include <utility>

// Qt includes
#include <QObject>
#include <QThread>

// AWS SDK includes
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/secretsmanager/SecretsManagerClient.h>
#include <aws/secretsmanager/model/GetSecretValueRequest.h>

// Forwarder includes
#include <utils/LoggingUtils.h>
#include <utils/AwsUtils.h>
#include <kubernetes/KubernetesWorker.h>
#include <secrets/SecretsWorker.h>

class SessionWorker : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param nameSpace AWS name space
     */
    explicit SessionWorker(QString nameSpace);

    ~SessionWorker() override;

public slots:
    /**
     * @brief Get AWS session
     */
    void DoWork(const QString &awsAccount, const QString &nameSpace);

signals:
    void finished() const;

private:
    /**
     * @brief Namespace
     */
    QString _nameSpace;

    /**
     * @brief Namespace
     */
    QString _awsAccount;

    /**
     * @brief AWS utilities
     */
    AwsUtils *_awsUtils;

    /**
     * @brief Kubernetes utilities
     */
    SecretsWorker *_secretsWorker;

    /**
     * @brief Kubernetes utilities
     */
    KubernetesWorker *_kubernetesWorker;

    /**
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief Rest manager
     */
    RestManager *_restManager;
};

#endif // AWSFORWARDER_SESSION_WORKER_H

//
// Created by vogje01 on 1/21/26.
//

#ifndef AWSFORWARDER_SECRETS_WORKER_H
#define AWSFORWARDER_SECRETS_WORKER_H

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

struct SecretRecord {

    QString username;
    QString password;

    void FromJson(const QJsonObject &jsonObject) {
        username = jsonObject["username"].toString();
        password = jsonObject["password"].toString();
    }
};

class SecretsWorker : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param nameSpace AWS name space
     */
    explicit SecretsWorker(QString nameSpace);

public slots:
    /**
     * @brief Synchronize secrets
     */
    //void DoWork();

    void Shutdown() const;

    void GetPasswords(const QString &awsAccount, const QString &nameSpace, const Aws::STS::Model::Credentials &credentials) const;

signals:
    void finished();

private:
    /**
     * @nrief Get the forwarder password from AWS
     *
     * @param smClient AWS SSM client
     * @param secretId secrets ID
     * @return password for forwarder
     */
    static QString GetSecretFromAws(const Aws::SecretsManager::SecretsManagerClient &smClient, const QString &secretId);

    static void WriteWorkbenchConfig();

    /**
     * @brief Extract the username from the secrets JSON string
     *
     * @param secret secret string
     * @return username
     */
    static QString GetUsername(const QString &secret);

    /**
     * @brief Extract the password from the secrets JSON string
     *
     * @param secret secret string
     * @return password
     */
    static QString GetPassword(const QString &secret);

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
    KubernetesWorker *_kubernetesUtils;

    /**
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief Rest manager
     */
    RestManager *_restManager;
};

#endif //AWSFORWARDER_SECRETS_WORKER_H

//
// Created by vogje01 on 1/24/26.
//

#ifndef AWSFORWARDER_KUBERNETES_WORKER_H
#define AWSFORWARDER_KUBERNETES_WORKER_H

// Qt includes
#include <QObject>
#include <QTemporaryFile>

// Forwarder config
#include <utils/AwsUtils.h>

class KubernetesWorker : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    explicit KubernetesWorker();

public slots:
    /**
     * @brief Threads main loop
     */
    void DoWork(const QString &awsAccount, const Aws::STS::Model::Credentials &credentials) const;

    /**
     * Shutdown worker
     */
    void Shutdown() const;

private:
    void CheckPods(const QString &awsAccount) const;

    static QStringList GetPortList(const QString &nameSpace);

    void StartForwarderPod(const QString &awsAccount, const QString &nameSpace) const;

    /**
     * @brief Namespace
     */
    QString _nameSpace;

    /**
     * @brief Namespace
     */
    QString _awsAccount;

    /**
     * @brief Kubernetes utilities
     */
    KubernetesUtils *_kubernetesUtils;

    /**
     * @brief Kubernetes forwarder POD suffix
     */
    QString _suffix;

    /**
     * @brief Rest manager
     */
    RestManager *_restManager;
};

#endif //AWSFORWARDER_KUBERNETES_WORKER_H

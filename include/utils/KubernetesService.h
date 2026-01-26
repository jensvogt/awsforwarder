//
// Created by vogje01 on 1/24/26.
//

#ifndef AWSFORWARDER_KUBERNETES_SERVICE_H
#define AWSFORWARDER_KUBERNETES_SERVICE_H

// Qt includes
#include <QElapsedTimer>

// Forwarder includes
#include <utils/BaseService.h>
#include "utils/LoggingUtils.h"

class KubernetesService : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief OneLogin service constructor
     */
    explicit KubernetesService(RestManager *restManager, QObject *parent = nullptr) : _restManager(restManager) {
    }

    void GetPodList(const QString &nameSpace, const QString &endpoint, const QMap<QString, QString> &headers, const QSslConfiguration &sslConfiguration) const;

private:
    RestManager *_restManager;
};
#endif //AWSFORWARDER_KUBERNETES_SERVICE_H

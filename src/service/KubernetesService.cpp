//
// Created by vogje01 on 1/24/26.
//

#include <service/KubernetesService.h>

void KubernetesService::GetPodList(const QString &nameSpace, const QString &endpoint, const QMap<QString, QString> &headers, const QSslConfiguration &sslConfiguration) const {

    QElapsedTimer timer;
    timer.start();
    const QUrl url(QString(endpoint + "/api/v1/namespaces/%1/pods").arg(nameSpace));

    _restManager->get(url,
                      headers,
                      sslConfiguration,
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  //                                   emit GetAccessTokenSignal(oneloginResponse);
                              } else {
                                  log_error("GetPodList response is not an object");
                              }
                          } else {
                              log_error("GetPodList failed, error: " + error);
                          }
                      });
}

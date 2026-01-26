#include <utils/OneLoginService.h>

void OneLoginService::GetAccessToken(const QString &host, const QString &path, const QString &body, const QMap<QString, QString> &headers) {
    QElapsedTimer timer;
    timer.start();

    const QString url = "https://" + host + path;

    _restManager->post(url,
                       body.toUtf8(),
                       headers,
                       [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                           if (success) {
                               if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                   OneLoginApiTokenResponse oneloginResponse;
                                   oneloginResponse.FromJsonObject(jsonDoc.object());
                                   log_debug("GetAccessToken succeeded");
                                   emit GetAccessTokenSignal(oneloginResponse);
                               } else {
                                   log_error("GetAccessToken response is not an object");
                               }
                           } else {
                               log_error("GetAccessToken failed, error: " + error);
                           }
                       });
}

void OneLoginService::GetSamlResponse(const QString &host, const QString &path, const QString &body, const QMap<QString, QString> &headers) {
    QElapsedTimer timer;
    timer.start();

    const QString url = "https://" + host + path;

    _restManager->post(url,
                       body.toUtf8(),
                       headers,
                       [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                           if (success) {
                               if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                   SamlAssertionResponse samlAssertionResponse;
                                   samlAssertionResponse.FromJsonObject(jsonDoc.object());
                                   log_debug("GetSamlResponse succeeded");
                                   emit GetSamlAssertionSignal(samlAssertionResponse);
                               } else {
                                   log_error("GetAccessToken response is not an object");
                               }
                           } else {
                               log_error("GetAccessToken failed, error: " + error);
                           }
                       });
}

void OneLoginService::GetVerifyResponse(const QString &host, const QString &path, const QString &body, const QMap<QString, QString> &headers) {
    QElapsedTimer timer;
    timer.start();

    const QString url = "https://" + host + path;

    _restManager->post(url,
                       body.toUtf8(),
                       headers,
                       [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                           if (success) {
                               if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                   SamlVerifyResponse samlVerifyResponse;
                                   samlVerifyResponse.FromJsonObject(jsonDoc.object());
                                   log_debug("GetVerifyResponse succeeded");
                                   emit GetSamlVerifySignal(samlVerifyResponse);
                               } else {
                                   log_error("GetVerifyResponse response is not an object");
                               }
                           } else {
                               log_error("GetVerifyResponse failed, error: " + error + ", response: " + response);
                           }
                       });
}

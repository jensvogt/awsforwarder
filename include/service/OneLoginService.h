#ifndef AWSFORWARDER_ONE_LOGIN_SERVICE_H
#define AWSFORWARDER_ONE_LOGIN_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QObject>
#include <QUrlQuery>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <utils/EventBus.h>
#include <utils/LoggingUtils.h>
#include <dto/onelogin/OneLoginApiTokenResponse.h>
#include <dto/onelogin/SamlAssertionResponse.h>
#include <dto/onelogin/SamlVerifyResponse.h>
#include <dto/onelogin/SamlAssertionResponse.h>
#include <dto/onelogin/SamlVerifyResponse.h>

class OneLoginService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief OneLogin service constructor
     */
    explicit OneLoginService(RestManager *restManager, QObject *parent = nullptr) : _restManager(restManager) {
    }

    /**
     * @brief Get the AWS access token
     *
     * @param host host name
     * @param path path for REST call
     * @param body message body
     * @param headers message headers
     */
    void GetAccessToken(const QString &host, const QString &path, const QString &body, const QMap<QString, QString> &headers);

    /**
     * @brief Get the SAML response
     *
     * @param host host name
     * @param path path for REST call
     * @param body message body
     * @param headers message headers
     */
    void GetSamlResponse(const QString &host, const QString &path, const QString &body, const QMap<QString, QString> &headers);

    /**
     * @brief Verify MFA key response
     *
     * @param host host name
     * @param path path for REST call
     * @param body message body
     * @param headers message headers
     */
    void GetVerifyResponse(const QString &host, const QString &path, const QString &body, const QMap<QString, QString> &headers);

signals:
    /**
     * @brief Signaled when an access token is available
     *
     * @param accessTokenResponse access token response
     */
    void GetAccessTokenSignal(const OneLoginApiTokenResponse &accessTokenResponse);

    /**
     * @brief Signaled when SAML assertion response is available
     *
     * @param samlAssertionResponse SAML assertion response
     */
    void GetSamlAssertionSignal(const SamlAssertionResponse &samlAssertionResponse);

    /**
     * @brief Signaled when SAML verification response is available
     *
     * @param samlVerifyResponse SAML verification response
     */
    void GetSamlVerifySignal(const SamlVerifyResponse &samlVerifyResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager *_restManager;
};


#endif // AWSFORWARDER_ONE_LOGIN_SERVICE_H

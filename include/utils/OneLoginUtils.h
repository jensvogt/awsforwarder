//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_ONELOGIN_UTILS_H
#define AWSFORWARDER_ONELOGIN_UTILS_H

// C++ standard includes
#include <string>

// AwsLogin includes
#include <dto/onelogin/OneLoginApiTokenRequest.h>
#include <dto/onelogin/OneLoginApiTokenResponse.h>
#include <dto/onelogin/SamlAssertionRequest.h>
#include <dto/onelogin/SamlAssertionResponse.h>
#include <dto/onelogin/SamlVerifyRequest.h>
#include <dto/onelogin/SamlVerifyResponse.h>
#include <utils/Configuration.h>
//#include <utils/HttpUtils.h>
#include <utils/Otp.h>
#include <utils/OtpUtils.h>
#include <service/OneLoginService.h>

using namespace std::chrono;

struct OneLoginResult {

    /**
     * @brief OneLogin token
     */
    QString token{};

    /**
     * @brief OneLogin token
     */
    system_clock::time_point expiresAt = system_clock::now();
};

class OneLoginUtils : public QObject {
    Q_OBJECT

public:
    /**
     * Constructor
     */
    OneLoginUtils(RestManager *restManager, QObject *parent);

    /**
     * @brief Sets the AWS access token
     *
     * @param awsAccount AWS account, either pim-int or pim-prod
     */
    void GetAccessToken(const QString &awsAccount);

    void GetSamlAssertion(const QString &host, const QString &awsAccount, const OneLoginApiTokenResponse &accessTokenResponse);

signals:
    /**
     * @brief Signaled when an access token is available
     *
     * @param awsAccount AWS account, either pim-int or pim-prod
     * @param accessToken AWS access token
     */
    void GetAccessTokenSignal(const QString &awsAccount, const QString &accessToken);

private:
    /**
     * @brief Returns the OneLogin URL
     *
     * @returns Onelogin URL
     */
    static QString GetOneLoginUrl(const QString &subDomain);

    /**
     * @brief Returns the OneLogin URL
     *
     * @returns Onelogin URL
     */
    static QString GetOneLoginHost(const QString &subDomain);

    /**
     * @brief Current access tokens
     */
    QMap<QString, QString> _currentAccessTokens;

    OneLoginApiTokenResponse _currentTokenResponse{};

    SamlVerifyResponse _currentVerifyResponse{};

    /**
     * @brief Expiration timestamp
     */
    system_clock::time_point _expiredAt = system_clock::now();

    /**
     * @brief Onelogin URL
     */
    QString _oneLoginUrl;

    /**
     * @brief Onelogin client ID
     */
    QString _clientId;

    /**
     * @brief Onelogin client secret
     */
    QString _clientSecret;

    /**
     * @brief Onelogin user
     */
    QString _user;

    /**
     * @brief Onelogin sub domain
     */
    QString _subDomain;

    /**
     * @brief Onelogin password
     */
    QString _password;

    /**
     * @brief Onelogin client secret
     */
    QMap<QString, long> _appIds;

    /**
     * @brief OTP key
     */
    QString _otpKey;

    /**
     * @brief OneLogin REST service
     */
    OneLoginService *_oneloginService;

    QObject *_parent;

    RestManager *_restManager;
};

#endif// AWSFORWARDER_ONELOGIN_UTILS_H

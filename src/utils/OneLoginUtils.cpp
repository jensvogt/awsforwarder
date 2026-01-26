//
// Created by vogje01 on 08/03/2025.
//

#include <utils/OneLoginUtils.h>

OneLoginUtils::OneLoginUtils(RestManager *restManager, QObject *parent) : _parent(parent), _restManager(restManager) {

    _appIds["pim-int"] = Configuration::instance().GetValue<long>("onelogin.app-id.int");
    _appIds["pim-prod"] = Configuration::instance().GetValue<long>("onelogin.app-id.prod");
    _user = Configuration::instance().GetValue<QString>("onelogin.user");
    _password = Configuration::instance().GetValue<QString>("onelogin.password");
    _clientId = Configuration::instance().GetValue<QString>("onelogin.clientId");
    _clientSecret = Configuration::instance().GetValue<QString>("onelogin.clientSecret");
    _subDomain = Configuration::instance().GetValue<QString>("onelogin.subDomain");

    // OTP key
    _otpKey = Configuration::instance().GetValue<QString>("otp.key");

    _oneLoginUrl = GetOneLoginUrl(_subDomain);
    _oneloginService = new OneLoginService(restManager);
}

void OneLoginUtils::GetAccessToken(const QString &awsAccount) {

    if (_currentTokenResponse.expiresIn == 0 || QDateTime::currentDateTime() > _currentTokenResponse.expiresAt) {
        QMap<QString, QString> headers;
        headers["Authorization"] = "client_id:" + _clientId + ",client_secret:" + _clientSecret;
        headers["Content-Type"] = "application/json";

        OneLoginApiTokenRequest request;
        request.grantType = "client_credentials";

        const QString host = GetOneLoginHost(_subDomain);
        _oneloginService->GetAccessToken(host, "/auth/oauth2/v2/token", request.ToJson(), headers);

        connect(_oneloginService, &OneLoginService::GetAccessTokenSignal, this, [this, awsAccount, host](const OneLoginApiTokenResponse &accessTokenResponse) {
            log_debug("Got OneLogin access, account: " + awsAccount + ", token: " + accessTokenResponse.accessToken);
            _currentTokenResponse = accessTokenResponse;
            Configuration::instance().SetLastStarted(awsAccount, accessTokenResponse.createdAt);
            Configuration::instance().SetExpires(awsAccount, accessTokenResponse.expiresAt);
            GetSamlAssertion(host, awsAccount, accessTokenResponse);
        });
    } else {
        emit GetAccessTokenSignal(awsAccount, _currentAccessTokens[awsAccount]);
    }
}

void OneLoginUtils::GetSamlAssertion(const QString &host, const QString &awsAccount, const OneLoginApiTokenResponse &accessTokenResponse) {
    SamlAssertionRequest samlRequest;
    samlRequest.userIdOrEmail = _user;
    samlRequest.password = _password;
    samlRequest.appId = this->_appIds[awsAccount];
    samlRequest.subDomain = _subDomain;

    QMap<QString, QString> samlHeaders;
    samlHeaders["Authorization"] = "bearer:" + accessTokenResponse.accessToken;
    samlHeaders["Content-Type"] = "application/json";
    _oneloginService->GetSamlResponse(host, "/api/2/saml_assertion", samlRequest.ToJson(), samlHeaders);
    connect(_oneloginService, &OneLoginService::GetSamlAssertionSignal, this, [this, awsAccount, accessTokenResponse, host](const SamlAssertionResponse &samlResponse) {
                log_debug("Got SAML assertion, account: " + awsAccount + ", message: " + samlResponse.message);

                if (samlResponse.message.contains("MFA")) {

                    // MFA verification request
                    SamlVerifyRequest verifyRequest;
                    verifyRequest.appId = _appIds[awsAccount];
                    verifyRequest.deviceId = samlResponse.devices[1].deviceId;
                    verifyRequest.otpToken = OtpUtils::instance().WaitForNextToken();
                    verifyRequest.stateToken = samlResponse.stateToken;

                    QMap<QString, QString> verifyHeaders;
                    verifyHeaders["Authorization"] = "bearer:" + accessTokenResponse.accessToken;
                    verifyHeaders["Content-Type"] = "application/json";

                    _oneloginService->GetVerifyResponse(host, "/api/2/saml_assertion/verify_factor", verifyRequest.ToJson(), verifyHeaders);
                    connect(_oneloginService, &OneLoginService::GetSamlVerifySignal, this, [this, awsAccount](const SamlVerifyResponse &samlVerifyResponse) {
                        log_debug("Got SAML verification, account: " + awsAccount + ", message: " + samlVerifyResponse.message);
                        _currentAccessTokens[awsAccount] = samlVerifyResponse.data;
                        emit GetAccessTokenSignal(awsAccount, samlVerifyResponse.data);
                    });
                }
            }
    );
}

QString OneLoginUtils::GetOneLoginUrl(const QString &subDomain) {
    return "https://" + subDomain + ".onelogin.com/auth/oauth2/v2/token";
}

QString OneLoginUtils::GetOneLoginHost(const QString &subDomain) {
    return subDomain + ".onelogin.com";
}

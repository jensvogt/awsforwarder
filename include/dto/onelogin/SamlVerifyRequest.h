//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_DTO_SAML_VERIFY_REQUEST_H
#define AWSFORWARDER_DTO_SAML_VERIFY_REQUEST_H

struct SamlVerifyRequest {

    /**
     * @Brief Application ID
     */
    long appId;

    /**
     * @brief OTP token
     */
    uint64_t otpToken;

    /**
     * @brief Device ID
     */
    int deviceId;

    /**
     * @brief State token
     */
    QString stateToken;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject.insert("app_id", QString::number(appId));
        jsonObject.insert("otp_token", QString::number(otpToken));
        jsonObject.insert("device_id", QString::number(deviceId));
        jsonObject.insert("state_token", stateToken);
        return QJsonDocument(jsonObject).toJson();
    }
};

#endif// AWSFORWARDER_DTO_SAML_VERIFY_REQUEST_H

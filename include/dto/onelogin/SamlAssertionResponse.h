//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_SAML_ASSERTION_RESPONSE_H
#define AWSFORWARDER_DTO_SAML_ASSERTION_RESPONSE_H

// AwsLogin includes
#include <dto/onelogin/model/SamlAssertionDevice.h>
#include <dto/onelogin/model/SamlAssertionUser.h>

/**
 * {
 *   "devices":[
 *     {
 *        "device_type":"Google Authenticator",
 *        "device_id":10427440
 *     }
 *   ],
 *   "message":"MFA is required for this user",
 *   "callback_url":"https://api.us.onelogin.com/api/2/saml_assertion/verify_factor",
 *   "state_token":"afcccda71b822991fa05e8b9b6c8a19ac5d597f1",
 *   "user":{
 *     "lastname":"Vogt",
 *     "firstname":"Jens",
 *     "id":143350913,
 *     "username":"jvogt",
 *     "email":"jens.vogt@opitz-consulting.com"
 *   }
 * }
 */
struct SamlAssertionResponse {

    /**
     * @brief Message
     */
    QString message;

    /**
     * @brief Message
     */
    QString callbackUrl;

    /**
     * @brief State token
     */
    QString stateToken;

    /**
     * @brief Message
     */
    SamlAssertionUser user;

    /**
     * @brief MFA devices
     */
    QVector<SamlAssertionDevice> devices;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject.insert("message", message);
        jsonObject.insert("callback_url", callbackUrl);
        jsonObject.insert("state_token", stateToken);
        jsonObject.insert("user", user.ToJsonObject());
        if (!devices.empty()) {
            QJsonArray devicesArray;
            for (const auto &device: devices) {
                devicesArray.append(device.ToJsonObject());
            }
            jsonObject.insert("devices", devicesArray);
        }
        return QJsonDocument(jsonObject).toJson();
    }

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    void FromJsonObject(const QJsonObject &jsonObject) {
        message = jsonObject.value("message").toString();
        callbackUrl = jsonObject.value("callback_url").toString();
        stateToken = jsonObject.value("state_token").toString();
        user.FromJsonObject(jsonObject.value("user").toObject());
        if (jsonObject.contains("devices") && jsonObject.value("devices").isArray()) {
            for (const auto &jsonDevice : jsonObject.value("devices").toArray()) {
                SamlAssertionDevice device;
                device.FromJsonObject(jsonDevice.toObject());
                devices.append(device);
            }
        }
    }
};

#endif// AWSFORWARDER_DTO_SAML_ASSERTION_RESPONSE_H

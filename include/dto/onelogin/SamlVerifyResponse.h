//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_DTO_SAML_VERIFY_RESPONSE_H
#define AWSFORWARDER_DTO_SAML_VERIFY_RESPONSE_H

struct SamlVerifyResponse {

    /**
     * @brief Data
     */
    QString data;

    /**
     * @brief message
     */
    QString message;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    void FromJsonObject(const QJsonObject &jsonObject) {
        data = jsonObject["data"].toString();
        message = jsonObject["message"].toString();
    }

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject.insert("data", data);
        jsonObject.insert("message", message);
        return QJsonDocument(jsonObject).toJson();
    }
};

#endif// AWSFORWARDER_DTO_SAML_VERIFY_RESPONSE_H

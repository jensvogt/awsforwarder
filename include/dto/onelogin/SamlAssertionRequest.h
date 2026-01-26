//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_DTO_SAML_ASSERTION_REQUEST_H
#define AWSFORWARDER_DTO_SAML_ASSERTION_REQUEST_H

struct SamlAssertionRequest {

    /**
     * @brief UserID or Email
     */
    QString userIdOrEmail;

    /**
     * @brief Password
     */
    QString password;

    /**
     * @brief Application ID
     */
    long appId;

    /**
     * @brief Sub domain
     */
    QString subDomain;

    /**
     * @brief IP address
     */
    QString ipAddress;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject.insert("username_or_email", userIdOrEmail);
        jsonObject.insert("password", password);
        jsonObject.insert("app_id", static_cast<qint64>(appId));
        jsonObject.insert("subdomain", subDomain);
        jsonObject.insert("ip_address", ipAddress);
        return QJsonDocument(jsonObject).toJson();
    }
};

#endif// AWSFORWARDER_DTO_SAML_ASSERTION_REQUEST_H

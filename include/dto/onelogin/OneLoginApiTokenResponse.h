//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_RESPONSE_H
#define AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_RESPONSE_H

// Qt includes
#include <QJsonArray>
#include <QJsonDocument>

// Forwarder includes
#include <dto/onelogin/model/OneLoginApiTokenResponseData.h>
#include <dto/onelogin/model/OneLoginStatus.h>

using namespace std::chrono;

/**
 * AccessToken  string    `json:"access_token"`
 * CreatedAt    time.Time `json:"created_at"`
 * ExpiresIn    int       `json:"expires_in"`
 * RefreshToken string    `json:"refresh_token"`
 * TokenType    string    `json:"token_type"`
 * AccountId    int       `json:"account_id"`
 */
struct OneLoginApiTokenResponse {

    /**
     * Access token
     */
    QString accessToken;

    /**
     * Refresh token
     */
    QString refreshToken;

    /**
     * Token type
     */
    QString tokenType;

    /**
     * Created At
     */
    QDateTime createdAt;

    /**
     * Account ID
     */
    int accountId{};

    /**
     * Created At
     */
    qint64 expiresIn{};

    /**
     * Expires at
     */
    QDateTime expiresAt;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJson() const {
        return QJsonDocument(ToJsonObject()).toJson();
    };

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QJsonObject &jsonObject) {
        accessToken = jsonObject.value("access_token").toString();
        refreshToken = jsonObject.value("refresh_token").toString();
        tokenType = jsonObject.value("token_type").toString();
        createdAt = QDateTime::fromString(jsonObject.value("created_at").toString(), Qt::ISODate).toLocalTime();
        accountId = jsonObject.value("account_id").toInt();
        expiresIn = jsonObject.value("expires_in").toInteger();
        expiresAt = createdAt.addSecs(expiresIn);
    }

    /**
     * @brief Convert to a JSON document
     *
     * @return JSON document
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("access_token", accessToken);
        jsonObject.insert("refresh_token", refreshToken);
        jsonObject.insert("token_type", tokenType);
        jsonObject.insert("created_at", createdAt.toUTC().toString(Qt::ISODate));
        jsonObject.insert("account_id", accountId);
        jsonObject.insert("expires_in", expiresIn);
        return jsonObject;
    }
};

#endif//AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_RESPONSE_H

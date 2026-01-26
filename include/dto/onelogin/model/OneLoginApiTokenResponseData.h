//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_RESPONSE_DATA_H
#define AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_RESPONSE_DATA_H

// Qt includes
#include <QString>
#include <QJsonObject>

// Forwarder includes
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
struct OneLoginApiTokenResponseData {

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
    system_clock::time_point createdAt;

    /**
     * Account ID
     */
    int accountId;

    /**
     * Created At
     */
    long expiresIn;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    QString ToJson() const;

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QJsonObject &jsonObject);

    /**
     * @brief Convert to a JSON document
     *
     * @return JSON document
     */
    QJsonObject ToJsonObject() const;
};

#endif// AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_RESPONSE_DATA_H

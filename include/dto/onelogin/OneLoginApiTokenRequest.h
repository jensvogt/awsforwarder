//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_REQUEST_H
#define AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_REQUEST_H

// Qt includes
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

struct OneLoginApiTokenRequest {

    /**
     * Grant type
     */
    QString grantType;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject.insert("grant_type", grantType);
        return QJsonDocument(jsonObject).toJson();
    }
};

#endif//AWSFORWARDER_DTO_ONELOGIN_API_TOKEN_REQUEST_H

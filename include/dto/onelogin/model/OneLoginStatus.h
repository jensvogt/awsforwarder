//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_ONELOGIN_STATUS_H
#define AWSFORWARDER_DTO_ONELOGIN_STATUS_H

// Qt includes
#include <QString>
#include <QJsonObject>

/**
 * {
 *  "status":{
 *     "error":false,
 *     "code":200,
 *     "type":"success",
 *     "message":"Success"
 *  },
 *  "data":[
 *     {
 *       "access_token":"917c437c78a7c12291f05150d900f5a2824d8c4536616c85b789213864e011d8",
 *       "created_at":"2025-03-08T17:21:53.790Z",
 *       "expires_in":36000,
 *       "refresh_token":"d25a60672410200d6e35ca2a4e22cf08cc78844abde12fe2ff3ff6924f1c3c1b",
 *       "token_type":"bearer",
 *       "account_id":139454
 *     }
 *   ]
 * }
 */
struct OneLoginStatus {

    /**
     * Error flag
     */
    bool error = false;

    /**
     * Code
     */
    int code;

    /**
     * Type
     */
    QString type;

    /**
     * Message
     */
    QString message;

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QJsonObject &jsonObject) {
        error = jsonObject.value("error").toBool();
        code = jsonObject.value("code").toInt();
        type = jsonObject.value("type").toString();
        message = jsonObject.value("message").toString();
    }

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("error", error);
        jsonObject.insert("code", code);
        jsonObject.insert("type", type);
        jsonObject.insert("message", message);
        return jsonObject;
    }

    /**
     * @brief Convert to a JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJson() const {
        const QJsonObject jsonObject = ToJsonObject();
        const QJsonDocument requestDoc(jsonObject);
        return requestDoc.toJson();
    };
};
#endif//AWSFORWARDER_DTO_ONELOGIN_STATUS_H

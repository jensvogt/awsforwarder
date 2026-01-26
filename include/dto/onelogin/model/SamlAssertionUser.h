//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_SAML_ASSERTION_USER_H
#define AWSFORWARDER_DTO_SAML_ASSERTION_USER_H

// C++ includes
#include <string>

struct SamlAssertionUser {

    /**
     * @brief Firstname
     */
    QString firstname;

    /**
     * @brief Lastname
     */
    QString lastname;

    /**
     * @brief ID
     */
    int id;

    /**
     * @brief Username
     */
    QString username;

    /**
     * @brief Email
     */
    QString email;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    QString ToJson() const {
        QJsonObject jsonObject;
        jsonObject.insert("firstname", firstname);
        jsonObject.insert("lastname", lastname);
        jsonObject.insert("id", id);
        jsonObject.insert("username", username);
        jsonObject.insert("email", email);
        return QJsonDocument(jsonObject).toJson();
    }

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QJsonObject &jsonObject) {
        firstname = jsonObject["firstname"].toString();
        lastname = jsonObject["lastname"].toString();
        id = jsonObject["id"].toInt();
        username = jsonObject["username"].toString();
        email = jsonObject["email"].toString();
    }

    /**
     * @brief Convert to a JSON document
     *
     * @return JSON document
     */
    QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("firstname", firstname);
        jsonObject.insert("lastname", lastname);
        jsonObject.insert("id", id);
        jsonObject.insert("username", username);
        jsonObject.insert("email", email);
        return jsonObject;
    }
};

#endif//AWSFORWARDER_DTO_SAML_ASSERTION_USER_H

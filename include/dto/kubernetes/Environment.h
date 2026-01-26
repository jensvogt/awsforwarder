//
// Created by vogje01 on 13/03/2025.
//

#ifndef AWSFORWARDER_DTO_KUBERNETES_ENVIRONMENT_H
#define AWSFORWARDER_DTO_KUBERNETES_ENVIRONMENT_H

// Qt includes
#include <QJsonObject>

struct Environment {

    /**
     * @brief Name
     */
    QString name;

    /**
     * @brief Value
     */
    QString value;

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("name", name);
        jsonObject.insert("value", value);
        return jsonObject;
    }

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJsonString() const {
        return QJsonDocument(ToJsonObject()).toJson();
    }
};

#endif// AWSFORWARDER_DTO_KUBERNETES_ENVIRONMENT_H

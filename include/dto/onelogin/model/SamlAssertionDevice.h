//
// Created by vogje01 on 09/03/2025.
//

#ifndef AWSFORWARDER_DTO_SAML_ASSERTION_DEVICE_H
#define AWSFORWARDER_DTO_SAML_ASSERTION_DEVICE_H

struct SamlAssertionDevice {

    /**
     * @brief Firstname
     */
    QString deviceType;

    /**
     * @brief ID
     */
    int deviceId;

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    QString ToJson() const {
        return QJsonDocument(ToJsonObject()).toJson();
    }

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QJsonObject &jsonObject) {
        deviceType = jsonObject.value("device_type").toString();
        deviceId = jsonObject.value("device_id").toInt();
    }

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("device_type", deviceType);
        jsonObject.insert("device_id", deviceId);
        return jsonObject;
    }
};


#endif// AWSFORWARDER_DTO_SAML_ASSERTION_DEVICE_H

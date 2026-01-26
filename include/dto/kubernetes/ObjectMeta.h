//
// Created by vogje01 on 13/03/2025.
//

#ifndef AWSFORWARDER_DTO_KUBERNETES_OBJECT_META_H
#define AWSFORWARDER_DTO_KUBERNETES_OBJECT_META_H

// C++ standard includes
#include <sstream>
#include <string>

// Qt includes
#include <QJsonObject>
#include <QMap>
#include <QString>

struct ObjectMeta {

    /**
     * @brief Name
     */
    QString name;

    /**
     * @brief Name
     */
    QString uid;

    /**
     * @brief Name space
     */
    QString nameSpace;

    /**
     * @brief Object labels
     */
    QMap<QString, QString> labels;

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QJsonObject &jsonObject);

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("name", name);
        jsonObject.insert("namespace", nameSpace);

        if (!labels.isEmpty()) {
            QJsonObject labelsObject;
            for (const auto &key: labels.keys()) {
                labelsObject.insert(key, labels[key]);
            }
            jsonObject.insert("labels", labelsObject);
        }
        return jsonObject;
    }

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJsonString() const;

    /**
     * @brief Converts the DTO to a string representation.
     *
     * @return DTO as string
     */
    [[nodiscard]] QString ToString() const;
};

#endif// AWSFORWARDER_DTO_KUBERNETES_OBJECT_META_H

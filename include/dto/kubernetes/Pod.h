//
// Created by vogje01 on 13/03/2025.
//

#ifndef AWSFORWARDER_DTO_KUBERNETES_POD_H
#define AWSFORWARDER_DTO_KUBERNETES_POD_H

// Qt includes
#include <QString>
#include <QVector>
#include <QJsonDocument>

// AwsLogin includes
#include <boost/json/impl/object.hpp>
#include <dto/kubernetes/ObjectMeta.h>
#include <dto/kubernetes/PodSpec.h>

struct Pod {

    /**
     * @brief API version
     */
    QString apiVersion;

    /**
     * @brief Request kind
     */
    QString kind;

    /**
     * @brief Object metadata
     */
    ObjectMeta objectMeta;

    /**
     * @brief Pod specification
     */
    PodSpec podSpec;

    /**
     * @brief Convert from a JSON object
     *
     * @param jsonObject JSON object
     */
    void FromJsonObject(const QString &jsonObject);

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("apiVersion", apiVersion);
        jsonObject.insert("kind", kind);
        jsonObject.insert("metadata", objectMeta.ToJsonObject());
        jsonObject.insert("spec", podSpec.ToJsonObject());
        return jsonObject;
    }

    /**
     * @brief Convert to JSON string
     *
     * @return JSON string
     */
    [[nodiscard]] QString ToJsonString() const {
        return QJsonDocument(ToJsonObject()).toJson();
    };
};

#endif//AWSFORWARDER_DTO_KUBERNETES_POD_H

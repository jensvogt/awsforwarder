//
// Created by vogje01 on 13/03/2025.
//

#ifndef AWSFORWARDER_DTO_KUBERNETES_CONTAINER_H
#define AWSFORWARDER_DTO_KUBERNETES_CONTAINER_H

// C++ standard includes
#include <string>
#include <vector>

// AwsLogin includes
#include <dto/kubernetes/Environment.h>
#include <dto/kubernetes/ObjectMeta.h>
#include <dto/kubernetes/PullPolicy.h>

struct Container {
    /**
     * @brief Container name
     */
    QString name;

    /**
     * @brief Image name
     */
    QString imageName;

    /**
     * @brief Image pull policy
     */
    PullPolicy pullPolicy = UnknownPull;

    /**
     * @brief Environment variables
     */
    QVector<Environment> envVars;

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject.insert("name", name);
        jsonObject.insert("image", imageName);
        jsonObject.insert("imagePullPolicy", PullPolicyToString(pullPolicy));
        if (!envVars.isEmpty()) {
            QJsonArray envVarsArray;
            for (const auto &envVar: envVars) {
                envVarsArray.append(envVar.ToJsonObject());
            }
            jsonObject.insert("env", envVarsArray);
        }
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

#endif// AWSFORWARDER_DTO_KUBERNETES_CONTAINER_H

//
// Created by vogje01 on 13/03/2025.
//

#ifndef AWSFORWARDER_DTO_KUBERNETES_PODSPEC_H
#define AWSFORWARDER_DTO_KUBERNETES_PODSPEC_H

// C++ standard includes
#include <string>
#include <vector>

// AwsLogin includes
#include <dto/kubernetes/Container.h>

struct PodSpec {

    /**
     * @brief Containers
     */
    std::vector<Container> containers;

    /**
     * @brief Convert to a JSON object
     *
     * @return JSON object
     */
    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        if (!containers.empty()) {
            QJsonArray containersArray;
            for (const auto &container: containers) {
                containersArray.push_back(container.ToJsonObject());
            }
            jsonObject.insert("containers", containersArray);
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

#endif//AWSFORWARDER_DTO_KUBERNETES_PODSPEC_H

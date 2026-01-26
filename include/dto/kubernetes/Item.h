//
// Created by vogje01 on 12/03/2025.
//

#ifndef AWSLOGIN_DTO_MODEL_ITEM_H
#define AWSLOGIN_DTO_MODEL_ITEM_H

// C++ includes
#include <sstream>
#include <string>
#include <vector>

// Boost includes
#include <boost/json.hpp>

namespace AwsLogin::Dto {

    /**
     * @Brief Pod item
     */
    struct Item {

        /**
         * API version
         */
        std::string apiVersion;

        /**
         * Item kind
         */
        std::string kind;

        /**
         * @brief Convert from a JSON object
         *
         * @param jsonObject JSON object
         */
        void FromJsonObject(const boost::json::object &jsonObject);

        /**
         * @brief Convert to a JSON object
         *
         * @return JSON object
         */
        [[nodiscard]] boost::json::object ToJsonObject() const;

        /**
         * @brief Convert to a JSON string
         *
         * @return JSON string
         */
        [[nodiscard]] std::string ToJsonString() const;

        /**
         * @brief Converts the DTO to a string representation.
         *
         * @return DTO as string
         */
        [[nodiscard]] std::string ToString() const;

        /**
         * @brief Stream provider.
         *
         * @return output stream
         */
        friend std::ostream &operator<<(std::ostream &os, const Item &r);
    };

}// namespace AwsLogin::Dto


#endif// AWSLOGIN_DTO_MODEL_ITEM_H

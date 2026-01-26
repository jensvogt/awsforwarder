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
     * @Brief Forwarder DTO
     */
    struct Forwarder {

        /**
         * Forwarder name
         */
        std::string name;

        /**
         * Forwarder name space
         */
        std::string nameSpace;

        /**
         * Forwarder name space
         */
        int localPort;

        /**
         * Forwarder name space
         */
        int remotePort;
    };

}// namespace AwsLogin::Dto


#endif// AWSLOGIN_DTO_MODEL_ITEM_H

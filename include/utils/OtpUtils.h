//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_OTP_UTILS_H
#define AWSFORWARDER_OTP_UTILS_H

// C++ includes
#include <string>
#include <thread>

// AwsLogin includes
#include <utils/Configuration.h>
#include <utils/Bytes.h>
#include <utils/Otp.h>

class OtpUtils {

public:
    /**
     * @brief Constructor
     */
    OtpUtils();

    /**
     * @brief Singleton instance
     */
    static OtpUtils &instance() {
        static OtpUtils otpUtils;
        return otpUtils;
    }

    /**
     * @brief wait for the next available token
     */
    uint64_t WaitForNextToken();

private:
    /**
     * @brief OTP key
     */
    ByteString _key;

    /**
     * @brief OTP token
     */
    uint32_t _otpToken;
};
#endif// AWSFORWARDER_OTP_UTILS_H

//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_OTP_H
#define AWSFORWARDER_OTP_H

// C++ standard includes
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <cinttypes>
#include <cstring>

// AwsForwarder includes
#include <utils/Bytes.h>
#include <utils/Sha1.h>


/**
 * The 64-bit-blocksize variant of HMAC-SHA1.
 */
ByteString hmacSha1_64(const ByteString &key, const ByteString &msg);

/**
 * Calculate the HOTP value of the given key, message and digit count.
 */
uint32_t hotp(const ByteString &key, uint64_t counter, size_t digitCount = 6, HmacFunc hmac = hmacSha1_64);

/**
 * Calculate the TOTP value from the given parameters.
 */
uint32_t totp(const ByteString &key, uint64_t timeNow, uint64_t timeStart, uint64_t timeStep, size_t digitCount = 6, HmacFunc hmac = hmacSha1_64);

#endif //AWSFORWARDER_OTP_H

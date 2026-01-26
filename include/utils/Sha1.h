//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_SHA1_H
#define AWSFORWARDER_SHA1_H

#include <utils/Bytes.h>

typedef ByteString (*HmacFunc)(const ByteString &, const ByteString &);

/**
 * Calculate the SHA-1 hash of the given message.
 */
ByteString sha1(const ByteString &msg);

/**
 * Calculate the HMAC-SHA-1 hash of the given key/message pair.
 *
 * @note Most services assume a block size of 64.
 */
ByteString hmacSha1(const ByteString &key, const ByteString &msg, size_t blockSize = 64);

#endif//AWSFORWARDER_SHA1_H

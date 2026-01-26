//
// Created by vogje01 on 08/03/2025.
//

#ifndef AWSFORWARDER_BYTES_H
#define AWSFORWARDER_BYTES_H

// C++ standard includes
#include <string>
#include <sstream>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cstdlib>

/** The type of a single byte. */
#ifdef __APPLE__
typedef char Byte;
#else
typedef uint8_t Byte;
#endif

/** The type of a byte string. */
typedef std::basic_string<Byte> ByteString;

/** Deletes the contents of a byte string. */
void clearByteString(ByteString *bstr);

/** Replaces target with source, clearing as much as possible. */
void swizzleByteStrings(ByteString *target, ByteString *source);

/** Converts a byte string into a hex string. */
std::string toHexString(const ByteString &bstr);

/** Converts an unsigned 32-bit integer into a corresponding byte string. */
ByteString u32beToByteString(uint32_t num);

/** Converts an unsigned 64-bit integer into a corresponding byte string. */
ByteString u64beToByteString(uint64_t num);

/** Converts a Base32 string into the correspoding byte string. */
ByteString fromBase32(const std::string &b32str);

/**
 * Converts a potentially unpadded Base32 string into the corresponding byte
 * string.
 */
ByteString fromUnpaddedBase32(const std::string &b32str);

/** Converts byte string into the corresponding Base32 string. */
std::string toBase32(const ByteString &b32str);

/** Deletes the contets of a byte string on destruction. */
class ByteStringDestructor {

    /** The byte string to clear. */
    ByteString *m_bs;

public:
    ByteStringDestructor(ByteString *bs) : m_bs(bs) {
    }

    ~ByteStringDestructor() { clearByteString(m_bs); }
};
#endif //AWSFORWARDER_BYTES_H

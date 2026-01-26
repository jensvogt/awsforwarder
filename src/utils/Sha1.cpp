//
// Created by vogje01 on 08/03/2025.
//

#include <utils/Bytes.h>

#include <iostream>
#include <cassert>

static inline uint32_t lrot32(uint32_t num, uint8_t rotcount) {
    return (num << rotcount) | (num >> (32 - rotcount));
}

ByteString sha1(const ByteString &msg) {

    const size_t size_bytes = msg.size();
    const uint64_t size_bits = size_bytes * 8;
    ByteString bstr = msg;
    ByteStringDestructor asplode(&bstr);

    // the size of msg in bits is always even. adding the '1' bit will make
    // it odd and therefore incongruent to 448 modulo 512, so we can get
    // away with tacking on 0x80 and then the 0x00s.
    bstr.push_back(0x80);
    while (bstr.size() % (512 / 8) != (448 / 8)) {
        bstr.push_back(0x00);
    }

    // append the size in bits (uint64be)
    bstr.append(u64beToByteString(size_bits));

    assert(bstr.size() % (512/8) == 0);

    // initialize the hash counters
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    // for each 64-byte chunk
    for (size_t i = 0; i < bstr.size() / 64; ++i) {
        ByteString chunk(bstr.begin() + i * 64, bstr.begin() + (i + 1) * 64);
        ByteStringDestructor xplode(&chunk);

        uint32_t words[80];
        size_t j;

        // 0-15: the chunk as a sequence of 32-bit big-endian integers
        for (j = 0; j < 16; ++j) {
            words[j] =
                    (chunk[4 * j + 0] << 24) |
                    (chunk[4 * j + 1] << 16) |
                    (chunk[4 * j + 2] << 8) |
                    (chunk[4 * j + 3] << 0);
        }

        // 16-79: derivatives of 0-15
        for (j = 16; j < 32; ++j) {
            // unoptimized
            words[j] = lrot32(words[j - 3] ^ words[j - 8] ^ words[j - 14] ^ words[j - 16], 1);
        }
        for (j = 32; j < 80; ++j) {
            // Max Locktyuchin's optimization (SIMD)
            words[j] = lrot32(words[j - 6] ^ words[j - 16] ^ words[j - 28] ^ words[j - 32], 2);
        }

        // initialize hash values for the round
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        // the loop
        for (j = 0; j < 80; ++j) {
            uint32_t f = 0, k = 0;

            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else if (j < 80) {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            } else {
                assert(0 && "how did I get here?");
            }

            uint32_t tmp = lrot32(a, 5) + f + e + k + words[j];
            e = d;
            d = c;
            c = lrot32(b, 30);
            b = a;
            a = tmp;
        }

        // add that to the result so far
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // assemble the digest
    ByteString first = u32beToByteString(h0);
    ByteStringDestructor x1(&first);
    ByteString second = u32beToByteString(h1);
    ByteStringDestructor x2(&second);
    ByteString third = u32beToByteString(h2);
    ByteStringDestructor x3(&third);
    ByteString fourth = u32beToByteString(h3);
    ByteStringDestructor x4(&fourth);
    ByteString fifth = u32beToByteString(h4);
    ByteStringDestructor x5(&fifth);

    return first + second + third + fourth + fifth;
}

ByteString hmacSha1(const ByteString &key, const ByteString &msg, size_t blockSize = 64);

ByteString hmacSha1(const ByteString &key, const ByteString &msg, size_t blockSize) {
    ByteString realKey = key;
    ByteStringDestructor asplode(&realKey);

    if (realKey.size() > blockSize) {
        // resize by calculating hash
        ByteString newRealKey = sha1(realKey);
        swizzleByteStrings(&realKey, &newRealKey);
    }
    if (realKey.size() < blockSize) {
        // pad with zeroes
        realKey.resize(blockSize, 0x00);
    }

    // prepare the pad keys
    ByteString innerPadKey = realKey;
    ByteStringDestructor xplodeI(&innerPadKey);
    ByteString outerPadKey = realKey;
    ByteStringDestructor xplodeO(&outerPadKey);

    // transform the pad keys
    for (size_t i = 0; i < realKey.size(); ++i) {
        innerPadKey[i] = innerPadKey[i] ^ 0x36;
        outerPadKey[i] = outerPadKey[i] ^ 0x5c;
    }

    // sha1(outerPadKey + sha1(innerPadKey + msg))
    ByteString innerMsg = innerPadKey + msg;
    ByteStringDestructor xplodeIM(&innerMsg);
    ByteString innerHash = sha1(innerMsg);
    ByteStringDestructor xplodeIH(&innerHash);
    ByteString outerMsg = outerPadKey + innerHash;
    ByteStringDestructor xplodeOM(&outerMsg);

    return sha1(outerMsg);
}

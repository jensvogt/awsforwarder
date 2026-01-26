//
// Created by vogje01 on 08/03/2025.
//

#include <utils/Otp.h>

ByteString hmacSha1_64(const ByteString &key, const ByteString &msg) {
    return hmacSha1(key, msg, 64);
}

uint32_t hotp(const ByteString &key, const uint64_t counter, const size_t digitCount, HmacFunc hmacf) {

    ByteString msg = u64beToByteString(counter);
    ByteStringDestructor dmsg(&msg);

    ByteString hmac = hmacf(key, msg);
    ByteStringDestructor dhmac(&hmac);

    uint32_t digits10 = 1;
    for (size_t i = 0; i < digitCount; ++i) {
        digits10 *= 10;
    }

    // fetch the offset (from the last nibble)
    const uint8_t offset = hmac[hmac.size() - 1] & 0x0F;

    // fetch the four bytes from the offset
    ByteString fourWord = hmac.substr(offset, 4);
    ByteStringDestructor dfourWord(&fourWord);

    // turn them into a 32-bit integer
    const uint32_t ret = (fourWord[0] << 24) | (fourWord[1] << 16) | (fourWord[2] << 8) | (fourWord[3] << 0);

    // snip off the MSB (to alleviate signed/unsigned troubles)
    // and calculate modulo digit count
    return (ret & 0x7fffffff) % digits10;
}

uint32_t totp(const ByteString &key, const uint64_t timeNow, const uint64_t timeStart, const uint64_t timeStep, const size_t digitCount, HmacFunc hmacf) {
    const uint64_t timeValue = (timeNow - timeStart) / timeStep;
    return hotp(key, timeValue, digitCount, hmacf);
}

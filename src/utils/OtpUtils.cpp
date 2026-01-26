//
// Created by vogje01 on 08/03/2025.
//

#include <utils/OtpUtils.h>

OtpUtils::OtpUtils() : _otpToken(0) {
    _key = fromBase32(Configuration::instance().GetOtpKey().toStdString());
}

uint64_t OtpUtils::WaitForNextToken() {

    if (_otpToken == 0) {
        _otpToken = hotp(_key, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 30);
    } else {
        uint32_t newToken = hotp(_key, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 30);
        while (newToken == _otpToken) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            newToken = hotp(_key, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 30);
        };
        _otpToken = newToken;
    }
    return _otpToken;
}


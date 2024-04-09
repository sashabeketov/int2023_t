#include "number.h"
#include <iomanip>

const int2023_t ZERO = from_int(0);
const int2023_t TEN = from_int(10);

int2023_t from_int(int32_t value) {
    auto result = int2023_t();
    for (int it = 0; it < 4; ++it) {
        result.data[it] = std::byte(value >> (it << 3) & 0xFF);
    }
    if (value < 0) {
        for (int it = 5; it < sizeof(int2023_t); ++it) {
            result.data[it] = std::byte(0xFF);
        }
    }
    return result;
}

short int signum(int2023_t value) {
    if (value == ZERO) {
        return 0;
    }
    if (int(value.data[255] >> 7) == 1) {
        return -1;
    } else {
        return 1;
    }
}

int2023_t abs(int2023_t value) {
    if (int(value.data[255] >> 7) == 1) {
        return from_int(0) - value;
    } else {
        return value;
    }
}

int2023_t from_string(const char *buff) {
    auto res = int2023_t();
    int it = 0;
    bool neg_flag = false;
    if (buff[0] == '-') {
        neg_flag = true;
        ++it;
    }
    for (; it < strlen(buff); ++it) {
        res = res * TEN + from_int(buff[it] - '0');
    }
    return neg_flag ? ZERO - res : res;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    return int2023_t();
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return int2023_t();
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    return int2023_t();
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    return int2023_t();
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    return false;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return false;
}

std::ostream &operator<<(std::ostream &ostream, const int2023_t &value) {
    bool zero_flag = true;
    ostream << "0x";
    for (int it = 252; it >= 0; --it) {
        if (value.data[it] == std::byte(0) && zero_flag) {
            continue;
        }
        zero_flag = false;
        ostream << std::hex << int(value.data[it] >> 4);
        ostream << std::hex << int(value.data[it] & std::byte(0xF));
    }
    if (zero_flag) {
        ostream << "0";
    }
    return ostream;
}

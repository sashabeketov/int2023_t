#include "number.h"
#include <iomanip>

const int2023_t ZERO = from_int(0);
const int2023_t TEN = from_int(10);

int2023_t from_int(int32_t value) {
    int2023_t result = int2023_t(); // явная инициализация
//    int2023_t result; // неявная инициализация
    for (int it = 0; it < 4; ++it) {
        result.data[it] = std::byte(value >> (it << 3) & 0xFF);
    }
    if (value < 0) {
        for (int it = 4; it < sizeof(int2023_t); ++it) {
            result.data[it] = std::byte(0xFF);
        }
    }
    return result;
}

short int signum(int2023_t value) {
    if (value == ZERO) {
        return 0;
    }
    if (int(value.data[252] >> 7) == 1) {
        return -1;
    } else {
        return 1;
    }
}

int2023_t abs(int2023_t value) {
    if (int(value.data[252] >> 7) == 1) {
        return ZERO - value;
    } else {
        return value;
    }
}

int2023_t from_string(const char *buff) {
    int2023_t res = int2023_t();
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
    int2023_t result = int2023_t();
    bool carry_flag = false;
    for (int it = 0; it < 253; ++it) {
        int tmp = int(lhs.data[it]) + int(rhs.data[it]) + carry_flag;
        result.data[it] = std::byte(tmp % 0x100);
        if (tmp > 0xFF) {
            carry_flag = true;
        } else {
            carry_flag = false;
        }
    }
    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result = int2023_t();
    bool carry_flag = false;
    for (int it = 0; it < 253; ++it) {
        int tmp = int(lhs.data[it]) - int(rhs.data[it]) - carry_flag;
        result.data[it] = std::byte(tmp % 0x100);
        if (tmp < 0) {
            carry_flag = true;
        } else {
            carry_flag = false;
        }
    }
    return result;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int sign = signum(lhs) * signum(rhs);
    if (sign == 0) {
        return ZERO;
    }
    int2023_t lhs_abs = abs(lhs);
    int2023_t rhs_abs = abs(rhs);
    int2023_t result = int2023_t();
    for (auto & it : rhs_abs.data) {
        for (int jt = 0; jt < 8; ++jt) {
            if ((char) it >> jt & 1) {
                result = result + lhs_abs;
            }
            lhs_abs = lhs_abs << 1;
        }
    }
    if (sign > 0) {
        return result;
    } else {
        return ZERO - result;
    }
}

bool operator>(const int2023_t &lhs, const int2023_t &rhs) {
    return signum(lhs - rhs) > 0;
}

bool operator<=(const int2023_t &lhs, const int2023_t &rhs) {
    return signum(lhs - rhs) <= 0;
}

bool operator>=(const int2023_t &lhs, const int2023_t &rhs) {
    return signum(lhs - rhs) >= 0;
}

bool operator<(const int2023_t &lhs, const int2023_t &rhs) {
    return signum(lhs - rhs) < 0;
}

int2023_t operator/(const int2023_t &lhs, const int2023_t &rhs) {
    int2023_t lhs_abs = abs(lhs);
    int2023_t rhs_abs = abs(rhs);
    if (rhs == ZERO) {
        throw std::runtime_error("Division by zero");
    }
    if (lhs_abs < rhs_abs) {
        return ZERO;
    }
    auto res = int2023_t();
    int2023_t buf = lhs_abs;
    while (buf >= rhs_abs) {
        int2023_t div = rhs_abs;
        int2023_t mul = from_int(1);
        while (div + div <= buf) {
            div = div + div;
            mul = mul + mul;
        }
        buf = buf - div;
        res = res + mul;
    }
    return (signum(lhs) * signum(rhs) > 0) ? res : ZERO - res;
}

int2023_t operator<<(const int2023_t &lhs, const int &rhs) {
    int2023_t result = lhs;
    for (int it = 0; it < rhs; ++it) {
        bool carry = false;
        for (auto & jt : result.data) {
            bool tmp = (char) jt & 0x80;
            jt <<= 1;
            jt |= std::byte(carry);
            carry = tmp;
        }
    }
    return result;
}

bool operator==(const int2023_t &lhs, const int2023_t &rhs) {
    for (int it = 0; it < 253; ++it) {
        if (lhs.data[it] != rhs.data[it]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t &lhs, const int2023_t &rhs) {
    return !(lhs == rhs);
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

#pragma once
#include <cinttypes>
#include <iostream>


struct int2023_t {
    std::byte data[253];
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t&, const int2023_t&);

bool operator<(const int2023_t&, const int2023_t&);

bool operator>=(const int2023_t&, const int2023_t&);

bool operator<=(const int2023_t&, const int2023_t&);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& ostream, const int2023_t& value);

int2023_t operator<<(const int2023_t& lhs, const int& rhs);
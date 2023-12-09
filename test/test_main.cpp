/*
 * Copyright (C) 2023 Nikolas Koesling <nikolas@koesling.info>.
 * This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
 */

#include "../src/endian.hpp"
#include "MemoryFormatter.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

int main() {
    std::vector<uint64_t> data;

    float  d_float  = 42.0f;
    double d_double = 420.0;

    void                                       *addr;
    std::string                                 str;
    std::shared_ptr<memformat::MemoryFormatter> formatter;
    std::ostringstream                          ss;

    // float
    addr              = &data.emplace_back(0);
    auto p_float_host = reinterpret_cast<float *>(addr);
    *p_float_host     = d_float;
    formatter         = memformat::MemoryFormatter::get_formatter(
            p_float_host, "0x0", memformat::wordsize::BIT_32, memformat::format::FLOAT, memformat::endianness::HOST);
    str = formatter->string();
    assert(str == "42.000000");

    addr             = &data.emplace_back(0);
    auto p_float_big = reinterpret_cast<float *>(addr);
    *p_float_big     = ::endian::host_to_big(d_float);
    formatter        = memformat::MemoryFormatter::get_formatter(
            p_float_big, "0x0", memformat::wordsize::BIT_32, memformat::format::FLOAT, memformat::endianness::BIG);
    str = formatter->string();
    assert(str == "42.000000");

    addr                = &data.emplace_back(0);
    auto p_float_little = reinterpret_cast<float *>(addr);
    *p_float_little     = ::endian::host_to_little(d_float);
    formatter           = memformat::MemoryFormatter::get_formatter(p_float_little,
                                                          "0x0",
                                                          memformat::wordsize::BIT_32,
                                                          memformat::format::FLOAT,
                                                          memformat::endianness::LITTLE);
    str                 = formatter->string();
    assert(str == "42.000000");

    // double
    addr               = &data.emplace_back(0);
    auto p_double_host = reinterpret_cast<double *>(addr);
    *p_double_host     = d_double;
    formatter          = memformat::MemoryFormatter::get_formatter(
            p_double_host, "0x0", memformat::wordsize::BIT_64, memformat::format::FLOAT, memformat::endianness::HOST);
    str = formatter->string();
    assert(str == "420.000000");

    addr              = &data.emplace_back(0);
    auto p_double_big = reinterpret_cast<double *>(addr);
    *p_double_big     = ::endian::host_to_big(d_double);
    formatter         = memformat::MemoryFormatter::get_formatter(
            p_double_big, "0x0", memformat::wordsize::BIT_64, memformat::format::FLOAT, memformat::endianness::BIG);
    str = formatter->string();
    assert(str == "420.000000");

    addr                 = &data.emplace_back(0);
    auto p_double_little = reinterpret_cast<double *>(addr);
    *p_double_little     = ::endian::host_to_little(d_double);
    formatter            = memformat::MemoryFormatter::get_formatter(p_double_little,
                                                          "0x0",
                                                          memformat::wordsize::BIT_64,
                                                          memformat::format::FLOAT,
                                                          memformat::endianness::LITTLE);
    str                  = formatter->string();
    assert(str == "420.000000");

    // single bits, additionally test memory offset as oct, hex and dec
    addr           = &data.emplace_back(0);
    auto    p_bits = reinterpret_cast<uint8_t *>(addr);
    uint8_t d_bits = 0b10010011;
    *p_bits        = d_bits;
    for (std::size_t i = 0; i < 8; ++i) {
        auto expect = (d_bits >> i) & 0x1;
        formatter   = memformat::MemoryFormatter::get_formatter(
                p_bits, "0x0." + std::to_string(i), memformat::wordsize::BIT_1);
        str = formatter->string();
        assert(str == (expect ? "1" : "0"));

        formatter = memformat::MemoryFormatter::get_formatter(
                p_bits - 45457, "45457." + std::to_string(i), memformat::wordsize::BIT_1);
        str = formatter->string();
        assert(str == (expect ? "1" : "0"));

        formatter = memformat::MemoryFormatter::get_formatter(
                p_bits - 045457, "045457." + std::to_string(i), memformat::wordsize::BIT_1);
        str = formatter->string();
        assert(str == (expect ? "1" : "0"));

        formatter = memformat::MemoryFormatter::get_formatter(
                p_bits - 0x45457, "0x45457." + std::to_string(i), memformat::wordsize::BIT_1);
        str = formatter->string();
        assert(str == (expect ? "1" : "0"));
    }

    // 8 bit
    addr       = &data.emplace_back(0);
    auto    p8 = reinterpret_cast<uint8_t *>(addr);
    uint8_t d8 = 0b10010011;
    *p8        = d8;

    formatter =
            memformat::MemoryFormatter::get_formatter(p8, "0x0", memformat::wordsize::BIT_8, memformat::format::BIN);
    str = formatter->string();
    assert(str == "10010011");

    formatter =
            memformat::MemoryFormatter::get_formatter(p8, "0x0", memformat::wordsize::BIT_8, memformat::format::OCT);
    str = formatter->string();
    ss << std::oct << static_cast<unsigned>(d8);
    assert(str == ss.str());
    ss.str("");

    formatter =
            memformat::MemoryFormatter::get_formatter(p8, "0x0", memformat::wordsize::BIT_8, memformat::format::HEX);
    str = formatter->string();
    ss << std::hex << static_cast<unsigned>(d8);
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p8, "0x0", memformat::wordsize::BIT_8, memformat::format::UNSIGNED);
    str = formatter->string();
    ss << std::dec << static_cast<unsigned>(d8);
    assert(str == ss.str());
    ss.str("");

    formatter =
            memformat::MemoryFormatter::get_formatter(p8, "0x0", memformat::wordsize::BIT_8, memformat::format::SIGNED);
    str = formatter->string();
    ss << std::dec << static_cast<int>(*reinterpret_cast<int8_t *>(&d8));
    assert(str == ss.str());
    ss.str("");

    // 16 bit
    addr         = &data.emplace_back(0);
    auto     p16 = reinterpret_cast<uint16_t *>(addr);
    uint16_t d16 = 0b1001001111110000;
    *p16         = d16;

    formatter =
            memformat::MemoryFormatter::get_formatter(p16, "0x0", memformat::wordsize::BIT_16, memformat::format::BIN);
    str = formatter->string();
    assert(str == "1001001111110000");

    formatter =
            memformat::MemoryFormatter::get_formatter(p16, "0x0", memformat::wordsize::BIT_16, memformat::format::OCT);
    str = formatter->string();
    ss << std::oct << d16;
    assert(str == ss.str());
    ss.str("");

    formatter =
            memformat::MemoryFormatter::get_formatter(p16, "0x0", memformat::wordsize::BIT_16, memformat::format::HEX);
    str = formatter->string();
    ss << std::hex << d16;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p16, "0x0", memformat::wordsize::BIT_16, memformat::format::UNSIGNED);
    str = formatter->string();
    ss << std::dec << d16;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p16, "0x0", memformat::wordsize::BIT_16, memformat::format::SIGNED);
    str = formatter->string();
    ss << std::dec << *reinterpret_cast<int16_t *>(&d16);
    assert(str == ss.str());
    ss.str("");

    // 32 bit
    addr         = &data.emplace_back(0);
    auto     p32 = reinterpret_cast<uint32_t *>(addr);
    uint32_t d32 = 0b10010011111100000111110111110010;
    *p32         = d32;

    formatter =
            memformat::MemoryFormatter::get_formatter(p32, "0x0", memformat::wordsize::BIT_32, memformat::format::BIN);
    str = formatter->string();
    assert(str == "10010011111100000111110111110010");

    formatter =
            memformat::MemoryFormatter::get_formatter(p32, "0x0", memformat::wordsize::BIT_32, memformat::format::OCT);
    str = formatter->string();
    ss << std::oct << d32;
    assert(str == ss.str());
    ss.str("");

    formatter =
            memformat::MemoryFormatter::get_formatter(p32, "0x0", memformat::wordsize::BIT_32, memformat::format::HEX);
    str = formatter->string();
    ss << std::hex << d32;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p32, "0x0", memformat::wordsize::BIT_32, memformat::format::UNSIGNED);
    str = formatter->string();
    ss << std::dec << d32;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p32, "0x0", memformat::wordsize::BIT_32, memformat::format::SIGNED);
    str = formatter->string();
    ss << std::dec << *reinterpret_cast<int32_t *>(&d32);
    assert(str == ss.str());
    ss.str("");

    // 64 bit
    addr         = &data.emplace_back(0);
    auto     p64 = reinterpret_cast<uint64_t *>(addr);
    uint64_t d64 = 0b1001001111110000011111011111001010010011111100000111110111110010;
    *p64         = d64;

    formatter =
            memformat::MemoryFormatter::get_formatter(p64, "0x0", memformat::wordsize::BIT_64, memformat::format::BIN);
    str = formatter->string();
    assert(str == "1001001111110000011111011111001010010011111100000111110111110010");

    formatter =
            memformat::MemoryFormatter::get_formatter(p64, "0x0", memformat::wordsize::BIT_64, memformat::format::OCT);
    str = formatter->string();
    ss << std::oct << d64;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p64 - 1, std::to_string(sizeof(*p64)), memformat::wordsize::BIT_64, memformat::format::HEX);
    str = formatter->string();
    ss << std::hex << d64;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p64 - 5769, std::to_string(sizeof(*p64) * 5769), memformat::wordsize::BIT_64, memformat::format::UNSIGNED);
    str = formatter->string();
    ss << std::dec << d64;
    assert(str == ss.str());
    ss.str("");

    formatter = memformat::MemoryFormatter::get_formatter(
            p64, "0x0", memformat::wordsize::BIT_64, memformat::format::SIGNED);
    str = formatter->string();
    ss << std::dec << *reinterpret_cast<int64_t *>(&d64);
    assert(str == ss.str());
    ss.str("");


    // TODO test endianness (+ swap 16/32)
}

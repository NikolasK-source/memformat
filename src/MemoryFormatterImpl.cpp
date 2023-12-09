/*
 * Copyright (C) 2023 Nikolas Koesling <nikolas@koesling.info>.
 * This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
 */

#include "MemoryFormatterImpl.hpp"

#include "endian.hpp"
#include "split_string.hpp"

#include <bitset>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace memformat {

MemoryFormatter_Bit_1::MemoryFormatter_Bit_1(void *base_address, std::size_t offset, std::size_t bit_offset)
    : MemoryFormatter(base_address, offset), bit_offset(bit_offset) {}

std::string MemoryFormatter_Bit_1::string() const {
    const auto byte = *(reinterpret_cast<volatile uint8_t *>(base_address) + offset);
    const auto bit  = (byte >> bit_offset) & 0x1;
    return bit ? "1" : "0";
}

size_t MemoryFormatter_Bit_1::max_offset() const { return offset; }


MemoryFormatter_Bit_8::MemoryFormatter_Bit_8(void *base_address, std::size_t offset, endianness)
    : MemoryFormatter(base_address, offset) {}

std::size_t MemoryFormatter_Bit_8::max_offset() const { return offset; }

uint8_t MemoryFormatter_Bit_8::get_data() const {
    return *(reinterpret_cast<volatile uint8_t *>(base_address) + offset);
}

MemoryFormatter_Bit_16::MemoryFormatter_Bit_16(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter(base_address, offset) {
    switch (endian) {
        case endianness::HOST: convert_endianess = [](uint16_t data) { return data; }; break;
        case endianness::BIG: convert_endianess = [](uint16_t data) { return ::endian::big_to_host(data); }; break;
        case endianness::LITTLE:
            convert_endianess = [](uint16_t data) { return ::endian::little_to_host(data); };
            break;
        case endianness::BIG_SWAP16:
            throw std::invalid_argument("Endianness BIG_SWAP16 is not allowed for 16 bit values");
        case endianness::LITTLE_SWAP16:
            throw std::invalid_argument("Endianness LITTLE_SWAP16 is not allowed for 16 bit values");
        case endianness::BIG_SWAP32:
            throw std::invalid_argument("Endianness BIG_SWAP32 is not allowed for 16 bit values");
        case endianness::LITTLE_SWAP32:
            throw std::invalid_argument("Endianness LITTLE_SWAP32 is not allowed for 16 bit values");
    }
}

std::size_t MemoryFormatter_Bit_16::max_offset() const { return offset + 1; }

uint16_t MemoryFormatter_Bit_16::get_data() const {
    return convert_endianess(
            *reinterpret_cast<volatile uint16_t *>((reinterpret_cast<volatile uint8_t *>(base_address) + offset)));
}

MemoryFormatter_Bit_32::MemoryFormatter_Bit_32(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter(base_address, offset) {
    switch (endian) {
        case endianness::HOST: convert_endianess = [](uint32_t data) { return data; }; break;
        case endianness::BIG: convert_endianess = [](uint32_t data) { return ::endian::big_to_host(data); }; break;
        case endianness::LITTLE:
            convert_endianess = [](uint32_t data) { return ::endian::little_to_host(data); };
            break;
        case endianness::BIG_SWAP16:
            convert_endianess = [](uint32_t data) {
                union {  // NOLINT
                    uint32_t result;
                    uint16_t raw[2];
                };
                result = ::endian::big_to_host(data);
                std::swap(raw[0], raw[1]);
                return result;
            };
            break;
        case endianness::LITTLE_SWAP16:
            convert_endianess = [](uint32_t data) {
                union {  // NOLINT
                    uint32_t result;
                    uint16_t raw[2];
                };
                result = ::endian::little_to_host(data);
                std::swap(raw[0], raw[1]);
                return result;
            };
            break;
        case endianness::BIG_SWAP32:
            throw std::invalid_argument("Endianness BIG_SWAP32 is not allowed for 32 bit values");
        case endianness::LITTLE_SWAP32:
            throw std::invalid_argument("Endianness LITTLE_SWAP32 is not allowed for 32 bit values");
    }
}

std::size_t MemoryFormatter_Bit_32::max_offset() const { return offset + 3; }

uint32_t MemoryFormatter_Bit_32::get_data() const {
    return convert_endianess(
            *reinterpret_cast<volatile uint32_t *>((reinterpret_cast<volatile uint8_t *>(base_address) + offset)));
}

MemoryFormatter_Bit_64::MemoryFormatter_Bit_64(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter(base_address, offset) {
    switch (endian) {
        case endianness::HOST: convert_endianess = [](uint64_t data) { return data; }; break;
        case endianness::BIG: convert_endianess = [](uint64_t data) { return ::endian::big_to_host(data); }; break;
        case endianness::LITTLE:
            convert_endianess = [](uint64_t data) { return ::endian::little_to_host(data); };
            break;
        case endianness::BIG_SWAP16:
            convert_endianess = [](uint64_t data) {
                union {  // NOLINT
                    uint64_t result;
                    uint16_t raw[4];
                };
                result = ::endian::big_to_host(data);
                std::swap(raw[0], raw[1]);
                std::swap(raw[2], raw[3]);
                return result;
            };
            break;
        case endianness::LITTLE_SWAP16:
            convert_endianess = [](uint64_t data) {
                union {  // NOLINT
                    uint64_t result;
                    uint16_t raw[4];
                };
                result = ::endian::little_to_host(data);
                std::swap(raw[0], raw[1]);
                std::swap(raw[2], raw[3]);
                return result;
            };
            break;
        case endianness::BIG_SWAP32:
            convert_endianess = [](uint64_t data) {
                union {  // NOLINT
                    uint64_t result;
                    uint32_t raw[2];
                };
                result = ::endian::big_to_host(data);
                std::swap(raw[0], raw[1]);
                return result;
            };
            break;
        case endianness::LITTLE_SWAP32:
            convert_endianess = [](uint64_t data) {
                union {  // NOLINT
                    uint64_t result;
                    uint32_t raw[2];
                };
                result = ::endian::little_to_host(data);
                std::swap(raw[0], raw[1]);
                return result;
            };
            break;
    }
}

std::size_t MemoryFormatter_Bit_64::max_offset() const { return offset + 7; }

uint64_t MemoryFormatter_Bit_64::get_data() const {
    auto data_raw =
            *reinterpret_cast<volatile uint64_t *>((reinterpret_cast<volatile uint8_t *>(base_address) + offset));
    auto data_endian = convert_endianess(data_raw);
    return data_endian;
}

MemoryFormatter_Bit_8_Bin::MemoryFormatter_Bit_8_Bin(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_8(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_8_Bin::string() const {
    std::ostringstream result;
    result << std::bitset<8>(get_data());
    return result.str();
}

MemoryFormatter_Bit_8_Hex::MemoryFormatter_Bit_8_Hex(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_8(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_8_Hex::string() const {
    std::ostringstream result;
    result << std::hex << static_cast<std::size_t>(get_data());
    return result.str();
}

MemoryFormatter_Bit_8_Oct::MemoryFormatter_Bit_8_Oct(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_8(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_8_Oct::string() const {
    std::ostringstream result;
    result << std::oct << static_cast<std::size_t>(get_data());
    return result.str();
}

MemoryFormatter_Bit_8_Signed::MemoryFormatter_Bit_8_Signed(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_8(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_8_Signed::string() const {
    auto value = get_data();
    return std::to_string(static_cast<int>(*reinterpret_cast<int8_t *>(&value)));
}

MemoryFormatter_Bit_8_Unsigned::MemoryFormatter_Bit_8_Unsigned(void       *base_address,
                                                               std::size_t offset,
                                                               endianness  endian)
    : MemoryFormatter_Bit_8(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_8_Unsigned::string() const { return std::to_string(static_cast<unsigned>(get_data())); }

MemoryFormatter_Bit_16_Bin::MemoryFormatter_Bit_16_Bin(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_16(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_16_Bin::string() const {
    std::ostringstream result;
    result << std::bitset<16>(get_data());
    return result.str();
}

MemoryFormatter_Bit_16_Hex::MemoryFormatter_Bit_16_Hex(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_16(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_16_Hex::string() const {
    std::ostringstream result;
    result << std::hex << get_data();
    return result.str();
}

MemoryFormatter_Bit_16_Oct::MemoryFormatter_Bit_16_Oct(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_16(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_16_Oct::string() const {
    std::ostringstream result;
    result << std::oct << get_data();
    return result.str();
}

MemoryFormatter_Bit_16_Signed::MemoryFormatter_Bit_16_Signed(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_16(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_16_Signed::string() const {
    auto value = get_data();
    return std::to_string(*reinterpret_cast<int16_t *>(&value));
}

MemoryFormatter_Bit_16_Unsigned::MemoryFormatter_Bit_16_Unsigned(void       *base_address,
                                                                 std::size_t offset,
                                                                 endianness  endian)
    : MemoryFormatter_Bit_16(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_16_Unsigned::string() const { return std::to_string(get_data()); }

MemoryFormatter_Bit_32_Bin::MemoryFormatter_Bit_32_Bin(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_32(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_32_Bin::string() const {
    std::ostringstream result;
    result << std::bitset<32>(get_data());
    return result.str();
}

MemoryFormatter_Bit_32_Hex::MemoryFormatter_Bit_32_Hex(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_32(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_32_Hex::string() const {
    std::ostringstream result;
    result << std::hex << get_data();
    return result.str();
}

MemoryFormatter_Bit_32_Oct::MemoryFormatter_Bit_32_Oct(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_32(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_32_Oct::string() const {
    std::ostringstream result;
    result << std::oct << get_data();
    return result.str();
}

MemoryFormatter_Bit_32_Signed::MemoryFormatter_Bit_32_Signed(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_32(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_32_Signed::string() const {
    auto value = get_data();
    return std::to_string(*reinterpret_cast<int32_t *>(&value));
}

MemoryFormatter_Bit_32_Unsigned::MemoryFormatter_Bit_32_Unsigned(void       *base_address,
                                                                 std::size_t offset,
                                                                 endianness  endian)
    : MemoryFormatter_Bit_32(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_32_Unsigned::string() const { return std::to_string(get_data()); }

MemoryFormatter_Bit_32_Float::MemoryFormatter_Bit_32_Float(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_32(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_32_Float::string() const {
    auto value    = get_data();
    auto void_ptr = reinterpret_cast<void *>(&value);
    return std::to_string(*reinterpret_cast<float *>(void_ptr));
}

MemoryFormatter_Bit_64_Bin::MemoryFormatter_Bit_64_Bin(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_64(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_64_Bin::string() const {
    std::ostringstream result;
    result << std::bitset<64>(get_data());
    return result.str();
}

MemoryFormatter_Bit_64_Hex::MemoryFormatter_Bit_64_Hex(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_64(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_64_Hex::string() const {
    std::ostringstream result;
    result << std::hex << get_data();
    return result.str();
}

MemoryFormatter_Bit_64_Oct::MemoryFormatter_Bit_64_Oct(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_64(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_64_Oct::string() const {
    std::ostringstream result;
    result << std::oct << get_data();
    return result.str();
}

MemoryFormatter_Bit_64_Signed::MemoryFormatter_Bit_64_Signed(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_64(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_64_Signed::string() const {
    auto value = get_data();
    return std::to_string(*reinterpret_cast<int64_t *>(&value));
}

MemoryFormatter_Bit_64_Unsigned::MemoryFormatter_Bit_64_Unsigned(void       *base_address,
                                                                 std::size_t offset,
                                                                 endianness  endian)
    : MemoryFormatter_Bit_64(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_64_Unsigned::string() const { return std::to_string(get_data()); }

MemoryFormatter_Bit_64_Float::MemoryFormatter_Bit_64_Float(void *base_address, std::size_t offset, endianness endian)
    : MemoryFormatter_Bit_64(base_address, offset, endian) {}

std::string MemoryFormatter_Bit_64_Float::string() const {
    auto value    = get_data();
    auto void_ptr = reinterpret_cast<void *>(&value);
    auto d        = *reinterpret_cast<double *>(void_ptr);
    return std::to_string(d);
}

/**
 * @brief get 8 bit formatter
 * @param base_addr memory base address
 * @param offset memory offset (bytes)
 * @param e endianness
 * @param f format
 * @return shared pointer that holds a MemoryFormatter instance
 */
static std::shared_ptr<MemoryFormatter> get_formatter_8(void *base_addr, std::size_t offset, endianness e, format f) {
    switch (f) {
        case format::BIN: return std::make_shared<MemoryFormatter_Bit_8_Bin>(base_addr, offset, e);
        case format::HEX: return std::make_shared<MemoryFormatter_Bit_8_Hex>(base_addr, offset, e);
        case format::OCT: return std::make_shared<MemoryFormatter_Bit_8_Oct>(base_addr, offset, e);
        case format::SIGNED: return std::make_shared<MemoryFormatter_Bit_8_Signed>(base_addr, offset, e);
        case format::UNSIGNED: return std::make_shared<MemoryFormatter_Bit_8_Unsigned>(base_addr, offset, e);
        case format::FLOAT: throw std::invalid_argument("Format FLOAT is not allowed for 8 bit values");
    }
}

/**
 * @brief get 16 bit formatter
 * @param base_addr memory base address
 * @param offset memory offset (bytes)
 * @param e endianness
 * @param f format
 * @return shared pointer that holds a MemoryFormatter instance
 */
static std::shared_ptr<MemoryFormatter> get_formatter_16(void *base_addr, std::size_t offset, endianness e, format f) {
    switch (f) {
        case format::BIN: return std::make_shared<MemoryFormatter_Bit_16_Bin>(base_addr, offset, e);
        case format::HEX: return std::make_shared<MemoryFormatter_Bit_16_Hex>(base_addr, offset, e);
        case format::OCT: return std::make_shared<MemoryFormatter_Bit_16_Oct>(base_addr, offset, e);
        case format::SIGNED: return std::make_shared<MemoryFormatter_Bit_16_Signed>(base_addr, offset, e);
        case format::UNSIGNED: return std::make_shared<MemoryFormatter_Bit_16_Unsigned>(base_addr, offset, e);
        case format::FLOAT: throw std::invalid_argument("Format FLOAT is not allowed for 16 bit values");
    }
}

/**
 * @brief get 32 bit formatter
 * @param base_addr memory base address
 * @param offset memory offset (bytes)
 * @param e endianness
 * @param f format
 * @return shared pointer that holds a MemoryFormatter instance
 */
static std::shared_ptr<MemoryFormatter> get_formatter_32(void *base_addr, std::size_t offset, endianness e, format f) {
    switch (f) {
        case format::BIN: return std::make_shared<MemoryFormatter_Bit_32_Bin>(base_addr, offset, e);
        case format::HEX: return std::make_shared<MemoryFormatter_Bit_32_Hex>(base_addr, offset, e);
        case format::OCT: return std::make_shared<MemoryFormatter_Bit_32_Oct>(base_addr, offset, e);
        case format::SIGNED: return std::make_shared<MemoryFormatter_Bit_32_Signed>(base_addr, offset, e);
        case format::UNSIGNED: return std::make_shared<MemoryFormatter_Bit_32_Unsigned>(base_addr, offset, e);
        case format::FLOAT: return std::make_shared<MemoryFormatter_Bit_32_Float>(base_addr, offset, e);
    }
}

/**
 * @brief get 64 bit formatter
 * @param base_addr memory base address
 * @param offset memory offset (bytes)
 * @param e endianness
 * @param f format
 * @return shared pointer that holds a MemoryFormatter instance
 */
static std::shared_ptr<MemoryFormatter> get_formatter_64(void *base_addr, std::size_t offset, endianness e, format f) {
    switch (f) {
        case format::BIN: return std::make_shared<MemoryFormatter_Bit_64_Bin>(base_addr, offset, e);
        case format::HEX: return std::make_shared<MemoryFormatter_Bit_64_Hex>(base_addr, offset, e);
        case format::OCT: return std::make_shared<MemoryFormatter_Bit_64_Oct>(base_addr, offset, e);
        case format::SIGNED: return std::make_shared<MemoryFormatter_Bit_64_Signed>(base_addr, offset, e);
        case format::UNSIGNED: return std::make_shared<MemoryFormatter_Bit_64_Unsigned>(base_addr, offset, e);
        case format::FLOAT: return std::make_shared<MemoryFormatter_Bit_64_Float>(base_addr, offset, e);
    }
}

/**
 * @brief convert string to memory address offset
 * @param addr_string address string
 * @return address offset
 *
 * @exception: std::invalid_argument failed to parse string as memory offset
 */
static std::size_t get_address_from_string(const std::string &addr_string) {
    std::size_t addr_offset;
    std::size_t idx  = 0;
    bool        fail = false;
    try {
        addr_offset = std::stoull(addr_string, &idx, 0);
    } catch (const std::exception &) { fail = true; }

    if (fail || idx != addr_string.size()) {
        std::ostringstream error_msg;
        error_msg << "Failed to parse '" << addr_string << "' as address offset";
        throw std::invalid_argument(error_msg.str());
    }

    return addr_offset;
}

std::shared_ptr<MemoryFormatter> MemoryFormatter::get_formatter(
        void *base_addr, const std::string &addr_string, wordsize w, format f, endianness e) {
    switch (w) {
        case wordsize::BIT_1: {
            const auto split_address = split_string(addr_string, '.', 1);

            if (split_address.size() != 2) {
                std::ostringstream error_msg;
                error_msg << "Invalid address string '" << addr_string
                          << ": expected format \"<address_offset>.<bit_index>\"";
                throw std::invalid_argument(error_msg.str());
            }

            const auto &address_offset_str = split_address[0];
            const auto &bit_index_str      = split_address[1];

            std::size_t addr_offset;
            std::size_t idx  = 0;
            bool        fail = false;
            try {
                addr_offset = std::stoull(address_offset_str, &idx, 0);
            } catch (const std::exception &) { fail = true; }

            if (fail || idx != address_offset_str.size()) {
                std::ostringstream error_msg;
                error_msg << "Failed to parse '" << address_offset_str << "' as address offset";
                throw std::invalid_argument(error_msg.str());
            }

            std::size_t bit_offset;
            idx  = 0;
            fail = false;

            try {
                bit_offset = std::stoull(bit_index_str, &idx, 0);
            } catch (const std::exception &) { fail = true; }

            if (fail || idx != bit_index_str.size()) {
                std::ostringstream error_msg;
                error_msg << "Failed to parse '" << bit_index_str << "' as bit index";
                throw std::invalid_argument(error_msg.str());
            }

            if (bit_offset > 7) throw std::out_of_range("bit index out of range (0..7)");

            return std::make_shared<MemoryFormatter_Bit_1>(base_addr, addr_offset, bit_offset);
        }
        case wordsize::BIT_8: return get_formatter_8(base_addr, get_address_from_string(addr_string), e, f);
        case wordsize::BIT_16: return get_formatter_16(base_addr, get_address_from_string(addr_string), e, f);
        case wordsize::BIT_32: return get_formatter_32(base_addr, get_address_from_string(addr_string), e, f);
        case wordsize::BIT_64: return get_formatter_64(base_addr, get_address_from_string(addr_string), e, f);
    }
}

}  // namespace memformat

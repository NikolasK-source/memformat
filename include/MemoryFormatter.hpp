/*
 * Copyright (C) 2023 Nikolas Koesling <nikolas@koesling.info>.
 * This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
 */

#pragma once

#include <cstddef>
#include <memory>
#include <string>

namespace memformat {

/**
 * @brief endianness
 */
enum class endianness : std::size_t {
    HOST,    //*< use host endianness
    BIG,     //*< big endian (abcd...)
    LITTLE,  //*< little endian (...dcba)

    /**
     * @brief big endian, but every two subsequent bytes are swapped (abcd -> cdab)
     * @details only allowed for word size >= 32 bit
     */
    BIG_SWAP16,

    /**
     * @brief little endian, but every two subsequent bytes are swapped (dcba -> badc)
     * @details only allowed for word size >= 32 bit
     */
    LITTLE_SWAP16,

    /**
     * @brief big endian, but every four subsequent bytes are swapped (abcdefgh - > efghabcd)
     * @details only allowed for word size >= 64 bit
     */
    BIG_SWAP32,

    /**
     * @brief little endian, but every four subsequent bytes are swapped (hgfedcba -> dcbahgfe)
     * @details only allowed for word size >= 64 bit
     */
    LITTLE_SWAP32,
};

/**
 * @brief output format
 */
enum class format : std::size_t {
    BIN,       //*< binary
    OCT,       //*< octal
    HEX,       //*< hexadecimal
    SIGNED,    //*< signed decimal
    UNSIGNED,  //*< unsigned decimal
    FLOAT,     //*< floating point (only allowed for 32 and 64 bit word size)
};

/**
 * @brief word size
 */
enum class wordsize : std::size_t {
    BIT_1,   //*< 1 bit
    BIT_8,   //*< 8 bit
    BIT_16,  //*< 16 bit
    BIT_32,  //*< 32 bit
    BIT_64,  //*< 64 bit
};


/**
 * @brief abstract memory formatter class
 */
class MemoryFormatter {
protected:
    volatile void *const base_address;  //*< base memory address
    const std::size_t    offset;        //*< memory offset

    /**
     * @brief construct MemoryFormatter
     * @param base_address base memory address
     * @param offset memory offset
     */
    MemoryFormatter(volatile void *base_address, std::size_t offset) : base_address(base_address), offset(offset) {}

public:
    MemoryFormatter(const MemoryFormatter &)            = delete;
    MemoryFormatter(MemoryFormatter &&)                 = delete;
    MemoryFormatter &operator=(const MemoryFormatter &) = delete;
    MemoryFormatter &operator=(MemoryFormatter &&)      = delete;

    virtual ~MemoryFormatter() = default;

    /**
     * @brief format memory
     * @return formatted memory value as std::string
     */
    [[nodiscard, maybe_unused]] virtual std::string string() const = 0;

    /**
     * @brief get max memory offset that is read by a formatter
     * @return max memory offset
     */
    [[nodiscard]] virtual std::size_t max_offset() const = 0;

    /**
     * @brief get memory formatter instance
     * @param base_addr memory base address
     * @param addr_string string that is parsed as address
     *      word size 1: "<memory offset>.<bit index>" (regex: "^(0x)?[0-9]+\.[0-7]$")
     *      other word sizes: "<memory offset>" (regex: "^(0x)?[0-9]+$")
     * @param w word size \see memformat::wordsize
     * @param e endianness \see memformat::endianness
     * @param f format \see memformat::format
     *      value is ignored if wordsize is BIT_1
     * @return std::shared_pointer that holds an MemoryFormatter instance
     *
     * @exception std::invalid_argument: address string is invalid
     * @exception std::out_of_range: bit index out of range (only relevant for w == BIT_1)
     */
    [[nodiscard]] static std::shared_ptr<MemoryFormatter> get_formatter(void              *base_addr,
                                                                        const std::string &addr_string,
                                                                        wordsize           w,
                                                                        format             f = format::BIN,
                                                                        endianness         e = endianness::HOST);
};

}  // namespace memformat

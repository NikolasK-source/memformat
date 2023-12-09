/*
 * Copyright (C) 2023 Nikolas Koesling <nikolas@koesling.info>.
 * This program is free software. You can redistribute it and/or modify it under the terms of the MIT License.
 */

#pragma once

#include "MemoryFormatter.hpp"
#include <functional>

namespace memformat {

/**
 * @brief 1 Bit memory formatter
 */
class MemoryFormatter_Bit_1 : public MemoryFormatter {
protected:
    //* bit index [0..7]
    const std::size_t bit_offset;

public:
    MemoryFormatter_Bit_1(void *base_address, std::size_t offset, std::size_t bit_offset);

    [[nodiscard]] std::string string() const override;
    [[nodiscard]] std::size_t max_offset() const override;
};

/**
 * @brief abstract 8 Bit memory formatter
 */
class MemoryFormatter_Bit_8 : public MemoryFormatter {
protected:
    MemoryFormatter_Bit_8(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] uint8_t get_data() const;

public:
    [[nodiscard]] std::size_t max_offset() const override;
};


/**
 * @brief abstract 16 Bit memory formatter
 */
class MemoryFormatter_Bit_16 : public MemoryFormatter {
protected:
    //* function that handles the endianness conversion (set by constructor depending on value of endian)
    std::function<uint16_t(uint16_t)> convert_endianess;

    MemoryFormatter_Bit_16(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] uint16_t get_data() const;

public:
    [[nodiscard]] std::size_t max_offset() const override;
};


/**
 * @brief abstract 32 Bit memory formatter
 */
class MemoryFormatter_Bit_32 : public MemoryFormatter {
protected:
    //* function that handles the endianness conversion (set by constructor depending on value of endian)
    std::function<uint32_t(uint32_t)> convert_endianess;

    MemoryFormatter_Bit_32(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] uint32_t get_data() const;

public:
    [[nodiscard]] std::size_t max_offset() const override;
};


/**
 * @brief abstract 64 Bit memory formatter
 */
class MemoryFormatter_Bit_64 : public MemoryFormatter {
protected:
    //* function that handles the endianness conversion (set by constructor depending on value of endian)
    std::function<uint64_t(uint64_t)> convert_endianess;

    MemoryFormatter_Bit_64(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] uint64_t get_data() const;

public:
    [[nodiscard]] std::size_t max_offset() const override;
};


/**
 * @brief 8 bit binary memory formatter
 */
class MemoryFormatter_Bit_8_Bin : public MemoryFormatter_Bit_8 {
public:
    MemoryFormatter_Bit_8_Bin(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 8 bit hex memory formatter
 */
class MemoryFormatter_Bit_8_Hex : public MemoryFormatter_Bit_8 {
public:
    MemoryFormatter_Bit_8_Hex(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 8 bit oct memory formatter
 */
class MemoryFormatter_Bit_8_Oct : public MemoryFormatter_Bit_8 {
public:
    MemoryFormatter_Bit_8_Oct(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 8 bit signed memory formatter
 */
class MemoryFormatter_Bit_8_Signed : public MemoryFormatter_Bit_8 {
public:
    MemoryFormatter_Bit_8_Signed(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 8 bit unsigned memory formatter
 */
class MemoryFormatter_Bit_8_Unsigned : public MemoryFormatter_Bit_8 {
public:
    MemoryFormatter_Bit_8_Unsigned(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 16 bit binary memory formatter
 */
class MemoryFormatter_Bit_16_Bin : public MemoryFormatter_Bit_16 {
public:
    MemoryFormatter_Bit_16_Bin(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 16 bit hex memory formatter
 */
class MemoryFormatter_Bit_16_Hex : public MemoryFormatter_Bit_16 {
public:
    MemoryFormatter_Bit_16_Hex(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 16 bit oct memory formatter
 */
class MemoryFormatter_Bit_16_Oct : public MemoryFormatter_Bit_16 {
public:
    MemoryFormatter_Bit_16_Oct(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 16 bit signed memory formatter
 */
class MemoryFormatter_Bit_16_Signed : public MemoryFormatter_Bit_16 {
public:
    MemoryFormatter_Bit_16_Signed(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 16 bit unsigned memory formatter
 */
class MemoryFormatter_Bit_16_Unsigned : public MemoryFormatter_Bit_16 {
public:
    MemoryFormatter_Bit_16_Unsigned(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 32 bit binary memory formatter
 */
class MemoryFormatter_Bit_32_Bin : public MemoryFormatter_Bit_32 {
public:
    MemoryFormatter_Bit_32_Bin(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 32 bit hex memory formatter
 */
class MemoryFormatter_Bit_32_Hex : public MemoryFormatter_Bit_32 {
public:
    MemoryFormatter_Bit_32_Hex(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 32 bit oct memory formatter
 */
class MemoryFormatter_Bit_32_Oct : public MemoryFormatter_Bit_32 {
public:
    MemoryFormatter_Bit_32_Oct(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 32 bit signed memory formatter
 */
class MemoryFormatter_Bit_32_Signed : public MemoryFormatter_Bit_32 {
public:
    MemoryFormatter_Bit_32_Signed(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 32 bit unsigned memory formatter
 */
class MemoryFormatter_Bit_32_Unsigned : public MemoryFormatter_Bit_32 {
public:
    MemoryFormatter_Bit_32_Unsigned(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 32 bit float memory formatter
 */
class MemoryFormatter_Bit_32_Float : public MemoryFormatter_Bit_32 {
public:
    MemoryFormatter_Bit_32_Float(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 64 bit binary memory formatter
 */
class MemoryFormatter_Bit_64_Bin : public MemoryFormatter_Bit_64 {
public:
    MemoryFormatter_Bit_64_Bin(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 64 bit hex memory formatter
 */
class MemoryFormatter_Bit_64_Hex : public MemoryFormatter_Bit_64 {
public:
    MemoryFormatter_Bit_64_Hex(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 64 bit oct memory formatter
 */
class MemoryFormatter_Bit_64_Oct : public MemoryFormatter_Bit_64 {
public:
    MemoryFormatter_Bit_64_Oct(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 64 bit signed memory formatter
 */
class MemoryFormatter_Bit_64_Signed : public MemoryFormatter_Bit_64 {
public:
    MemoryFormatter_Bit_64_Signed(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 64 bit unsigned memory formatter
 */
class MemoryFormatter_Bit_64_Unsigned : public MemoryFormatter_Bit_64 {
public:
    MemoryFormatter_Bit_64_Unsigned(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

/**
 * @brief 64 bit float memory formatter
 */
class MemoryFormatter_Bit_64_Float : public MemoryFormatter_Bit_64 {
public:
    MemoryFormatter_Bit_64_Float(void *base_address, std::size_t offset, endianness endian);

    [[nodiscard]] std::string string() const override;
};

}  // namespace memformat

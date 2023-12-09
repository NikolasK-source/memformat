# Memory Format library

Formatter class to get the value of memory as string.

## Data types
 - 1 bit
 - 8 bit
 - 16 bit
 - 32 bit
 - 64 bit

## Formats
 - Binary
 - Signed decimal
 - Unsigned decimal
 - Hexadecimal
 - Octal
 - Float

## Endianness
 - Host
 - Big
 - Little

In addition, the swapping of 16 and 32 bit blocks is supported

## Example

```
// pointer to memory area
void *data = ...;

// 32 bit as hex value at offset 0x42
auto formatter_hex32 = memformat::MemoryFormatter::get_formatter(
                       data, "0x42", memformat::wordsize::BIT_32, memformat::format::HEX);
std::cout << formatter_hex32->string() << std::endl;

// 5th bit of byte at offset 42
auto formatter_bit = memformat::MemoryFormatter::get_formatter(data, "42.5", memformat::wordsize::BIT_1);
std::cout << formatter_bit->string() << std::endl;
```
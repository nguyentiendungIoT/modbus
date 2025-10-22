/**
 * @file modbus_common.c
 * @brief Common Modbus utility functions
 * @author Nguyen Tien Dung
 * @date 2025
 */

#include "modbus_common.h"

/**
 * @brief Convert byte order (big-endian to little-endian and vice versa)
 */
uint16_t modbus_swap_bytes(uint16_t value)
{
    return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF);
}

/**
 * @brief Set bit in byte array
 */
void modbus_set_bit(uint8_t *byte_array, uint16_t bit_index, uint8_t value)
{
    uint16_t byte_index = bit_index / 8;
    uint8_t bit_pos = bit_index % 8;
    
    if (value) {
        byte_array[byte_index] |= (1 << bit_pos);
    } else {
        byte_array[byte_index] &= ~(1 << bit_pos);
    }
}

/**
 * @brief Get bit from byte array
 */
uint8_t modbus_get_bit(const uint8_t *byte_array, uint16_t bit_index)
{
    uint16_t byte_index = bit_index / 8;
    uint8_t bit_pos = bit_index % 8;
    
    return (byte_array[byte_index] >> bit_pos) & 0x01;
}

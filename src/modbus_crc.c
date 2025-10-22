/**
 * @file modbus_crc.c
 * @brief CRC16 calculation for Modbus RTU
 * @author Nguyen Tien Dung
 * @date 2025
 */

#include "modbus_common.h"

/**
 * @brief Calculate CRC16 for Modbus RTU
 * @param buffer Data buffer
 * @param length Buffer length
 * @return CRC16 value
 */
uint16_t modbus_crc16(const uint8_t *buffer, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    uint16_t i, j;
    
    for (i = 0; i < length; i++) {
        crc ^= buffer[i];
        
        for (j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}

/**
 * @file modbus_common.h
 * @brief Common definitions and structures for Modbus protocol
 * @author Nguyen Tien Dung
 * @date 2025
 */

#ifndef MODBUS_COMMON_H
#define MODBUS_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include "modbus_config.h"

/* Modbus function codes */
#define MODBUS_FC_READ_COILS                    0x01
#define MODBUS_FC_READ_DISCRETE_INPUTS          0x02
#define MODBUS_FC_READ_HOLDING_REGISTERS        0x03
#define MODBUS_FC_READ_INPUT_REGISTERS          0x04
#define MODBUS_FC_WRITE_SINGLE_COIL             0x05
#define MODBUS_FC_WRITE_SINGLE_REGISTER         0x06
#define MODBUS_FC_WRITE_MULTIPLE_COILS          0x0F
#define MODBUS_FC_WRITE_MULTIPLE_REGISTERS      0x10

/* Modbus exception codes */
#define MODBUS_EXCEPTION_ILLEGAL_FUNCTION       0x01
#define MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS   0x02
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE     0x03
#define MODBUS_EXCEPTION_SLAVE_DEVICE_FAILURE   0x04
#define MODBUS_EXCEPTION_ACKNOWLEDGE            0x05
#define MODBUS_EXCEPTION_SLAVE_DEVICE_BUSY      0x06
#define MODBUS_EXCEPTION_MEMORY_PARITY_ERROR    0x08
#define MODBUS_EXCEPTION_GATEWAY_PATH           0x0A
#define MODBUS_EXCEPTION_GATEWAY_TARGET         0x0B

/* Modbus error codes */
typedef enum {
    MODBUS_OK = 0,
    MODBUS_ERROR_TIMEOUT,
    MODBUS_ERROR_CRC,
    MODBUS_ERROR_INVALID_SLAVE,
    MODBUS_ERROR_INVALID_FUNCTION,
    MODBUS_ERROR_INVALID_DATA,
    MODBUS_ERROR_EXCEPTION,
    MODBUS_ERROR_BUSY,
    MODBUS_ERROR_INVALID_PARAM,
    MODBUS_ERROR_NOT_INITIALIZED
} modbus_error_t;

/* Modbus parity options */
typedef enum {
    MODBUS_PARITY_NONE = 0,
    MODBUS_PARITY_EVEN,
    MODBUS_PARITY_ODD
} modbus_parity_t;

/* Modbus mode */
typedef enum {
    MODBUS_MODE_MASTER = 0,
    MODBUS_MODE_SLAVE
} modbus_mode_t;

/* Modbus data model */
typedef struct {
    uint8_t *coils;                 /* Coil status (read/write) */
    uint16_t coils_count;
    
    uint8_t *discrete_inputs;       /* Discrete input status (read only) */
    uint16_t discrete_inputs_count;
    
    uint16_t *holding_registers;    /* Holding registers (read/write) */
    uint16_t holding_registers_count;
    
    uint16_t *input_registers;      /* Input registers (read only) */
    uint16_t input_registers_count;
} modbus_data_model_t;

/* Function prototypes */

/**
 * @brief Calculate CRC16 for Modbus RTU
 * @param buffer Data buffer
 * @param length Buffer length
 * @return CRC16 value
 */
uint16_t modbus_crc16(const uint8_t *buffer, uint16_t length);

/**
 * @brief Convert byte order (big-endian to little-endian and vice versa)
 * @param value Input value
 * @return Converted value
 */
uint16_t modbus_swap_bytes(uint16_t value);

/**
 * @brief Set bit in byte array
 * @param byte_array Byte array
 * @param bit_index Bit index
 * @param value Bit value (0 or 1)
 */
void modbus_set_bit(uint8_t *byte_array, uint16_t bit_index, uint8_t value);

/**
 * @brief Get bit from byte array
 * @param byte_array Byte array
 * @param bit_index Bit index
 * @return Bit value (0 or 1)
 */
uint8_t modbus_get_bit(const uint8_t *byte_array, uint16_t bit_index);

#endif /* MODBUS_COMMON_H */

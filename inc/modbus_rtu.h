/**
 * @file modbus_rtu.h
 * @brief Modbus RTU protocol implementation for STM32
 * @author Nguyen Tien Dung
 * @date 2025
 */

#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include "modbus_common.h"
#include "stm32f4xx_hal.h"  /* Change to your STM32 series */

/* Modbus RTU structure */
typedef struct {
    UART_HandleTypeDef *huart;      /* UART handle */
    uint8_t slave_id;                /* Slave ID (1-247, 0=broadcast) */
    modbus_mode_t mode;              /* Master or Slave mode */
    modbus_parity_t parity;          /* Parity configuration */
    uint32_t baudrate;               /* Baud rate */
    
    uint8_t tx_buffer[MODBUS_RTU_MAX_ADU_LENGTH];
    uint8_t rx_buffer[MODBUS_RTU_MAX_ADU_LENGTH];
    uint16_t rx_index;
    uint16_t tx_length;
    
    uint32_t last_rx_time;           /* Last receive timestamp */
    bool frame_ready;                /* Frame ready flag */
    
    modbus_data_model_t data_model;  /* Data model for slave */
    
    GPIO_TypeDef *de_port;           /* DE/RE control port (optional) */
    uint16_t de_pin;                 /* DE/RE control pin (optional) */
} modbus_rtu_t;

/* Function prototypes */

/**
 * @brief Initialize Modbus RTU
 * @param mb Modbus RTU handle
 * @param huart UART handle
 * @param slave_id Slave ID
 * @param baudrate Baud rate
 * @param parity Parity setting
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_init(modbus_rtu_t *mb, UART_HandleTypeDef *huart,
                               uint8_t slave_id, uint32_t baudrate,
                               modbus_parity_t parity);

/**
 * @brief Set DE/RE control pin for RS-485
 * @param mb Modbus RTU handle
 * @param port GPIO port
 * @param pin GPIO pin
 */
void modbus_rtu_set_de_pin(modbus_rtu_t *mb, GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief Set Modbus mode (Master/Slave)
 * @param mb Modbus RTU handle
 * @param mode Mode setting
 */
void modbus_rtu_set_mode(modbus_rtu_t *mb, modbus_mode_t mode);

/**
 * @brief Register data model for slave mode
 * @param mb Modbus RTU handle
 * @param data_model Data model structure
 */
void modbus_rtu_register_data_model(modbus_rtu_t *mb, modbus_data_model_t *data_model);

/**
 * @brief Poll for incoming frames (call in main loop for slave)
 * @param mb Modbus RTU handle
 */
void modbus_rtu_poll(modbus_rtu_t *mb);

/**
 * @brief UART receive complete callback
 * @param mb Modbus RTU handle
 */
void modbus_rtu_rx_callback(modbus_rtu_t *mb);

/* Master functions */

/**
 * @brief Read holding registers
 * @param mb Modbus RTU handle
 * @param slave_id Slave address
 * @param start_addr Starting address
 * @param count Number of registers
 * @param dest Destination buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_read_holding_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                                  uint16_t start_addr, uint16_t count,
                                                  uint16_t *dest);

/**
 * @brief Read input registers
 * @param mb Modbus RTU handle
 * @param slave_id Slave address
 * @param start_addr Starting address
 * @param count Number of registers
 * @param dest Destination buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_read_input_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                                uint16_t start_addr, uint16_t count,
                                                uint16_t *dest);

/**
 * @brief Read coils
 * @param mb Modbus RTU handle
 * @param slave_id Slave address
 * @param start_addr Starting address
 * @param count Number of coils
 * @param dest Destination buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_read_coils(modbus_rtu_t *mb, uint8_t slave_id,
                                     uint16_t start_addr, uint16_t count,
                                     uint8_t *dest);

/**
 * @brief Write single register
 * @param mb Modbus RTU handle
 * @param slave_id Slave address
 * @param address Register address
 * @param value Register value
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_write_single_register(modbus_rtu_t *mb, uint8_t slave_id,
                                                 uint16_t address, uint16_t value);

/**
 * @brief Write multiple registers
 * @param mb Modbus RTU handle
 * @param slave_id Slave address
 * @param start_addr Starting address
 * @param count Number of registers
 * @param src Source buffer
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_write_multiple_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                                    uint16_t start_addr, uint16_t count,
                                                    const uint16_t *src);

/**
 * @brief Write single coil
 * @param mb Modbus RTU handle
 * @param slave_id Slave address
 * @param address Coil address
 * @param value Coil value (0x0000 or 0xFF00)
 * @return MODBUS_OK on success
 */
modbus_error_t modbus_rtu_write_single_coil(modbus_rtu_t *mb, uint8_t slave_id,
                                            uint16_t address, uint16_t value);

#endif /* MODBUS_RTU_H */

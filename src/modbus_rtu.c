/**
 * @file modbus_rtu.c
 * @brief Modbus RTU protocol implementation
 * @author Nguyen Tien Dung
 * @date 2025
 */

#include "modbus_rtu.h"
#include <string.h>

/* Private function prototypes */
static void modbus_rtu_enable_transmit(modbus_rtu_t *mb);
static void modbus_rtu_enable_receive(modbus_rtu_t *mb);
static modbus_error_t modbus_rtu_send_frame(modbus_rtu_t *mb, uint16_t length);
static modbus_error_t modbus_rtu_receive_frame(modbus_rtu_t *mb, uint32_t timeout_ms);
static void modbus_rtu_process_frame(modbus_rtu_t *mb);

/**
 * @brief Initialize Modbus RTU
 */
modbus_error_t modbus_rtu_init(modbus_rtu_t *mb, UART_HandleTypeDef *huart,
                               uint8_t slave_id, uint32_t baudrate,
                               modbus_parity_t parity)
{
    if (!mb || !huart) {
        return MODBUS_ERROR_INVALID_PARAM;
    }
    
    memset(mb, 0, sizeof(modbus_rtu_t));
    
    mb->huart = huart;
    mb->slave_id = slave_id;
    mb->baudrate = baudrate;
    mb->parity = parity;
    mb->mode = MODBUS_MODE_SLAVE;
    mb->frame_ready = false;
    mb->rx_index = 0;
    
    /* Start UART reception */
    HAL_UART_Receive_IT(mb->huart, &mb->rx_buffer[mb->rx_index], 1);
    
    return MODBUS_OK;
}

/**
 * @brief Set DE/RE control pin for RS-485
 */
void modbus_rtu_set_de_pin(modbus_rtu_t *mb, GPIO_TypeDef *port, uint16_t pin)
{
    mb->de_port = port;
    mb->de_pin = pin;
    
    /* Initialize DE pin to receive mode */
    modbus_rtu_enable_receive(mb);
}

/**
 * @brief Set Modbus mode
 */
void modbus_rtu_set_mode(modbus_rtu_t *mb, modbus_mode_t mode)
{
    mb->mode = mode;
}

/**
 * @brief Register data model for slave mode
 */
void modbus_rtu_register_data_model(modbus_rtu_t *mb, modbus_data_model_t *data_model)
{
    memcpy(&mb->data_model, data_model, sizeof(modbus_data_model_t));
}

/**
 * @brief Enable RS-485 transmit mode
 */
static void modbus_rtu_enable_transmit(modbus_rtu_t *mb)
{
    if (mb->de_port) {
        HAL_GPIO_WritePin(mb->de_port, mb->de_pin, GPIO_PIN_SET);
    }
}

/**
 * @brief Enable RS-485 receive mode
 */
static void modbus_rtu_enable_receive(modbus_rtu_t *mb)
{
    if (mb->de_port) {
        HAL_GPIO_WritePin(mb->de_port, mb->de_pin, GPIO_PIN_RESET);
    }
}

/**
 * @brief Send Modbus frame
 */
static modbus_error_t modbus_rtu_send_frame(modbus_rtu_t *mb, uint16_t length)
{
    uint16_t crc;
    
    /* Calculate and append CRC */
    crc = modbus_crc16(mb->tx_buffer, length);
    mb->tx_buffer[length] = crc & 0xFF;
    mb->tx_buffer[length + 1] = (crc >> 8) & 0xFF;
    
    mb->tx_length = length + 2;
    
    /* Enable transmit mode */
    modbus_rtu_enable_transmit(mb);
    
    /* Send frame */
    HAL_StatusTypeDef status = HAL_UART_Transmit(mb->huart, mb->tx_buffer, mb->tx_length, 100);
    
    /* Wait for transmission complete */
    HAL_Delay(2);
    
    /* Enable receive mode */
    modbus_rtu_enable_receive(mb);
    
    return (status == HAL_OK) ? MODBUS_OK : MODBUS_ERROR_TIMEOUT;
}

/**
 * @brief Receive Modbus frame
 */
static modbus_error_t modbus_rtu_receive_frame(modbus_rtu_t *mb, uint32_t timeout_ms)
{
    uint32_t start_time = HAL_GetTick();
    
    mb->frame_ready = false;
    
    while (!mb->frame_ready) {
        if ((HAL_GetTick() - start_time) > timeout_ms) {
            return MODBUS_ERROR_TIMEOUT;
        }
    }
    
    return MODBUS_OK;
}

/**
 * @brief UART receive complete callback
 */
void modbus_rtu_rx_callback(modbus_rtu_t *mb)
{
    mb->rx_index++;
    mb->last_rx_time = HAL_GetTick();
    
    /* Continue receiving */
    if (mb->rx_index < MODBUS_RTU_MAX_ADU_LENGTH) {
        HAL_UART_Receive_IT(mb->huart, &mb->rx_buffer[mb->rx_index], 1);
    }
}

/**
 * @brief Poll for incoming frames
 */
void modbus_rtu_poll(modbus_rtu_t *mb)
{
    /* Check for frame timeout (inter-frame delay) */
    if (mb->rx_index > 0) {
        if ((HAL_GetTick() - mb->last_rx_time) >= MODBUS_INTER_FRAME_DELAY_MS) {
            mb->frame_ready = true;
            
            /* Process the frame if in slave mode */
            if (mb->mode == MODBUS_MODE_SLAVE) {
                modbus_rtu_process_frame(mb);
            }
            
            /* Reset for next frame */
            mb->rx_index = 0;
            HAL_UART_Receive_IT(mb->huart, &mb->rx_buffer[0], 1);
        }
    }
}

/**
 * @brief Process received frame (slave mode)
 */
static void modbus_rtu_process_frame(modbus_rtu_t *mb)
{
    uint16_t crc_received, crc_calculated;
    uint8_t slave_id, function_code;
    uint16_t start_addr, count;
    uint16_t i;
    
    /* Minimum frame size check */
    if (mb->rx_index < 4) {
        return;
    }
    
    /* Extract and verify CRC */
    crc_received = mb->rx_buffer[mb->rx_index - 2] | (mb->rx_buffer[mb->rx_index - 1] << 8);
    crc_calculated = modbus_crc16(mb->rx_buffer, mb->rx_index - 2);
    
    if (crc_received != crc_calculated) {
        return;  /* CRC error */
    }
    
    slave_id = mb->rx_buffer[0];
    function_code = mb->rx_buffer[1];
    
    /* Check if this frame is for us */
    if (slave_id != mb->slave_id && slave_id != 0) {
        return;
    }
    
    /* Process function code */
    switch (function_code) {
        case MODBUS_FC_READ_HOLDING_REGISTERS:
            start_addr = (mb->rx_buffer[2] << 8) | mb->rx_buffer[3];
            count = (mb->rx_buffer[4] << 8) | mb->rx_buffer[5];
            
            /* Build response */
            mb->tx_buffer[0] = mb->slave_id;
            mb->tx_buffer[1] = function_code;
            mb->tx_buffer[2] = count * 2;  /* Byte count */
            
            /* Copy register values */
            for (i = 0; i < count; i++) {
                uint16_t reg_value = mb->data_model.holding_registers[start_addr + i];
                mb->tx_buffer[3 + i * 2] = (reg_value >> 8) & 0xFF;
                mb->tx_buffer[4 + i * 2] = reg_value & 0xFF;
            }
            
            modbus_rtu_send_frame(mb, 3 + count * 2);
            break;
            
        case MODBUS_FC_WRITE_SINGLE_REGISTER:
            start_addr = (mb->rx_buffer[2] << 8) | mb->rx_buffer[3];
            mb->data_model.holding_registers[start_addr] = (mb->rx_buffer[4] << 8) | mb->rx_buffer[5];
            
            /* Echo back the request */
            memcpy(mb->tx_buffer, mb->rx_buffer, 6);
            modbus_rtu_send_frame(mb, 6);
            break;
            
        /* Add more function code handlers here */
        
        default:
            /* Unsupported function code */
            mb->tx_buffer[0] = mb->slave_id;
            mb->tx_buffer[1] = function_code | 0x80;
            mb->tx_buffer[2] = MODBUS_EXCEPTION_ILLEGAL_FUNCTION;
            modbus_rtu_send_frame(mb, 3);
            break;
    }
}

/**
 * @brief Read holding registers (master mode)
 */
modbus_error_t modbus_rtu_read_holding_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                                  uint16_t start_addr, uint16_t count,
                                                  uint16_t *dest)
{
    modbus_error_t error;
    uint16_t i;
    
    /* Build request */
    mb->tx_buffer[0] = slave_id;
    mb->tx_buffer[1] = MODBUS_FC_READ_HOLDING_REGISTERS;
    mb->tx_buffer[2] = (start_addr >> 8) & 0xFF;
    mb->tx_buffer[3] = start_addr & 0xFF;
    mb->tx_buffer[4] = (count >> 8) & 0xFF;
    mb->tx_buffer[5] = count & 0xFF;
    
    /* Send request */
    error = modbus_rtu_send_frame(mb, 6);
    if (error != MODBUS_OK) {
        return error;
    }
    
    /* Receive response */
    error = modbus_rtu_receive_frame(mb, MODBUS_RESPONSE_TIMEOUT_MS);
    if (error != MODBUS_OK) {
        return error;
    }
    
    /* Verify response */
    if (mb->rx_buffer[0] != slave_id || mb->rx_buffer[1] != MODBUS_FC_READ_HOLDING_REGISTERS) {
        return MODBUS_ERROR_EXCEPTION;
    }
    
    /* Extract register values */
    for (i = 0; i < count; i++) {
        dest[i] = (mb->rx_buffer[3 + i * 2] << 8) | mb->rx_buffer[4 + i * 2];
    }
    
    return MODBUS_OK;
}

/**
 * @brief Write single register (master mode)
 */
modbus_error_t modbus_rtu_write_single_register(modbus_rtu_t *mb, uint8_t slave_id,
                                                 uint16_t address, uint16_t value)
{
    modbus_error_t error;
    
    /* Build request */
    mb->tx_buffer[0] = slave_id;
    mb->tx_buffer[1] = MODBUS_FC_WRITE_SINGLE_REGISTER;
    mb->tx_buffer[2] = (address >> 8) & 0xFF;
    mb->tx_buffer[3] = address & 0xFF;
    mb->tx_buffer[4] = (value >> 8) & 0xFF;
    mb->tx_buffer[5] = value & 0xFF;
    
    /* Send request */
    error = modbus_rtu_send_frame(mb, 6);
    if (error != MODBUS_OK) {
        return error;
    }
    
    /* Receive response */
    error = modbus_rtu_receive_frame(mb, MODBUS_RESPONSE_TIMEOUT_MS);
    if (error != MODBUS_OK) {
        return error;
    }
    
    /* Verify response */
    if (mb->rx_buffer[0] != slave_id || mb->rx_buffer[1] != MODBUS_FC_WRITE_SINGLE_REGISTER) {
        return MODBUS_ERROR_EXCEPTION;
    }
    
    return MODBUS_OK;
}

/* Stub implementations for other functions */
modbus_error_t modbus_rtu_read_input_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                                uint16_t start_addr, uint16_t count,
                                                uint16_t *dest)
{
    /* Implementation similar to read_holding_registers */
    return MODBUS_OK;
}

modbus_error_t modbus_rtu_read_coils(modbus_rtu_t *mb, uint8_t slave_id,
                                     uint16_t start_addr, uint16_t count,
                                     uint8_t *dest)
{
    /* Implementation for reading coils */
    return MODBUS_OK;
}

modbus_error_t modbus_rtu_write_multiple_registers(modbus_rtu_t *mb, uint8_t slave_id,
                                                    uint16_t start_addr, uint16_t count,
                                                    const uint16_t *src)
{
    /* Implementation for writing multiple registers */
    return MODBUS_OK;
}

modbus_error_t modbus_rtu_write_single_coil(modbus_rtu_t *mb, uint8_t slave_id,
                                            uint16_t address, uint16_t value)
{
    /* Implementation for writing single coil */
    return MODBUS_OK;
}
